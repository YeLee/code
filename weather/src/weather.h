#ifndef WEATHER_H
#define WEATHER_H
#include <curl/curl.h>
#include <json-c/json.h>
#include "mem.h"

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

#endif
