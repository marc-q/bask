#ifndef __BASK_TIME_H__
#define __BASK_TIME_H__

short time_get_tm (struct tm**);
short time_get_str (char*, size_t);
short time_get_str_described (char*, size_t, char*);
short time_get_tm_str (struct tm*, char*);

int time_get_hours (char*);
int time_get_minutes (char*);
int time_get_seconds (char*);
int time_get_day (char*);
int time_get_month (char*);
int time_get_year (char*);

#endif /* __BASK_TIME_H__ */
