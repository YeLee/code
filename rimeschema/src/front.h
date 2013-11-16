#ifndef FRONT_H
#define FRONT_H

#include "rimeschema.h"
#define TEXT_COLUMN 0

char* get_rimedirname();
GtkTreeModel* createmodel(ArgvHandle* handle);
gboolean changed_value(GtkComboBox* combobox, ArgvHandle* handle);
gboolean initwindow(ArgvHandle* handle);

#endif
