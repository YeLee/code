#ifndef HAD_JSON_C
#define HAD_JSON_C
#include <json/json.h>
#include <string.h>

#define obj2obj(x) json_object_get_object(x)
#define obj2str(x) json_object_get_string(x)

char* get_key(json_object* obj, const char* val)
{
	struct lh_entry* entry = obj2obj(obj)->head;

	while(entry) {
		if (!strcmp(obj2str((json_object*) entry->v), val)) {
			return (char*) entry->k;
		}
		if (entry->next) {
			entry = entry->next;
		} else {
			entry = NULL;
		}
	}
	return NULL;
}

json_object* get_value_object(json_object* obj, const char* key)
{
	struct lh_entry* entry = obj2obj(obj)->head;

	while(entry) {
		if (!strcmp(entry->k, key)) {
			return (json_object*) entry->v;
		}
		if (entry->next) {
			entry = entry->next;
		} else {
			entry = NULL;
		}
	}
	return NULL;
}

const char* get_value(json_object* obj, const char* key)
{
	json_object* result = get_value_object(obj, key);

	if (result == NULL) return NULL;
	return (char*) obj2str(result);
}

#endif
