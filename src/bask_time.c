/* Copyright 2015 - 2016 Marc Volker Dickmann */
#define _XOPEN_SOURCE  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bask_core.h"
#include "bask_errors.h"
#include "bask_time.h"

/* |--------------------------------------------|
   |			Time			|
   |--------------------------------------------| */

/*
	Function: time_get_tm (struct tm** out);
	Description: Builds a tm structure with the current date.
	InitVersion: 0.0.1
*/
short time_get_tm (struct tm** out)
{
	time_t t;
	
	t = time (NULL);
	*out = localtime (&t);
	
	if (*out == NULL)
	{
		return -1;
	}
	
	return 0;
}

/*
	Function: time_get_str (char* out, size_t outsize);
	Description: Builds a string with the current date.
	InitVersion: 0.0.1
*/
short time_get_str (char* out, size_t outsize)
{
	struct tm* tmp;
	
	if (time_get_tm (&tmp) != 0)
	{
		errors_timenotgot ();
		return -1;
	}
	
	if (strftime (out, outsize, "%H/%M/%S/%d/%m/%Y", tmp) == 0)
	{
		errors_timenotgot ();
		return -2;
	}
	
	return 0;
}

/*
	Function: time_get_str_described (char* out, size_t outsize, char* desc);
	Description: Builds a string with the described date.
	InitVersion: 0.0.1
*/
short time_get_str_described (char* out, size_t outsize, char* desc)
{
	struct tm* tmp;
	
	if (time_get_tm (&tmp) != 0)
	{
		errors_timenotgot ();
		return -1;
	}
	
	if (strftime (out, outsize, "%H/%M/%S/%d/%m/%Y", tmp) == 0)
	{
		errors_timenotgot ();
		return -2;
	}
	
	return 0;
}

/*
	Function: time_get_tm_str (struct tm* out, char* datestr);
	Description: Converts the baskdate string to an tm struct.
	InitVersion: 0.0.1
*/
short time_get_tm_str (struct tm* out, char* datestr)
{
	if (strlen (datestr) != F_BB_S_DATE-1)
	{
		return -1;
	}
	
	memset (out, 0, sizeof (struct tm));
	
	if (strptime (datestr, "%H/%M/%S/%d/%m/%Y", out) == 0)
	{
		errors_timenotgot ();
		return -2;
	}
	
	return 0;
}

/*
	Function: time_get_hours (char* datestr);
	Description: Returns the hours from the datestr.
	InitVersion: 0.0.1
*/
int time_get_hours (char* datestr)
{
	char hours[3];
	
	if (strlen (datestr) != F_BB_S_DATE-1)
	{
		return -1;
	}
	
	hours[0] = datestr[0];
	hours[1] = datestr[1];
	hours[2] = '\0';
	
	return atoi (hours);
}

/*
	Function: time_get_minutes (char* datestr);
	Description: Returns the minutes from the datestr.
	InitVersion: 0.0.1
*/
int time_get_minutes (char* datestr)
{
	char minutes[3];
	
	if (strlen (datestr) != F_BB_S_DATE-1)
	{
		return -1;
	}
	
	minutes[0] = datestr[3];
	minutes[1] = datestr[4];
	minutes[2] = '\0';
	
	return atoi (minutes);
}

/*
	Function: time_get_seconds (char* datestr);
	Description: Returns the seconds from the datestr.
	InitVersion: 0.0.1
*/
int time_get_seconds (char* datestr)
{
	char seconds[3];
	
	if (strlen (datestr) != F_BB_S_DATE-1)
	{
		return -1;
	}
	
	seconds[0] = datestr[6];
	seconds[1] = datestr[7];
	seconds[2] = '\0';
	
	return atoi (seconds);
}

/*
	Function: time_get_day (char* datestr);
	Description: Returns the day from the datestr.
	InitVersion: 0.0.1
*/
int time_get_day (char* datestr)
{
	char day[3];
	
	if (strlen (datestr) != F_BB_S_DATE-1)
	{
		return -1;
	}
	
	day[0] = datestr[9];
	day[1] = datestr[10];
	day[2] = '\0';
	
	return atoi (day);
}

/*
	Function: time_get_month (char* datestr);
	Description: Returns the month from the datestr.
	InitVersion: 0.0.1
*/
int time_get_month (char* datestr)
{
	char month[3];
	
	if (strlen (datestr) != F_BB_S_DATE-1)
	{
		return -1;
	}
	
	month[0] = datestr[12];
	month[1] = datestr[13];
	month[2] = '\0';
	
	return atoi (month);
}

/*
	Function: time_get_year (char* datestr);
	Description: Returns the year from the datestr.
	InitVersion: 0.0.1
*/
int time_get_year (char* datestr)
{
	char year[5];
	
	if (strlen (datestr) != F_BB_S_DATE-1)
	{
		return -1;
	}
	
	year[0] = datestr[15];
	year[1] = datestr[16];
	year[2] = datestr[17];
	year[3] = datestr[18];
	year[4] = '\0';
	
	return atoi (year);
}
