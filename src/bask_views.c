/* Copyright 2015 - 2016 Marc Volker Dickmann */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "../lib/dutils.h"
#include "bask_core.h"
#include "bask_time.h"
#include "bask_task.h"
#include "bask_project.h"
#include "bask_filter.h"
#include "bask_ui.h"
#include "bask_priority.h"
#include "bask_views.h"

/* |--------------------------------------------|
   |			Legend			|
   |--------------------------------------------| */

/*
	Function: view_print_legend_tag (bask_core* tcore, char* tagname, char* tagcolor);
	Description: Prints a single tag labeld tagname with txt color tagcolor.
	InitVersion: 0.0.1
*/
void view_print_legend_tag (bask_core* tcore, char* tagname, char* tagcolor)
{
	ui_misc_print_color (tcore, tagcolor);
	printf ("%s, ", tagname);
	ui_misc_print_color (tcore, BC_TXT_RST);
}

/*
	Function: view_legend (bask_core* tcore, bask_theme* btheme, bask_priority** bprioritys);
	Description: Displays a legend for the meanings of colors.
	InitVersion: 0.0.1
*/
void view_legend (bask_core* tcore, bask_theme* btheme, bask_priority** bprioritys)
{
	bask_priority* ptr = *bprioritys;
	
	printf ("\n\nLegend: ");
	
	while (ptr != NULL)
	{
		view_print_legend_tag (tcore, ptr->p_name, ptr->p_color);
		
		ptr = ptr->next;
	}
	
	view_print_legend_tag (tcore, "Finished", btheme->color_finished);
	printf ("\n");
}

/* |--------------------------------------------|
   |		    ViewSingle			|
   |--------------------------------------------| */

/*
	Function: view_print_single (bask_core* tcore, struct bask_task* task);
	Description: Prints informations about a single task.
	InitVersion: 0.0.1
*/
void view_print_single (bask_core* tcore, struct bask_task* task)
{
	unsigned int i, j;
	
	if (task != NULL)
	{
		printf ("\nTask:\n");
		printf ("\tID:\t\t%i\n", task->t_id);
		printf ("\tActive:\t\t%i\n", BITGET (task->t_flags, TASK_FLAG_ACTIVE));
		printf ("\tState:\t\t%i\n", BITGET (task->t_flags, TASK_FLAG_FINISHED));
		printf ("\tPriority:\t%hi\n", task->t_priority);
		printf ("\tAdded:\t\t%s\n", task->t_added);
		printf ("\tDue:\t\t%s\n", task->t_due);
		printf ("\tFinished:\t%s\n", task->t_finished);
		printf ("\tProject:\t%s\n", task->t_project);
		
		/* Prefix length: strlen ("Description:")+8; Suffix Length: 2*8; */
		ui_tbl_print_field_str ("Description:", 20, 16);
		ui_misc_print_linebreak (tcore, task->t_description, 24, tcore->t_descriptionmin);
		printf ("\n");
	}
}

/*
	Function: view_single (bask_core* tcore, struct bask_task** first, bask_filter* bfilter, unsigned int id, short multi);
	Description: Displays a single task.
	InitVersion: 0.0.1
*/
void view_single (bask_core* tcore, struct bask_task** first, bask_filter* bfilter, unsigned int id, short multi)
{
	struct bask_task* ptr = *first;
	
	while (ptr != NULL)
	{
		if (multi == TRUE &&
		   filter_check_task (bfilter, ptr) == TRUE)
		{
			view_print_single (tcore, ptr);
		}
		else if (ptr->t_id == id)
		{
			view_print_single (tcore, ptr);
			break;
		}
		
		ptr = ptr->next;
	}
}

/* |--------------------------------------------|
   |		    ViewSummary			|
   |--------------------------------------------| */

