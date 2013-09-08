#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "table.h"

#ifndef uint
typedef unsigned int uint;
#endif

uint dayofspring(const uint year)
{
	if (((THIS_YEAR(year) & 0xC00000) >> 22) == 1)
		return ((THIS_YEAR(year) & 0x3E0000) >> 17) - 1;
	return ((THIS_YEAR(year) & 0x3E0000) >> 17) - 1 + 31;
}

uint dayofyear(const uint year, const uint mon, const uint mday)
{
	static int monofday[12] = {0, 31, 59, 90, 120, 151,
		181, 212, 243, 273, 304, 334}; 
	uint day = 0;
	day = monofday[mon - 1] + mday - 1;
	if ((mon > 2) &&
			((((year % 4) == 0) && ((year % 100) != 0)) ||
			((year % 400) == 0)))
		day++;
	return day;
}

uint dayoffset(const uint year, const uint mon, const uint mday)
{
	uint yday = dayofyear(year, mon, mday);
	uint dspr = dayofspring(year);

	if (yday >= dspr) return (yday - dspr);

	uint last = dayoffset(year - 1, 12, 31);
	if (mon == 1) return (last + mday);
	return (last + 31 + mday);
}

uint monday(const uint year, const uint dayoffset)
{
	uint months = 12;
	uint monday = 0;
	uint leap = ((THIS_YEAR(year) & 0xF000) >> 12);
	uint nleap = ((THIS_YEAR(year) & 0x10000) >> 16);
	uint lmon = 0x1000;

	if (leap) {
		months = 13;
		monday |= (leap << 9);
		monday |= (nleap << 13);
	}

	uint i = 0;
	uint left = dayoffset;

	for (i = 1; i <= months ; i++)
	{
		uint mdays = 0;
		if (!leap || (i != (leap + 1))) {
			mdays = ((lmon >> i) & THIS_YEAR(year))? 30 : 29;
		} else {
			i--;
			leap = 0;
			months = 12;
			mdays = (nleap? 30 : 29);
			if (left < mdays) i = 0;
		}
		if (left < mdays) {
			monday |= (i << 5);
			monday |= left;
			return monday;
		}
		left -= mdays;
	}

	return 0;
}

uint getrealmonday(const uint year, const uint mon, const uint mday)
{
	if (dayofyear(year, mon, mday) >= dayofspring(year)) {
		return (monday(year, dayoffset(year, mon, mday)));
	}
	return (monday(year - 1, dayoffset(year, mon, mday)));
}

void ganzhi(const uint year, const uint mon, const uint mday)
{
	static char* tgGB[] = { "甲", "乙", "丙", "丁", "戊",
		"己", "庚", "辛", "壬", "癸" };
	static char* dzGB[] = { "子", "丑", "寅", "卯", "辰",
		"巳", "午", "未", "申", "酉", "戌", "亥" };

	uint i = 6, j = 8;
	uint realyear = year;
	uint lcoffset = ((0x3000000 & THIS_YEAR(year)) >> 24);

	//懒得算立春日了 -_-||
	if (dayofyear(year, mon, mday) < (31 + 2 + lcoffset)) 
		realyear -= 1;

	uint noun = realyear % 60;
	i += noun;
	i %= 10;
	j += noun;
	j %= 12;
	printf("%s%s年", tgGB[i], dzGB[j]);
}

void printmonday(uint monday)
{
	static char* numGB[] = { "初一", "初二", "初三", "初四", "初五",
		"初六", "初七", "初八", "初九", "初十",
		"十一", "十二", "十三", "十四", "十五",
		"十六", "十七", "十八", "十九", "二十",
		"廿一", "廿二", "廿三", "廿四", "廿五",
		"廿六", "廿七", "廿八", "廿九", "三十" };
	static char* monGB[] = { "正月", "二月", "三月", "四月", "五月",
		"六月", "七月", "八月", "九月", "十月", "冬月", "腊月" };
	uint mon = ((monday >> 5) & 0xF);
	uint mday = (monday & 0x1F);

	if (!mon) {
		printf("闰%s%s\n", monGB[((monday >> 9) & 0xF) - 1], numGB[mday]);
	} else {
		printf("%s%s\n", monGB[mon - 1], numGB[mday]);
	}
}

void usage(char* argv)
{
	fprintf(stderr, "%s [%d-%d] [1-12] [1-31]\n", argv,
			YEAR_SRT, YEAR_SRT + YEAR_NUM - 1);
	return;
}

int isdate(const uint year, const uint mon, const uint mday)
{
	if (mon > 12) return 0;
	if (mon == 2) {
		if ((((year % 4) ==  0) && ((year % 100) != 0)) ||
			((year % 400) == 0)) {
			if (mday > 29) return 0;
			return 1;
		}
		if (mday > 28) return 0;
		return 1;
	}
	if ((mon == 1) || (mon == 3) || (mon == 5) || (mon == 7) || (mon == 8) ||
			(mon == 10) || (mon == 12)) {
		if (mday > 31) return 0;
		return 1;
	}
	if ((mon == 2) || (mon == 4) || (mon == 6) || (mon == 9) || (mon == 11)) {
		if (mday > 30) return 0;
		return 1;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	if ((argc != 1) && (argc != 4)) {
		usage(argv[0]);
		return 1;
	}
	uint year = 0, mon = 0, mday = 0;

	int i = 1, j =0;
	if (argc == 4) {
		for (i = 1; i < 4; i++) {
			for (j = 0; j < strlen(argv[i]); j++) {
					if (!isdigit(* (argv[i] + j))) {
						usage(argv[0]);
						return 1;
					}
			}
		}
		year = atoi(argv[1]);
		mon = atoi(argv[2]);
		mday = atoi(argv[3]);
		if (!isdate(year, mon, mday)) {
			usage(argv[0]);
			return 1;
		}
	}

	if(argc == 1) {
		time_t cur_t = 0;
		time(&cur_t);
		struct tm* cur_tm = NULL;
		cur_tm = localtime(&cur_t);
		year = (cur_tm->tm_year) + 1900;
		mon = (cur_tm->tm_mon) + 1;
		mday = cur_tm->tm_mday;
	}

	if ((year < YEAR_SRT) || (year > (YEAR_SRT + YEAR_NUM - 1))) {
		usage(argv[0]);
		return 1;
	}

	if ((year == YEAR_SRT) &&
			(dayofyear(year, mon, mday) < dayofspring(year))) {
		usage(argv[0]);
		return 1;
	}

	ganzhi(year, mon, mday);
	uint monday = getrealmonday(year, mon, mday);
	if (monday) printmonday(monday);

	return 0;
}

