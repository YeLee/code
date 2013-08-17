#include "opencc-gtk.h"
#include "front.c"

char* opencc_start(const char* from, int index)
{
	opencc_t od = opencc_open(cfgfile[index]);
	char* result = NULL;

	if (od == (opencc_t) -1) {
		fprintf(stderr, "Failed to Initialization OPENCC!\n");
		return NULL;
	}

	result = opencc_convert_utf8(od, from, -1);
	opencc_close(od);

	return result;
}

int main(int argc, char* argv[])
{
	ArgvHandle handle = {0};
	gtk_init(&argc, &argv);

	if (!initwindow(&handle)) {
		return 1;
	}
	gtk_main();

	return 0;
}