/*
	Function: view_summary (bask_core* tcore, bask_theme* btheme, struct bask_task** first, bask_filter* bfilter);
	Description: Displays the summary of the projects!
	InitVersion: 0.0.1
*/
void view_summary (bask_core* tcore, bask_theme* btheme, struct bask_task** first, bask_filter* bfilter)
{
	int i, ppercent, premaining, pprojectmax;
	struct bask_project* tprojects = NULL, *pptr;
	struct bask_task* ptr = *first;
	
	i = ppercent = premaining  = 0;
	pprojectmax = tcore->t_projectmin;
	
	while (ptr != NULL)
	{
		if (filter_check_task (bfilter, ptr) == TRUE)
		{
			project_insert (&tprojects, TRUE, BITGET (ptr->t_flags, TASK_FLAG_FINISHED), ptr->t_project);
		
			i = strlen (ptr->t_project);
		
			if (i > pprojectmax)
			{
				pprojectmax = i;
			}
		}
		
		ptr = ptr->next;
	}
	
	ui_tbl_print_title (tcore, "Project", pprojectmax, 1);
	ui_tbl_print_title (tcore, "Remaining", -1, 1);
	ui_tbl_print_title (tcore, "Complete", -1, 1);
	printf ("0%%               100%%\n");
	
	/* Print the underline if no color is used. */
	if (BITGET (tcore->t_options, T_O_COLOR) == FALSE)
	{
		ui_tbl_print_title_underline (pprojectmax, 1);
		ui_tbl_print_title_underline (9, 1);
		ui_tbl_print_title_underline (8, 1);
		printf ("\n");
	}
	
	i = 0;
	pptr = tprojects;
	while (pptr != NULL)
	{
		if (pptr->p_active == TRUE)
		{
			if (pptr->p_complete == 0.0)
			{
				ppercent = 0;
			}
			else
			{
				ppercent = 100*(pptr->p_complete / pptr->p_tasks);
			}
			
			premaining = (int)pptr->p_tasks - (int)pptr->p_complete;
			
			ui_tbl_print_field_str (pptr->p_name, -1, pprojectmax+1);
			ui_tbl_print_field_int (premaining, 8-GETDIGITS (premaining), -1);
			ui_tbl_print_field_int (ppercent, 7-GETDIGITS (ppercent), -1);
			ui_tbl_print_field_str ("%", -1, 2);
			
			ui_misc_print_progress (tcore, 100.0*(pptr->p_complete/pptr->p_tasks), btheme->color_pbarbak);
			
			i++;
		}
		
		pptr = pptr->next;
	}
	
	if (i == 1)
	{
		printf ("\n%i project\n", i);
	}
	else
	{
		printf ("\n%i projects\n", i);
	}
	
	project_free_ll (&tprojects);
}

/* |--------------------------------------------|
   |		   ViewTasklist			|
   |--------------------------------------------| */

/*
	Function: view_tasklist (bask_core* tcore, bask_theme* btheme, bask_priority** bprioritys, struct bask_task** first, bask_filter* filter);
	Description: Displays all tasks in a table!
	InitVersion: 0.0.1
*/
void view_tasklist (bask_core* tcore, bask_theme* btheme, bask_priority** bprioritys, struct bask_task** first, bask_filter* filter)
{
	int i, j, x, tprojectmax, idmax, tdescriptionmax;
	char prefix[UI_S_THEMECOLOR*2], pri[PRI_S_PALIAS];
	struct bask_task* ptr = *first;
	
	i = j = x = idmax = 0;
	tprojectmax = tcore->t_projectmin;
	tdescriptionmax = tcore->t_descriptionmin;
	
	while (ptr != NULL)
	{
		if (filter_check_task (filter, ptr) == TRUE)
		{
			if (idmax < ptr->t_id)
			{
				idmax = ptr->t_id;
			}
			
			i = strlen (ptr->t_project);
		
			if (i > tprojectmax)
			{
				tprojectmax = i;
			}
			
			i = strlen (ptr->t_description);
			
			if (i > tdescriptionmax && BITGET (tcore->t_options, T_O_DESCRIPTIONBREAK) == FALSE)
			{
				tdescriptionmax = i;
			}
		}
		
		ptr = ptr->next;
	}
	
	ui_tbl_print_title (tcore, "ID", -1, GETDIGITS (idmax)+1);
	ui_tbl_print_title (tcore, "Project", tprojectmax, 1);
	ui_tbl_print_title (tcore, "Pri", 3, 1);
	ui_tbl_print_title (tcore, "Description", tdescriptionmax, 1);
	printf ("\n");
	
	/* Print the underline if no color is used. */
	if (BITGET (tcore->t_options, T_O_COLOR) == FALSE)
	{
		ui_tbl_print_title_underline (2, GETDIGITS (idmax)+1);
		ui_tbl_print_title_underline (tprojectmax, 1);
		ui_tbl_print_title_underline (3, 1);
		ui_tbl_print_title_underline (tdescriptionmax, 1);
		printf ("\n");
	}
	
	i = 0;
	ptr = *first;
	while (ptr != NULL)
	{
		if (filter_check_task (filter, ptr) == TRUE)
		{	
			priority_get_viewdata (bprioritys, ptr->t_priority, prefix, sizeof (prefix), pri, sizeof (pri));
			
			/* NOTE: This saves reduces the amount of instructions called. */
			if (BITGET (tcore->t_options, T_O_COLOR) == TRUE)
			{
				if (BITGET (ptr->t_flags, TASK_FLAG_FINISHED) == TRUE)
				{
					strcpy (prefix, btheme->color_finished);
				}
		
				if (i%2 == 1)
				{	
					strcat (prefix, btheme->color_seclinesbak);
				}
		
				printf ("%s", prefix);
			}
			
			ui_tbl_print_field_int (ptr->t_id, GETDIGITS (idmax) - GETDIGITS (ptr->t_id), 2);
			ui_tbl_print_field_str (ptr->t_project, -1, tprojectmax+1);
			ui_tbl_print_field_str (pri, -1, 4);
			ui_misc_print_linebreak (tcore, ptr->t_description, GETDIGITS (idmax) + tprojectmax + 8, tdescriptionmax);
			ui_misc_print_color (tcore, BC_TXT_RST);
			printf ("\n");
			
			i++;
		}
		
		ptr = ptr->next;
	}
	
	if (i == 1)
	{
		printf ("\n%i task\n", i);
	}
	else
	{
		printf ("\n%i tasks\n", i);
	}
	view_legend (tcore, btheme, bprioritys);
}

