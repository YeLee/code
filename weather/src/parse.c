#include <ctype.h>
#include <string.h>
#include "func.h"

unsigned short daysum(json_object* obj)
{
	unsigned short i = 0;
	char* temp = NULL;

	while(i < (unsigned short)~0) {
		temp = newstrcat(3, NULL, "temp", ui2str(i+1));
		if (get_value(obj, temp) == NULL) {
			tfree(temp);
			return i;
		}
		tfree(temp);
		i++;
	}
	return 0;
}

void gettime(ArgInfo* arg)
{
	int i = 0, j = 0, k = 0, l = 0;
	char date[3][11] = { {0}, {0}, {0} };
	const char* objdate = get_value(arg->obj, "date_y");
	struct tm time = {0};

	memset(date, '\0', sizeof(date));

	for (i = 0;i < strlen(objdate) && j < 3 && k < 10;i++) {
		if (isdigit(*(objdate + i))) {
			if (!l) k = 0;
			l = 1;
			date[j][k] = * (objdate + i);
			k++;
		} else {
			if (l) {
				j++;
				l = 0;
			}
		}
	}

	time.tm_year = atoi(date[0]) - 1900;
	time.tm_mon = atoi(date[1]) - 1;
	time.tm_mday = atoi(date[2]);
	arg->time = timegm(&time);
}

void daysafter(ArgInfo* arg, int i, WeatherInfo* info)
{
	time_t t = arg->time + (86400 * i);
	struct tm* time;

	time = gmtime(&t);

	if (info) {
		info->t_year = time->tm_year + 1900;
		info->t_mon = time->tm_mon + 1;
		info->t_day = time->tm_mday;
		info->t_week = time->tm_wday;
	}
}

int parseinfo(ArgInfo* arg)
{
	int i;
	char* index = NULL;
	char* iswap = NULL;
	WeatherInfo* current = NULL;
	WeatherInfo* prev = NULL;
	const char* key[3] = { "temp", "weather", "wind" };

	for (i = 1; i <= arg->days; i++) {
		current = tmalloc(sizeof(WeatherInfo));
		current->next = NULL;

		if (arg->weather == NULL) {
			arg->weather = current;
		} else {
			prev = arg->weather;
			while (prev->next)
				prev = prev->next;
			prev->next = current;
		}
		current->index = i;
		index = ui2str(i);

		int j;

		for (j = 0; j < 3; j++) {
			iswap = newstrcat(3, NULL, key[j], index);
			
			switch(j) {
				case 0:
					current->temp = (char*) get_value(arg->obj, iswap);
						break;
				case 1:
					current->weather = (char*) get_value(arg->obj, iswap);
					break;
				case 2:
					current->wind = (char*) get_value(arg->obj, iswap);
					break;
			}
			tfree(iswap);
		}
		daysafter(arg, i - 1, current);

	}
	return i - 1;
}

char* formatinfo(const ArgInfo* arg)
{
	const char* week[7] = {"礼拜天", "礼拜一", "礼拜二", "礼拜三", "礼拜四",
		"礼拜五", "礼拜六" };
	WeatherInfo* wi = arg->weather;
	char* output = NULL;
	char day[11] = {0};
	char mon[11] = {0};

	output = newstrcat(11, NULL, get_value(arg->obj, "city"),
			"天气预报\t当前显示：", get_value(arg->obj, "date_y"),
			"\n紫外线：", get_value(arg->obj, "index_uv"),
			"\t旅游：", get_value(arg->obj, "index_tr"),
			"\t晾晒：", get_value(arg->obj, "index_ls"), "\n");

	while(wi != NULL) {
		sprintf(day, "%02d", wi->t_day);
		sprintf(mon, "%02d", wi->t_mon);
		output = newstrcat(13, output, mon, "月",
				day, "日",
				week[wi->t_week], "\t",
				wi->temp, "\t",
				wi->wind, "\t",
				wi->weather, "\n");
		if (wi->next) {
			wi = wi->next;
		} else {
			wi = NULL;
		}
	}
	return output;
}

