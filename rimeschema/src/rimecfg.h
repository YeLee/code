#ifndef RIMECFG_H
#define RIMECFG_H

#include <dirent.h>
#include "rimeschema.h"

#define _TOKEN_DATA_ token.data.scalar.value
#define MARK_PROC 0X0001
#define MARK_MATCH 0X0002
#define STAT_KEY 0X0001
#define STAT_VALUE 0X0002
#define TYPE_ID 0X0001
#define TYPE_NAME 0X0002
#define TYPE_AUTHOR 0X0004
#define TYPE_DESC 0X0008
#define VOIDBIT 0x0000
typedef unsigned short bitint;

#define POINTLIST(obj) (PointList *) obj

typedef struct _STRUCT_POINT_LIST_ {
	void* data;
	struct _STRUCT_POINT_LIST_* next;
} PointList;

RimeSchemaTable* gethead(char* rimedirname);
PointList* savelist(PointList* head, void* data);
void freelist(RimeSchemaTable* head);
RimeSchemaInfo* getschemainfo(const char* id);

#endif
