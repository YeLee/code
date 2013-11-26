#ifndef MEM_H
#define MEM_H
#include <stdlib.h>

char* ui2str(const unsigned int i);
void* tmalloc(const size_t i);
void tfree(void* mem);
char* newstrcat(const int num, char* newstart, ...);

#endif
