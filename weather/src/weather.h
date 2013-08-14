#include <curl/curl.h>
#include <time.h>
#include "mem.c"
#include "json.c"

typedef struct _WEATHER_INFO_ {
	int index;
	char* temp;
	char* weather;
	char* wind;
	int t_year;
	int t_mon;
	int t_day;
	int t_week;
	struct _WEATHER_INFO_* next;
} WeatherInfo;

typedef struct _ARG_INFO_ {
	json_object* root;
	json_object* obj;
	time_t time;
	char* data;
	char* output;
	char* citycode;
	unsigned short days;
	WeatherInfo* weather;
} ArgInfo;

int getfile(const char* url, ArgInfo* arg);

char* getcitycode(ArgInfo* arg, const char* argv[]);

unsigned short daysum(json_object* obj);
int parseinfo(ArgInfo* arg);
char* formatinfo(const ArgInfo* arg);

void EndSession(ArgInfo* arg);
int StartSession(ArgInfo* arg);
void CleanupProc(ArgInfo* arg);
int InitProc(ArgInfo* arg, const char* argv[]);
