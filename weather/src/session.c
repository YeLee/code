#include "func.h"
#include "session.h"


void EndSession(ArgInfo* arg)
{
	WeatherInfo* current = arg->weather;
	WeatherInfo* prev;

	while(current != NULL) {
		prev = current;
		if (current->next == NULL) {
			current = NULL;
		} else {
			current = current->next;
		}
		tfree(prev);
	}

	if (arg->data != NULL) tfree(arg->data);
	if (arg->output != NULL) tfree(arg->output);

	json_object_put(arg->root);
	return;
}

int StartSession(ArgInfo* arg)
{
	const char* baseurl = "http://m.weather.com.cn/data/";
	char* infourl = newstrcat(4, NULL, baseurl, arg->citycode, ".html");

	if (!getfile(infourl, arg)) {
		fprintf(stderr, "Error! Can't get weather info.\n");
		tfree(infourl);
		return 0;
	}
	tfree(infourl);

	arg->root = json_tokener_parse(arg->data);
	arg->obj = get_value_object(arg->root, "weatherinfo");

	arg->days = daysum(arg->obj);
	if (arg->days == 0) return 0;

	gettime(arg);
	parseinfo(arg);
	arg->output = formatinfo(arg);
	return 1;
}

void CleanupProc(ArgInfo* arg, const int argc)
{
	curl_global_cleanup();
	if ((argc == 4) && (arg->citycode != NULL)) tfree(arg->citycode);

	tmalloc(0);
	tfree(NULL);
	return;
}

int InitProc(ArgInfo* arg, const int argc, const char* argv[])
{
	CURLcode code = curl_global_init(CURL_GLOBAL_ALL) ;

	if (code != CURLE_OK) {
		fprintf(stderr, "Error! %s!\n", curl_easy_strerror(code));
		return 0;
	}

	if (argc == 4) {
		arg->citycode = getcitycode(arg, argv);
		if (arg->citycode == NULL) {
			fprintf(stderr, "Error! Can't get citycode.\n");
			return 0;
		}
	} else {
		arg->citycode = (char*) argv[0];
	}

	return 1;
}
