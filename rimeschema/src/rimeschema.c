#include "mem.c"
#include "rimecfg.h"
#include "front.h"

int main(int argc, char* argv[])
{
	ArgvHandle handle = {0};
	char* rimedirname;
	gtk_init(&argc, &argv);
	if ( (rimedirname = get_rimedirname()) == NULL) return 0;
	handle.head = gethead(rimedirname);
	if (!initwindow(&handle)) {
		freelist(handle.head);
		return 1;
	}
	gtk_main();

	freelist(handle.head);
	g_free(rimedirname);
	tmalloc(0);
	tfree(NULL);
	return 0;
}
