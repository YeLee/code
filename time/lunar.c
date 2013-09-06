#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "table.h"

#ifndef uint
typedef unsigned int uint;
#endif

uint dayofspring(const uint year)
{
	if (((THIS_YEAR(year) & 0x0060) >> 5) == 1)
		return (THIS_YEAR(year) & 0x001F) - 1;
	else
		return (THIS_YEAR(year) & 0x001F) - 1 + 31;
}

uint dayofyear(const uint year, const uint mon, const uint mday)
{
	static int monofday[12] = {0, 31, 59, 90, 120, 151,
		181, 212, 243, 273, 304, 334}; 
	uint day = 0;
	day = monofday[mon - 1] + mday - 1;
	if (((year % 4) ==  0) && (mon > 2) &&
			((year % 100) != 0) || ((year % 400) == 0))
		day++;
	return day;
}

uint lunardays(const uint year)
{
	uint sum = 29 * 12;
	uint months = 0;
	uint i = 0;
	long offset = 0L;

	if (THIS_YEAR(year) & 0xF00000) {
		months = 13;
		sum += 29;
		offset = 0x40L;
	} else {
		months = 12;
		offset = 0x80L;
	}

	for (i = 1; i <= months; i++) {
		if ((offset << i) & THIS_YEAR(year)) sum++;
	}

	return sum;
}

uint dayoffset(const uint year, const uint mon, const uint mday)
{
	uint offset = 0;
	uint yday = dayofyear(year, mon, mday);
	uint dspr = dayofspring(year);

	if (yday >= dspr) return (yday - dspr);

	uint last = dayoffset(year - 1, 12, 31);
	if (mon == 1) return (last + mday);
	if (mon == 2) return (last + 31 + mday);
}

uint monday(const uint year, const uint dayoffset)
{
	uint months = 12;
	const long lmon = 0x100000L;
	uint monday = 0;
	uint leap = ((THIS_YEAR(year) & 0xF00000) >> 20);

	if (leap) {
		months = 13;
		monday |= (leap << 11);
	}

	uint i = 0;
	uint left = dayoffset;

	for (i = 1; i <= months ; i++)
	{
		uint mdays = ((lmon >> i) & THIS_YEAR(year))? 30 : 29;
		if (left < mdays) {
			if (leap && ((i - 1) == leap)) i = 0;
			if (leap && (i != 0) && ((i - 1) > leap)) i--;
			monday |= (i << 6);
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
	} else {
		return (monday(year - 1, dayoffset(year, mon, mday)));
	}
}

uint ganzhi(const uint year, const uint mon, const uint mday)
{
	static char* tgGB[] = { "甲", "乙", "丙", "丁", "戊",
		"己", "庚", "辛", "壬", "癸" };
	static char* dzGB[] = { "子", "丑", "寅", "卯", "辰",
		"巳", "午", "未", "申", "酉", "戌", "亥" };

	uint i = 6, j = 8;
	uint realyear = year;

	//懒得算春分日了 -_-||
	if (dayofyear(year, mon, mday) < 34) 
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
	static char numGB[30][9] = { "初一", "初二", "初三", "初四", "初五",
		"初六", "初七", "初八", "初九", "初十",
		"十一", "十二", "十三", "十四", "十五",
		"十六", "十七", "十八", "十九", "廿十",
		"廿一", "廿二", "廿三", "廿四", "廿五",
		"廿六", "廿七", "廿八", "廿九", "卅十" };
	static char monGB[12][9] = { "正月", "二月", "三月", "四月", "五月",
		"六月", "七月", "八月", "九月", "十月", "冬月", "腊月" };
	uint mon = ((monday >> 6) & 0xF);
	uint mday = (monday & 0x1F);

	if (!mon) {
		printf("闰%s%s\n", monGB[(monday >> 11) - 1], numGB[mday]);
	} else {
		printf("%s%s\n", monGB[mon - 1], numGB[mday]);
	}
	return;
}

void usage(char* argv)
{
	fprintf(stderr, "%s 年 月 日\n", argv);
	return;
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
		if ((year < YEAR_SRT) || (year > (YEAR_SRT + YEAR_NUM - 1))) {
			usage(argv[0]);
			return 1;
		}
		if ((mon < 1) || (mon > 12)) {
			usage(argv[0]);
			return 1;
		}
		if ((mday < 1) || (mday > 31)) {
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

	ganzhi(year, mon, mday);
	uint monday = getrealmonday(year, mon, mday);
	if (monday) printmonday(monday);

	return 0;
}

