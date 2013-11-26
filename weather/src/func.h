#ifndef FUNC_H
#define FUNC_H

#include "weather.h"
char* get_key(json_object* obj, const char* val);
json_object* get_value_object(json_object* obj, const char* key);
const char* get_value(json_object* obj, const char* key);

int getfile(const char* url, ArgInfo* arg);

unsigned short daysum(json_object* obj);
void gettime(ArgInfo* arg);
void daysafter(ArgInfo* arg, int i, WeatherInfo* info);
int parseinfo(ArgInfo* arg);
char* formatinfo(const ArgInfo* arg);

char* getcode(ArgInfo* arg, const char* match, const char* url);
char* getcitycode(ArgInfo* arg, const char* argv[]);


#endif
