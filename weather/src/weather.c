#include "weather.h"
#include "curl.c"
#include "static.c"
#include "parse.c"
#include "session.c"

void usage(const char* argv)
{
	fprintf(stderr, "%s <省> <市> <县>\n", argv);
	return;
}

int main(const int argc, const char* argv[])
{
	ArgInfo arg = {0};
	if (argc != 4) {
		usage(argv[0]);
		return 2;
	}

	if (!InitProc(&arg, argv + 1)) {
		fprintf(stderr, "Error! Can't Initialization.\n");
		return 1;
	}

	if (!StartSession(&arg)) {
		fprintf(stderr, "Error! Can't Start Session.\n");
		return 1;
	}

	//Do something
	printf("%s", arg.output);

	EndSession(&arg);
	CleanupProc(&arg);
	return 0;
}

