#ifndef OPENCC_GTK_H
#define OPENCC_GTK_H

#include <string.h>
#include <gtk/gtk.h>
#include <opencc/opencc.h>

#define TEXT_COLUMN 0
typedef struct _ARGV_HANDLE_ {
	GtkWindow* mwin;
	GtkComboBox* combobox;
	GtkTextView* view;
	GtkTextBuffer* buffer;
	GtkButton* clipbutton;
	GtkButton* clearbutton;
	GtkBox* box;
	GtkBox* subbox;
	GtkCellRenderer* renderer;
	GtkClipboard* clipboard;
} ArgvHandle;
const char* prompt[13] = { "暂不转换",
	"简转繁", "简转正", "简转正(仅字)", "简转正(仅词)",
	"繁转简", "繁转正", "繁转正(仅字)", "繁转正(仅词)",
	"正转简", "正转简(仅字)", "正转繁", "正转繁(仅字)" };
const char* cfgfile[12] = { "zhs2zht.ini", "zhs2zhtw_vp.ini",
	"zhs2zhtw_v.ini", "zhs2zhtw_p.ini", 
	"zht2zhs.ini", "zht2zhtw_vp.ini",
	"zht2zhtw_v.ini", "zht2zhtw_p.ini",
	"zhtw2zhcn_s.ini", "zhtw2zhs.ini",
	"zhtw2zhcn_t.ini", "zhtw2zht.ini" };

char* opencc_start(const char* from, int index);
GtkTreeModel* createmodel(ArgvHandle* handle);
gboolean combobox_changed(GtkComboBox* combobox, ArgvHandle* handle);
gboolean button_clicked(GtkButton* button, ArgvHandle* handle);
gboolean initwindow(ArgvHandle* handle);

#endif
