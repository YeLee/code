#include "front.h"

char* get_rimedirname()
{
	char* rimedirname = NULL;
	GtkFileChooser* filechooser = GTK_FILE_CHOOSER(gtk_file_chooser_dialog_new("Rime Schema List",
			NULL, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
			GTK_STOCK_CANCEL,  GTK_RESPONSE_CANCEL,
			GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL));
	if (gtk_dialog_run(GTK_DIALOG(filechooser)) == GTK_RESPONSE_ACCEPT)
		rimedirname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser));
	gtk_widget_destroy(GTK_WIDGET(filechooser));
	return rimedirname;
}

GtkTreeModel* createmodel(ArgvHandle* handle)
{
	GtkListStore* list = gtk_list_store_new(1, G_TYPE_STRING);
	GtkTreeIter iter = {0};
	RimeSchemaTable* current = handle->head;

	while(current) {
		gtk_list_store_append(list, &iter);
		gtk_list_store_set(list, &iter, TEXT_COLUMN, current->data->name, -1);

		if (current->next) {
			current = current->next;
		} else {
			current = NULL;
		}

	}

	return GTK_TREE_MODEL(list);
}

gboolean changed_value(GtkComboBox* combobox, ArgvHandle* handle)
{
	GtkTreeModel* model = gtk_combo_box_get_model(combobox);
	GtkTreeIter iter = {0};
	char* schema_name = NULL;
	char* str = NULL;
	RimeSchemaTable* current = handle->head;

	gtk_combo_box_get_active_iter(combobox, &iter);
	gtk_tree_model_get(model, &iter, TEXT_COLUMN, &schema_name, -1);

	while(current) {
		if (!strcmp(current->data->name, schema_name)) {
			str = newstrcat(6, NULL, "Schema_id:\n\t", current->data->id,
					"\nSchema_name:\n\t", current->data->name,
					"\nSchema_author:");
			RimeNameList* currentauthor = current->data->author;

			while(currentauthor) {
				str = newstrcat(3, str, "\n\t", currentauthor->data);

				if (currentauthor->next) {
					currentauthor = currentauthor->next;
				} else {
					currentauthor = NULL;
				}

			}

			str = newstrcat(3, str,
					"\nSchema_description:\n", current->data->desc);
	 		gtk_label_set_text(handle->label, str);
			tfree(str);
		}

		if (current->next) {
			current = current->next;
		} else {
			current = NULL;
		}

	}
	return TRUE;
}

gboolean initwindow(ArgvHandle* handle)
{
	handle->mwin = GTK_WINDOW(gtk_window_new(GTK_WINDOW_TOPLEVEL));
	if (!handle->mwin) return FALSE;

	gtk_window_set_title(handle->mwin, "Rime Schema List");
	gtk_window_set_default_size(handle->mwin, 300, 200);
	gtk_window_set_position(handle->mwin, GTK_WIN_POS_CENTER);
	gtk_window_set_opacity(handle->mwin, 0.9);
	//gtk_window_set_skip_taskbar_hint(handle->mwin, TRUE);
	//gtk_window_set_skip_pager_hint(handle->mwin, TRUE);
	g_signal_connect(handle->mwin, "destroy",
			G_CALLBACK(gtk_main_quit), NULL);

	handle->combobox = GTK_COMBO_BOX(gtk_combo_box_new_with_model
			(createmodel(handle)));
	if (!handle->combobox) return FALSE;

	handle->renderer = gtk_cell_renderer_text_new();
	if (!handle->renderer) return FALSE;
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(handle->combobox),
			handle->renderer, FALSE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(handle->combobox),
			handle->renderer, "text", TEXT_COLUMN, NULL);

	handle->label = GTK_LABEL(gtk_label_new(NULL));
	if (!handle->label) return FALSE;
	#ifdef ENABLE_GTK3_SUPPORT
	handle->box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
	#else
	handle->box = GTK_BOX(gtk_vbox_new(FALSE, 0));
	#endif
	if (!handle->box) return FALSE;
	gtk_box_pack_start(handle->box,
			GTK_WIDGET(handle->combobox), FALSE, FALSE, 0);
	gtk_box_pack_start(handle->box,
			GTK_WIDGET(handle->label), TRUE, TRUE, 0);

	gtk_container_add(GTK_CONTAINER(handle->mwin), GTK_WIDGET(handle->box));
	g_signal_connect(handle->combobox, "changed",
			G_CALLBACK(changed_value), handle);

	gtk_widget_show_all(GTK_WIDGET(handle->mwin));
	return TRUE;
}

