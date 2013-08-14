
char* getcode(ArgInfo* arg, const char* match, const char* url)
{
	if (!getfile(url, arg)) {
		fprintf(stderr, "Error! Can't download file from %s.\n", url);
		return NULL;
	}

	json_object* obj = json_tokener_parse(arg->data);
	char* result= get_key(obj, match);

	if (result != NULL)
		result = newstrcat(2, NULL, result);

	json_object_put(obj);
	return result;
}

char* getcitycode(ArgInfo* arg, const char* argv[])
{
	int i;
	char* url = NULL;
	char* last = NULL;
	char* result = NULL;
	const char* type[3] = { "china", "provshi/", "station/" };
	const char* baseurl = "http://weather.com.cn/data/city3jdata/";

	for (i = 0; i < 3; i++) {
		if (i != 0) {
			if (last != NULL) {
				result = newstrcat(2, result, last);
			} else {
				if (result != NULL) tfree(result);
				return NULL;
			}
			tfree(last);
		}
		url = newstrcat(5, NULL, baseurl, type[i], result, ".html");
		last = getcode(arg,argv[i],url);
		tfree(url);
	};

	if (last == NULL) {
		tfree(result);
		return NULL;
	}
	result = newstrcat(2, result, last);
	tfree(last);
	return result;
}
