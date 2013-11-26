#include "weather.h"
#include "session.h"

void usage(const char* argv)
{
	fprintf(stderr, "查询城市代码：%s <省> <市> <县>\n", argv);
	fprintf(stderr, "查询天气信息：%s <城市代码>\n", argv);
	return;
}

int main(const int argc, const char* argv[])
{
	ArgInfo arg = {0};
	if ((argc != 2) && (argc != 4)) {
		usage(argv[0]);
		return 2;
	}

	if (!InitProc(&arg, argc, argv + 1)) {
		fprintf(stderr, "Error! Can't Initialization.\n");
		return 1;
	}

	if (argc == 4) {
		printf("%s %s %s的城市代码是：%s\n",
				argv[1], argv[2], argv[3], arg.citycode);
		return 0;
	}

	if (!StartSession(&arg)) {
		fprintf(stderr, "Error! Can't Start Session.\n");
		return 1;
	}

	//Do something
	printf("%s", arg.output);

	EndSession(&arg);
	CleanupProc(&arg, argc);
	return 0;
}