/* |--------------------------------------------|
   |		    ViewHistory			|
   |--------------------------------------------| */
   
/*
	Function: view_history (bask_core* tcore, bask_theme* btheme, struct bask_task** first, bask_filter* filter);
	Description: Displays a history of monthly stats.
	InitVersion: 0.0.1
*/
void view_history (bask_core* tcore, bask_theme* btheme, struct bask_task** first, bask_filter* filter)
{
	unsigned int i, tasks_added, tasks_finished, current_year, max_year, year_added, year_finished;
	unsigned short current_month, max_month, month_added, month_finished;
	struct bask_task* ptr = *first;
	
	i = tasks_added = tasks_finished = current_year = current_month = year_added = month_added = year_finished = month_finished = 0;
	max_year = max_month = 1;
	
	while (current_year <= max_year || current_month < max_month)
	{
		while (ptr != NULL)
		{
			if (strlen (ptr->t_added) == F_BB_S_DATE-1 &&
			    filter_check_task (filter, ptr) == TRUE)
			{
				year_added = time_get_year (ptr->t_added);
				month_added = time_get_month (ptr->t_added);
				
				if (i == 0)
				{
					/* NOTE: If the tasks hasnt finished (equals no finished date) this return -1 which will result in the max value of the unsigned int variable. This fixes that problem. */
					year_finished = (time_get_year (ptr->t_finished) == -1) ? year_added : time_get_year (ptr->t_finished);
					month_finished = (time_get_month (ptr->t_finished) == -1) ? month_added : time_get_month (ptr->t_finished);
				
					if (BIGGEST (year_added, year_finished) > max_year)
					{
						max_year = BIGGEST (year_added, year_finished);
					}
					
					if (SMALLEST (year_added, year_finished) < current_year ||
					    current_year == 0)
					{
						current_year = SMALLEST (year_added, year_finished);
					}
					
					if (BIGGEST (month_added, month_finished) > max_month)
					{
						max_month = BIGGEST (month_added, month_finished);
					}
					
					if (SMALLEST (month_added, month_finished) < current_month ||
					    current_month == 0)
					{ 
						current_month = SMALLEST (month_added, month_finished);
					}
				}
				else
				{
					/* NOTE: If the tasks hasnt finished (equals no finished date) this return -1 which will result in the max value of the unsigned int variable. This fixes that problem. */
					year_finished = (time_get_year (ptr->t_finished) == -1) ? 0 : time_get_year (ptr->t_finished);
					month_finished = (time_get_month (ptr->t_finished) == -1) ? 0 : time_get_month (ptr->t_finished);
					
					/* ADDED TASKS */
					if (year_added == current_year &&
					    month_added == current_month)
					{
						tasks_added++;
					}
				
					/* FINISHED TASKS */
					if (year_finished == current_year &&
					    month_finished == current_month)
					{
						tasks_finished++;
					}
				}
			}
		
			ptr = ptr->next;
		}
		
		if (tasks_added > 0 || tasks_finished > 0)
		{
			ui_misc_print_color (tcore, BC_BLD_BLUE);
			ui_tbl_print_field_int (current_year, -1, -1);
			ui_tbl_print_field_str ("/", -1, -1);
			ui_tbl_print_field_int (current_month, -1, (max_year+1+max_month+10)-(current_year+1+current_month));
			
			ui_misc_print_color (tcore, BC_BLD_GREEN);
			printf ("%i", tasks_added);
			ui_misc_print_color (tcore, BC_TXT_RST);
				
			if (tasks_added != 1)
			{
				printf (" tasks added, ");
			}
			else
			{
				printf (" task added, ");
			}
			
			ui_misc_print_color (tcore, btheme->color_finished);
			printf ("%i", tasks_finished);
			ui_misc_print_color (tcore, BC_TXT_RST);
			
			if (tasks_finished != 1)
			{
				printf (" tasks finished.\n");
			}
			else
			{
				printf (" task finished.\n");
			}
		}
		
		tasks_added = 0;
		tasks_finished = 0;
		
		if (i != 0 && current_month == 12)
		{
			current_year++;
			current_month = 1;
		}
		else if (i != 0)
		{
			current_month++;
		}
		
		i++;
		ptr = *first;
	}
}
