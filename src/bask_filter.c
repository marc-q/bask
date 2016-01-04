/* Copyright 2015 - 2016 Marc Volker Dickmann */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "../lib/dutils.h"
#include "bask_core.h"
#include "bask_time.h"
#include "bask_task.h"
#include "bask_filter.h"

/*
	Function: filter_init (bask_filter* filter, short tactive, short tfinished, short tpriority, short tday, short tmonth, short tyear);
	Description: Inits a filter structure with values.
	InitVersion: 0.0.1
*/
void filter_init (bask_filter* filter, short tactive, short tfinished, short tpriority, short tday, short tmonth, short tyear)
{
	filter->flags = 0;
	filter->tday = 0;
	filter->tmonth = 0;
	filter->tyear = 0;
	filter->task.t_flags = 0;
	filter->task.t_priority = 0;
	filter->task.next = NULL;
	
	if (tactive != -1)
	{
		filter->flags ^= BITCOPY (FLTR_ON, 0, filter->flags, T_FLTR_ACTIVE);
		filter->task.t_flags ^= BITCOPY (tactive, 0, filter->task.t_flags, TASK_FLAG_ACTIVE);
	}
	
	if (tfinished != -1)
	{
		filter->flags ^= BITCOPY (FLTR_ON, 0, filter->flags, T_FLTR_FINISHED);
		filter->task.t_flags ^= BITCOPY (tfinished, 0, filter->task.t_flags, TASK_FLAG_FINISHED);
	}
	
	if (tpriority != -1)
	{
		filter->flags ^= BITCOPY (FLTR_ON, 0, filter->flags, T_FLTR_PRIORITY);
		filter->task.t_priority = tpriority;
	}
	
	if (tday != -1)
	{
		filter->flags ^= BITCOPY (FLTR_ON, 0, filter->flags, T_FLTR_DAY);
		filter->tday = tday;
	}
	
	if (tmonth != -1)
	{
		filter->flags ^= BITCOPY (FLTR_ON, 0, filter->flags, T_FLTR_MONTH);
		filter->tmonth = tmonth;
	}
	
	if (tyear != -1)
	{
		filter->flags ^= BITCOPY (FLTR_ON, 0, filter->flags, T_FLTR_YEAR);
		filter->tyear = tyear;
	}
}

/*
	Function: filter_check_task (bask_filter* filter, struct bask_task* task);
	Description: Checks if a task matches the filter.
	InitVersion: 0.0.1
*/
int filter_check_task (bask_filter* filter, struct bask_task* task)
{	
	if (( (BITGET (filter->flags, T_FLTR_ACTIVE) == FLTR_ON &&
	       BITGET (task->t_flags, TASK_FLAG_ACTIVE) == BITGET (filter->task.t_flags, TASK_FLAG_ACTIVE)) ||
	    BITGET (filter->flags, T_FLTR_ACTIVE) == FLTR_OFF) &&
	    
	    ( (BITGET (filter->flags, T_FLTR_FINISHED) == FLTR_ON &&
	       BITGET (task->t_flags, TASK_FLAG_FINISHED) == BITGET (filter->task.t_flags, TASK_FLAG_FINISHED)) ||
	    BITGET (filter->flags, T_FLTR_FINISHED) == FLTR_OFF) &&
	    
	    ( (BITGET (filter->flags, T_FLTR_PRIORITY) == FLTR_ON &&
	       task->t_priority == filter->task.t_priority) ||
	    BITGET (filter->flags, T_FLTR_PRIORITY) == FLTR_OFF) &&
	    
	    ( (BITGET (filter->flags, T_FLTR_DAY) == FLTR_ON &&
	       time_get_day (task->t_added) == filter->tday) ||
	    BITGET (filter->flags, T_FLTR_DAY) == FLTR_OFF) &&
	    
	    ( (BITGET (filter->flags, T_FLTR_MONTH) == FLTR_ON &&
	       time_get_month (task->t_added) == filter->tmonth) ||
	    BITGET (filter->flags, T_FLTR_MONTH) == FLTR_OFF) &&
	    
	    ( (BITGET (filter->flags, T_FLTR_YEAR) == FLTR_ON &&
	       time_get_year (task->t_added) == filter->tyear) ||
	    BITGET (filter->flags, T_FLTR_YEAR) == FLTR_OFF))
	{
		return 1;
	}
	
	return 0;
}
