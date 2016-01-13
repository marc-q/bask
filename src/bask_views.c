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
#include "bask_views.h"

/* |--------------------------------------------|
   |			Legend			|
   |--------------------------------------------| */

/*
	Function: view_print_legend_tag (char* tagname, char* tagcolor);
	Description: Prints a single tag labeld tagname with txt color tagcolor.
	InitVersion: 0.0.1
*/
void view_print_legend_tag (char* tagname, char* tagcolor)
{
	printf ("%s \b%s \b%s, ", tagcolor, tagname, BC_TXT_RST);
}

/*
	Function: view_legend (bask_theme* btheme);
	Description: Displays a legend for the meanings of colors.
	InitVersion: 0.0.1
*/
void view_legend (bask_theme* btheme)
{
	printf ("\n\nLegend: ");
	view_print_legend_tag ("Normal", btheme->color_normal);
	view_print_legend_tag ("Critical", btheme->color_critical);
	view_print_legend_tag ("Today", btheme->color_today);
	view_print_legend_tag ("Important", btheme->color_important);
	view_print_legend_tag ("Finished", btheme->color_finished);
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
		
		/* TODO: Put this into an functions! */
		if (strlen (task->t_description) > tcore->t_descriptionmin && BITGET (tcore->t_options, T_O_DESCRIPTIONBREAK) == 1)
		{
			printf ("\tDescription:\t");
			
			for (i = 0, j = 0; i < strlen (task->t_description); i++, j++)
			{
				if (j == tcore->t_descriptionmin)
				{
					/* NOTE: That can be improved but for now thats a acceptable solution. */
					printf ("\n\t");
					ui_print_nspaces (12);
					printf ("\t");
					j = 0;
				}
					
				printf ("%c", task->t_description[i]);
			}
			
			printf ("\n");
		}
		else
		{
			printf ("\tDescription:\t%s\n", task->t_description);
		}
	}
}

/*
	Function: view_single (bask_core* tcore, struct bask_task** first, unsigned int id);
	Description: Displays a single task.
	InitVersion: 0.0.1
*/
void view_single (bask_core* tcore, struct bask_task** first, unsigned int id)
{
	struct bask_task* ptr = *first;
	
	while (ptr != NULL)
	{
		if (ptr->t_id == id)
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
		if (filter_check_task (bfilter, ptr) == 1)
		{
			project_insert (&tprojects, 1, BITGET (ptr->t_flags, TASK_FLAG_FINISHED), ptr->t_project);
		
			i = strlen (ptr->t_project);
		
			if (i > pprojectmax)
			{
				pprojectmax = i;
			}
		}
		
		ptr = ptr->next;
	}
	
	ui_tbl_print_title ("Project", pprojectmax, 1);
	ui_tbl_print_title ("Remaining", -1, 1);
	ui_tbl_print_title ("Complete", -1, 1);
	printf ("0%%               100%%\n");
	
	i = 0;
	pptr = tprojects;
	while (pptr != NULL)
	{
		if (pptr->p_active == 1)
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
			
			ui_misc_print_progress (100.0*(pptr->p_complete/pptr->p_tasks), btheme->color_pbarbak);
			
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
	Function: view_tasklist (bask_core* tcore, bask_theme* btheme, struct bask_task** first, bask_filter* filter);
	Description: Displays all tasks in a table!
	InitVersion: 0.0.1
*/
void view_tasklist (bask_core* tcore, bask_theme* btheme, struct bask_task** first, bask_filter* filter)
{
	int i, j, x, tprojectmax, tdescriptionmax;
	char prefix[22], pri[4];
	struct bask_task* ptr = *first;
	
	i = j = x = 0;
	tprojectmax = tcore->t_projectmin;
	tdescriptionmax = tcore->t_descriptionmin;
	
	while (ptr != NULL)
	{
		if (filter_check_task (filter, ptr) == 1)
		{
			i = strlen (ptr->t_project);
		
			if (i > tprojectmax)
			{
				tprojectmax = i;
			}
			
			i = strlen (ptr->t_description);
			
			if (i > tdescriptionmax && BITGET (tcore->t_options, T_O_DESCRIPTIONBREAK) == 0)
			{
				tdescriptionmax = i;
			}
		}
		
		ptr = ptr->next;
	}
	
	ui_tbl_print_title ("ID", -1, GETDIGITS (tcore->baskbin_uid)+1);
	ui_tbl_print_title ("Project", tprojectmax, 1);
	ui_tbl_print_title ("Pri", 3, 1);
	ui_tbl_print_title ("Description", tdescriptionmax, 1);
	printf ("\n");
	
	i = 0;
	ptr = *first;
	while (ptr != NULL)
	{
		if (filter_check_task (filter, ptr) == 1)
		{
			switch (ptr->t_priority)
			{
				case 0:
					strcpy (prefix, btheme->color_normal);
					strcpy (pri, "L");
					break;
				case 1:
					strcpy (prefix, btheme->color_important);
					strcpy (pri, "I");
					break;
				case 2:
					strcpy (prefix, btheme->color_today);
					strcpy (pri, "T");
					break;
				case 3:
					strcpy (prefix, btheme->color_critical);
					strcpy (pri, "C");
					break;
				default:
					strcpy (prefix, btheme->color_normal);
					strcpy (pri, "N");
					break;
			}
		
			if (BITGET (ptr->t_flags, TASK_FLAG_FINISHED) == 1)
			{
				strcpy (prefix, btheme->color_finished);
			}
		
			if (i%2 == 1)
			{	
				strcat (prefix, btheme->color_seclinesbak);
			}
		
			printf ("%s", prefix);
			ui_tbl_print_field_int (ptr->t_id, GETDIGITS (tcore->baskbin_uid) - GETDIGITS (ptr->t_id), 2);
			ui_tbl_print_field_str (ptr->t_project, -1, tprojectmax+1);
			ui_tbl_print_field_str (pri, -1, 4);
			
			/* TODO: Put this into an functions! */
			if (strlen (ptr->t_description) > tdescriptionmax && BITGET (tcore->t_options, T_O_DESCRIPTIONBREAK) == 1)
			{
				for (j = 0, x = 0; j < strlen (ptr->t_description); j++, x++)
				{
					if (x == tdescriptionmax)
					{
						printf ("\n");
						ui_print_nspaces (GETDIGITS (tcore->baskbin_uid) + tprojectmax + 8);
						x = 0;
					}
					
					printf ("%c", ptr->t_description[j]);
				}
			}
			else
			{
				ui_tbl_print_field_str (ptr->t_description, -1, tdescriptionmax);
			}
			
			printf ("%s\n", BC_TXT_RST);
			
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
	view_legend (btheme);
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
			    filter_check_task (filter, ptr) == 1)
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
			printf ("%s", BC_BLD_BLUE);
			ui_tbl_print_field_int (current_year, -1, -1);
			ui_tbl_print_field_str ("/", -1, -1);
			ui_tbl_print_field_int (current_month, -1, (max_year+1+max_month+10)-(current_year+1+current_month));
			
			if (tasks_added != 1)
			{
				printf ("%s%i%s tasks added, ", BC_BLD_GREEN, tasks_added, BC_TXT_RST);
			}
			else
			{
				printf ("%s%i%s task added, ", BC_BLD_GREEN, tasks_added, BC_TXT_RST);
			}
			
			if (tasks_finished != 1)
			{
				printf ("%s%i%s tasks finished.\n", btheme->color_finished, tasks_finished, BC_TXT_RST);
			}
			else
			{
				printf ("%s%i%s task finished.\n", btheme->color_finished, tasks_finished, BC_TXT_RST);
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
