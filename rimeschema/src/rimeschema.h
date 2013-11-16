#ifndef RIMESCHEMA_H
#define RIMESCHEMA_H
#include <gtk/gtk.h>

typedef struct _RIME_NAME_LIST_ {
	char* data;
	struct _RIME_NAME_LIST_* next;
}RimeNameList;

typedef struct _STRUCT_RIME_SCHEMA_INFO_ {
	char* id;
	char* name;
	RimeNameList* author;
	char* desc;
} RimeSchemaInfo;

typedef struct _STRUCT_RIME_SCHEMA_TABLE_ {
	RimeSchemaInfo* data;
	struct _STRUCT_RIME_SCHEMA_TABLE_* next;
} RimeSchemaTable;

typedef struct _ARGV_HANDLE_ {
	GtkWindow* mwin;
	GtkComboBox* combobox;
	GtkLabel* label;
	GtkBox* box;
	GtkCellRenderer* renderer;
	RimeSchemaTable* head;
} ArgvHandle;

char* ui2str(const unsigned int i);
void* tmalloc(const size_t i);
void tfree(void* mem);
char* newstrcat(const int num, char* newstart, ...);

#endif
