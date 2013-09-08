#define HTTP_SUCCESS 200

size_t writefunc(void *ptr, size_t size, size_t nmemb, void* userdata)
{
	int iwrite = 0;
	ArgInfo* arg = (ArgInfo*) userdata;

	if (ptr) {
		arg->data = newstrcat(2, arg->data, ptr);
		iwrite = nmemb *size;
	}
	return iwrite;
}

int getfile(const char* url, ArgInfo* arg)
{
	CURLcode code;
	CURL* handle = NULL;
	long result;

	if ((handle = curl_easy_init()) == NULL) {
		fprintf(stderr, "Error! Can't init handle!\n");
		return 0;
	}

	if ((code = curl_easy_setopt(handle, CURLOPT_URL, url)) != CURLE_OK) {
		fprintf(stderr, "Error! %s!\n", curl_easy_strerror(code));
		curl_easy_cleanup(handle);
		return 0;
	}

	if ((code = curl_easy_setopt(handle, CURLOPT_TIMEOUT, 15)) != CURLE_OK) {
		fprintf(stderr, "Error! %s!\n", curl_easy_strerror(code));
		curl_easy_cleanup(handle);
		return 0;
	}

	if ((code = curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0"))
		!= CURLE_OK) {
		fprintf(stderr, "Error! %s!\n", curl_easy_strerror(code));
		curl_easy_cleanup(handle);
		return 0;
	}

	if (arg->data != NULL) {
		tfree(arg->data);
		arg->data = NULL;
	}

	if ((code = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writefunc))
			!= CURLE_OK) {
		fprintf(stderr, "Error! %s!\n", curl_easy_strerror(code));
		curl_easy_cleanup(handle);
		return 0;
	}

	if ((code = curl_easy_setopt(handle, CURLOPT_WRITEDATA, arg)) != CURLE_OK) {
		fprintf(stderr, "Error! %s!\n", curl_easy_strerror(code));
		curl_easy_cleanup(handle);
		return 0;
	}

	if ((code = curl_easy_perform(handle)) != CURLE_OK) {
		fprintf(stderr, "Error! %s!\n", curl_easy_strerror(code));
		curl_easy_cleanup(handle);
		return 0;
	}

	curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &result);
	if (result != HTTP_SUCCESS) {
		fprintf(stderr, "Error! Connection Error!\n");
		curl_easy_cleanup(handle);
		return 0;
	}

	curl_easy_cleanup(handle);
	return 1;
}


