#ifndef HAD_MEM_C
#define HAD_MEM_C
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

char* ui2str(const unsigned int i)
{
	static char result[11];
	memset(result, '\0', 11);
	sprintf(result, "%u", i);
	return result;
}

void* tmalloc(const size_t i)
{
	static int imalloc = 0;
	void* addr = NULL;

	if (i == 0) {
#ifndef NDEBUG
		fprintf(stderr, "Allocated:%d\n", imalloc);
#endif
		return NULL;
	}
	imalloc ++;

	addr = malloc(i);
#ifndef NDEBUG
	fprintf(stderr, "Allocated:0x%lx\n", (unsigned long) addr);
#endif
	memset(addr, '\0', sizeof(addr));
	return addr;
}

void tfree(void* mem)
{
	static int ifree = 0;

	if (mem == NULL) {
#ifndef NDEBUG
		fprintf(stderr, "Recovered:%d\n", ifree);
#endif
		return;
	}
	ifree ++;

	free(mem);
#ifndef NDEBUG
	fprintf(stderr, "Recovered:0x%lx\n", (unsigned long) mem);
#endif
	mem = NULL;
	return;
}

char* newstrcat(const int num, char* newstart, ...)
{
	int len = 0;
	int i;
	va_list ap;
	char* newstr = NULL;
	char* argstr = NULL;

	if (newstart) len = strlen(newstart);

	va_start(ap, newstart);

		for (i = 1; i < num; i++) {
			argstr = (char*) va_arg(ap, char*);
			if (argstr != NULL) len += strlen(argstr);
		}

	va_end(ap);
	
	newstr = tmalloc((len + 1) * sizeof(char));
	memset(newstr, '\0', sizeof(newstr));
	if (newstart) {
		strcpy(newstr, newstart);
		tfree(newstart);
	}

	va_start(ap, newstart);

		for (i = 1; i < num; i++) {
			argstr = (char*) va_arg(ap, char*);
			if (argstr != NULL) strcat(newstr, argstr);
		}

	va_end(ap);

	return newstr;
}

#endif
