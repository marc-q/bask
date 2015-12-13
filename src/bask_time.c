/* Copyright 2015 Marc Volker Dickmann */
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
	Function: time_get_str (char* out, size_t outsize);
	Description: Builds a string with the current date.
	InitVersion: 0.0.1
*/
int time_get_str (char* out, size_t outsize)
{
	time_t t;
	struct tm *tmp;
	
	t = time (NULL);
	tmp = localtime (&t);
	
	if (tmp == NULL)
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
int time_get_tm_str (struct tm* out, char* datestr)
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
