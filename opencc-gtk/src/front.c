GtkTreeModel* createmodel(ArgvHandle* handle)
{
	GtkListStore* list = gtk_list_store_new(1, G_TYPE_STRING);
	GtkTreeIter iter = {0};
	int i;

	for (i = 0; i < 13; i++) {
		gtk_list_store_append(list, &iter);
		gtk_list_store_set(list, &iter, TEXT_COLUMN, prompt[i], -1);
	}

	return GTK_TREE_MODEL(list);
}

gboolean combobox_changed(GtkComboBox* combobox, ArgvHandle* handle)
{
	GtkTreeModel* model = gtk_combo_box_get_model(combobox);
	GtkTreeIter iter = {0};
	char* type = NULL;
	char* result = NULL;
	int i;

	gtk_combo_box_get_active_iter(combobox, &iter);
	gtk_tree_model_get(model, &iter, TEXT_COLUMN, &type, -1);

	for (i = 0; i < 13 && strcmp(type, prompt[i]); i++);
	if (i == 0) return FALSE;

	gtk_text_buffer_get_bounds(
			handle->buffer, handle->start, handle->end);
	handle->context = gtk_text_buffer_get_text(
			handle->buffer, handle->start, handle->end, FALSE);
	if (strlen(handle->context) < 1) return FALSE;

	if (handle->undocontext != NULL) free(handle->undocontext);
	handle->undocontext = malloc((strlen(handle->context) + 1) * sizeof(char));
	strcpy(handle->undocontext, handle->context);

	result = opencc_start(handle->context, i - 1);
	if (result == NULL) return FALSE;
	gtk_text_buffer_set_text(handle->buffer, result, -1);
	gtk_widget_set_sensitive(GTK_WIDGET(handle->undobutton), TRUE);
	opencc_convert_utf8_free(result);
	return TRUE;
}

gboolean button_clicked(GtkButton* button, ArgvHandle* handle)
{
	char* name = NULL;

	gtk_text_buffer_get_bounds(
			handle->buffer, handle->start, handle->end);

	if (!strcmp(gtk_widget_get_name(
					GTK_WIDGET(button)), "undobutton")) {
		if (handle->undocontext != NULL) {
			gtk_text_buffer_set_text(handle->buffer,
					handle->undocontext, -1);
			gtk_widget_set_sensitive(GTK_WIDGET(handle->undobutton), FALSE);
		}
	} else if (!strcmp(gtk_widget_get_name(
					GTK_WIDGET(button)), "clearbutton")) {
		gtk_text_buffer_delete(
				handle->buffer, handle->start, handle->end);
	} else if (!strcmp(gtk_widget_get_name(
					GTK_WIDGET(button)), "clipbutton")) {
		handle->context = gtk_text_buffer_get_text(
				handle->buffer, handle->start, handle->end, FALSE);
		if (strlen(handle->context) < 1) return FALSE;
		gtk_clipboard_set_text(handle->clipboard, handle->context, -1);
	}
	return TRUE;
}

gboolean initwindow(ArgvHandle* handle)
{
	GtkWindow* mwin = NULL;
	GtkComboBox* combobox = NULL;
	GtkCellRenderer* renderer = NULL;
	GtkButton* clipbutton = NULL;
	GtkButton* clearbutton = NULL;
	GtkBox* box = NULL;
	GtkBox* subbox = NULL;

	mwin = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
	if (mwin == NULL) return FALSE;

	gtk_window_set_title(mwin, "又一个OpenCC的GTK前端");
	gtk_window_set_default_size(mwin, 500, 400);
	gtk_window_set_position(mwin, GTK_WIN_POS_CENTER);
	gtk_window_set_opacity(mwin, 0.9);
	g_signal_connect(mwin, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	combobox = GTK_COMBO_BOX(gtk_combo_box_new_with_model
			(createmodel(handle)));
	if (combobox == NULL) return FALSE;

	handle->buffer = gtk_text_buffer_new(NULL);
	if (handle->buffer == NULL) return FALSE;

	handle->view = GTK_TEXT_VIEW(
			gtk_text_view_new_with_buffer(handle->buffer));
	if (handle->view == NULL) return FALSE;

	gtk_text_view_set_wrap_mode(handle->view, GTK_WRAP_WORD_CHAR);

	renderer = gtk_cell_renderer_text_new();
	if (renderer == NULL) return FALSE;

	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combobox), renderer, FALSE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combobox),
			renderer, "text", TEXT_COLUMN, NULL);

	handle->undobutton = GTK_BUTTON(gtk_button_new_with_label("还原"));
	if (handle->undobutton == NULL) return FALSE;
	gtk_widget_set_name(GTK_WIDGET(handle->undobutton), "undobutton");
	gtk_widget_set_sensitive(GTK_WIDGET(handle->undobutton), FALSE);

	clearbutton = GTK_BUTTON(gtk_button_new_with_label("清空"));
	if (clearbutton == NULL) return FALSE;
	gtk_widget_set_name(GTK_WIDGET(clearbutton), "clearbutton");

	#ifdef USE_SELECTION_PRIMARY
	clipbutton = GTK_BUTTON(gtk_button_new_with_label("拷贝到主选区"));
	handle->clipboard = gtk_clipboard_get(GDK_SELECTION_PRIMARY);
	#else
	clipbutton = GTK_BUTTON(gtk_button_new_with_label("拷贝到剪切板"));
	handle->clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
	#endif
	if (clipbutton == NULL) return FALSE;
	if (handle->clipboard == NULL) return FALSE;
	gtk_widget_set_name(GTK_WIDGET(clipbutton), "clipbutton");

	#ifdef ENABLE_GTK3_SUPPORT
	subbox = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5));
	box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 8));
	#else
	subbox = GTK_BOX(gtk_hbox_new(FALSE, 5));
	box = GTK_BOX(gtk_vbox_new(FALSE, 8));
	#endif
	if (subbox == NULL) return FALSE;
	if (box == NULL) return FALSE;

	gtk_box_pack_start(subbox, GTK_WIDGET(combobox), TRUE, TRUE, 0);
	gtk_box_pack_start(subbox,
			GTK_WIDGET(handle->undobutton), FALSE, FALSE, 0);
	gtk_box_pack_start(subbox, GTK_WIDGET(clearbutton), FALSE, FALSE, 0);
	gtk_box_pack_start(subbox, GTK_WIDGET(clipbutton), FALSE, FALSE, 0);

	gtk_box_pack_start(box, GTK_WIDGET(subbox), FALSE, FALSE, 0);
	gtk_box_pack_start(box, GTK_WIDGET(handle->view), TRUE, TRUE, 0);

	g_signal_connect(combobox, "changed",
			G_CALLBACK(combobox_changed), handle);
	g_signal_connect(handle->undobutton, "clicked",
			G_CALLBACK(button_clicked), handle);
	g_signal_connect(clearbutton, "clicked",
			G_CALLBACK(button_clicked), handle);
	g_signal_connect(clipbutton, "clicked",
			G_CALLBACK(button_clicked), handle);

	gtk_container_add(GTK_CONTAINER(mwin), GTK_WIDGET(box));
	gtk_container_set_border_width(GTK_CONTAINER(mwin), 8);

	gtk_widget_show_all(GTK_WIDGET(mwin));
	return TRUE;
}

