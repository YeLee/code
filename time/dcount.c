#include <stdio.h>
#include <string.h>
#include <time.h>

void usage(char* argv)
{
	fprintf(stderr, "%s 年 月 日\n", argv);
	return;
}

int main(int argc, char* argv[])
{
	if (argc != 4) {
		usage(argv[0]);
		return 1;
	}

	int i = 1, j =0;
	for (i = 1; i < 4; i++) {
		for (j = 0; j < strlen(argv[i]); j++) {
				if (!isdigit(* (argv[i] + j))) {
					usage(argv[0]);
					return 1;
				}
		}
	}

	struct tm tar_tm = {0};
	time_t cur_t = 0L;
	time_t tar_t = 0L;

	time(&cur_t);

	tar_tm.tm_year = atoi(argv[1]) - 1900;
	tar_tm.tm_mon = atoi(argv[2]) - 1;
	tar_tm.tm_mday = atoi(argv[3]);
	tar_t = timelocal(&tar_tm);
	tar_t -= cur_t;

	if (tar_t > -86400L && tar_t < 0L) {
		printf("\e[1;4;32m%4d\e[0m年\e[1;4;32m%02d\e[0m月\e[1;4;32m"
				"%02d\e[0m日\e[1;4;31m就是今天\e[0m。\n",
				atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
	} else if (tar_t < -86400L) {
		printf("距\e[1;4;32m%4d\e[0m年\e[1;4;32m%02d\e[0m月"
				"\e[1;4;32m%02d\e[0m日已过去\e[1;4;31m%ld\e[0m天。\n",
				atoi(argv[1]), atoi(argv[2]), atoi(argv[3]),
				(0L - (tar_t / 86400L)));
	} else if (tar_t > 0L) {
		printf("距\e[1;4;32m%4d\e[0m年\e[1;4;32m%02d\e[0m月"
				"\e[1;4;32m%02d\e[0m日还有\e[1;4;31m%ld\e[0m天。\n",
				atoi(argv[1]), atoi(argv[2]), atoi(argv[3]),
				((tar_t + 86400L) / 86400L));
	}

	return 0;
}
