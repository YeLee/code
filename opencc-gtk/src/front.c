GtkTreeModel* createmodel(ArgvHandle* handle)
{
	GtkListStore* list = gtk_list_store_new(1, G_TYPE_STRING);
	GtkTreeIter iter = {0};
	int i;

	for(i = 0; i < 13; i++) {
		gtk_list_store_append(list, &iter);
		gtk_list_store_set(list, &iter, TEXT_COLUMN, prompt[i], -1);
	}

	return GTK_TREE_MODEL(list);
}

gboolean combobox_changed(GtkComboBox* combobox, ArgvHandle* handle)
{
	GtkTreeModel* model = gtk_combo_box_get_model(combobox);
	GtkTreeIter iter = {0};
	GtkTextIter start = {0}, end = {0};
	char* type = NULL;
	char* context = NULL;
	char* result = NULL;
	int i;

	gtk_combo_box_get_active_iter(combobox, &iter);
	gtk_tree_model_get(model, &iter, TEXT_COLUMN, &type, -1);

	for(i = 0; i < 13 && strcmp(type, prompt[i]); i++);
	if (i == 0) return FALSE;

	gtk_text_buffer_get_bounds(handle->buffer, &start, &end);
	context = gtk_text_buffer_get_text(handle->buffer, &start, &end, FALSE);
	if (strlen(context) <= 0) return FALSE;

	result = opencc_start(context, i - 1);
	if (result == NULL) return FALSE;
	gtk_text_buffer_set_text(handle->buffer, result, strlen(result));
	opencc_convert_utf8_free(result);
	return TRUE;
}

gboolean button_clicked(GtkButton* button, ArgvHandle* handle)
{
	GtkTextIter start = {0}, end = {0};
	char* name = NULL;
	char* context = NULL;

	gtk_text_buffer_get_bounds(handle->buffer, &start, &end);

	if(!strcmp(gtk_widget_get_name(
					GTK_WIDGET(button)), "clearbutton")) {
		gtk_text_buffer_delete(handle->buffer, &start, &end);
	} else if(!strcmp(gtk_widget_get_name(
					GTK_WIDGET(button)), "clipbutton")) {
		context = gtk_text_buffer_get_text(handle->buffer,
				&start, &end, FALSE);
		if (strlen(context) <= 0) return FALSE;
		gtk_clipboard_set_text(handle->clipboard, context, -1);
	}
	return TRUE;
}

gboolean initwindow(ArgvHandle* handle)
{
	handle->mwin = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
	if (handle->mwin == NULL) return FALSE;

	gtk_window_set_title(handle->mwin, "又一个OpenCC的GTK前端");
	gtk_window_set_default_size(handle->mwin, 500, 400);
	gtk_window_set_position(handle->mwin, GTK_WIN_POS_CENTER);
	gtk_window_set_opacity(handle->mwin, 0.9);
	//gtk_window_set_skip_taskbar_hint(handle->mwin, TRUE);
	//gtk_window_set_skip_pager_hint(handle->mwin, TRUE);
	g_signal_connect(handle->mwin, "destroy",
			G_CALLBACK(gtk_main_quit), NULL);

	handle->combobox = GTK_COMBO_BOX(gtk_combo_box_new_with_model
			(createmodel(handle)));
	if (handle->combobox == NULL) return FALSE;

	handle->buffer = gtk_text_buffer_new(NULL);
	if (handle->buffer == NULL) return FALSE;

	handle->view = GTK_TEXT_VIEW(
			gtk_text_view_new_with_buffer(handle->buffer));
	if (handle->view == NULL) return FALSE;

	gtk_text_view_set_wrap_mode(handle->view, GTK_WRAP_WORD_CHAR);

	handle->renderer = gtk_cell_renderer_text_new();
	if (handle->renderer == NULL) return FALSE;

	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(handle->combobox),
			handle->renderer, FALSE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(handle->combobox),
			handle->renderer, "text", TEXT_COLUMN, NULL);

	handle->clearbutton =
		GTK_BUTTON(gtk_button_new_with_label("清空"));
	if (handle->clearbutton == NULL) return FALSE;
	gtk_widget_set_name(GTK_WIDGET(handle->clearbutton), "clearbutton");

	#ifdef USE_SELECTION_PRIMARY
	handle->clipbutton =
		GTK_BUTTON(gtk_button_new_with_label("拷贝到主选区"));
	handle->clipboard = gtk_clipboard_get(GDK_SELECTION_PRIMARY);
	#else
	handle->clipbutton =
		GTK_BUTTON(gtk_button_new_with_label("拷贝到剪切板"));
	handle->clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
	#endif
	if (handle->clipbutton == NULL) return FALSE;
	if (handle->clipboard == NULL) return FALSE;
	gtk_widget_set_name(GTK_WIDGET(handle->clipbutton), "clipbutton");

	#ifdef ENABLE_GTK3_SUPPORT
	handle->subbox = GTK_BOX(gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5));
	handle->box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 8));
	#else
	handle->subbox = GTK_BOX(gtk_hbox_new(FALSE, 5));
	handle->box = GTK_BOX(gtk_vbox_new(FALSE, 8));
	#endif
	if (handle->subbox == NULL) return FALSE;
	if (handle->box == NULL) return FALSE;

	gtk_box_pack_start(handle->subbox,
			GTK_WIDGET(handle->combobox), TRUE, TRUE, 0);
	gtk_box_pack_start(handle->subbox,
			GTK_WIDGET(handle->clearbutton), FALSE, FALSE, 0);
	gtk_box_pack_start(handle->subbox,
			GTK_WIDGET(handle->clipbutton), FALSE, FALSE, 0);

	gtk_box_pack_start(handle->box,
			GTK_WIDGET(handle->subbox), FALSE, FALSE, 0);
	gtk_box_pack_start(handle->box,
			GTK_WIDGET(handle->view), TRUE, TRUE, 0);

	g_signal_connect(handle->combobox, "changed",
			G_CALLBACK(combobox_changed), handle);
	g_signal_connect(handle->clearbutton, "clicked",
			G_CALLBACK(button_clicked), handle);
	g_signal_connect(handle->clipbutton, "clicked",
			G_CALLBACK(button_clicked), handle);

	gtk_container_add(GTK_CONTAINER(handle->mwin), GTK_WIDGET(handle->box));
	gtk_container_set_border_width(GTK_CONTAINER(handle->mwin), 8);

	gtk_widget_show_all(GTK_WIDGET(handle->mwin));
	return TRUE;
}

