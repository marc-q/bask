/* Copyright 2015 Marc Volker Dickmann */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../lib/dutils.h"
#include "bask_core.h"
#include "bask_task.h"
#include "bask_project.h"
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
	Function: view_print_single (struct bask_task* task);
	Description: Prints informations about a single task.
	InitVersion: 0.0.1
*/
void view_print_single (struct bask_task* task)
{
	if (task != NULL)
	{
		printf ("\nTask:\n");
		printf ("\tID:\t\t%i\n", task->t_id);
		printf ("\tActive:\t\t%i\n", task->t_active);
		printf ("\tState:\t\t%i\n", task->t_state);
		printf ("\tPriority:\t%i\n", task->t_priority);
		printf ("\tAdded:\t\t%s\n", task->t_added);
		printf ("\tFinished:\t%s\n", task->t_finished);
		printf ("\tProject:\t%s\n", task->t_project);
		printf ("\tDescription:\t%s\n", task->t_description);
	}
}

/*
	Function: view_single (bask_core* tcore, struct bask_task** first, int id);
	Description: Displays a single task.
	InitVersion: 0.0.1
*/
void view_single (bask_core* tcore, struct bask_task** first, int id)
{
	struct bask_task* ptr = *first;
	
	while (ptr != NULL)
	{
		if (ptr->t_id == id)
		{
			view_print_single (ptr);
			break;
		}
		
		ptr = ptr->next;
	}
}

/* |--------------------------------------------|
   |		    ViewSummary			|
   |--------------------------------------------| */

/*
	Function: view_summary (bask_core* tcore, bask_theme* btheme, struct bask_task** first);
	Description: Displays the summary of the projects!
	InitVersion: 0.0.1
*/
void view_summary (bask_core* tcore, bask_theme* btheme, struct bask_task** first)
{
	int i, ppercent, premaining, pprojectmax;
	struct bask_project* tprojects = NULL, *pptr;
	struct bask_task* ptr = *first;
	
	i = ppercent = premaining  = 0;
	pprojectmax = tcore->t_projectmin;
	
	while (ptr != NULL)
	{
		project_insert (&tprojects, 1, ptr->t_state, ptr->t_project);
		
		i = strlen (ptr->t_project);
		
		if (i > pprojectmax)
		{
			pprojectmax = i;
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
	Function: view_tasklist (bask_core* tcore, bask_theme* btheme, struct bask_task** first);
	Description: Displays all tasks in a table!
	InitVersion: 0.0.1
*/
void view_tasklist (bask_core* tcore, bask_theme* btheme, struct bask_task** first)
{
	int i, j, x, tprojectmax, tdescriptionmax;
	char prefix[22], pri[4];
	struct bask_task* ptr = *first;
	
	i = j = x = 0;
	tprojectmax = tcore->t_projectmin;
	tdescriptionmax = tcore->t_descriptionmin;
	
	while (ptr != NULL)
	{
		if (ptr->t_active != 0)
		{
			
			i = strlen (ptr->t_project);
		
			if (i > tprojectmax)
			{
				tprojectmax = i;
			}
			
			i = strlen (ptr->t_description);
			
			if (i > tdescriptionmax && tcore->t_descriptionbreak == 0)
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
		if (ptr->t_active != 0)
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
		
			if (ptr->t_state == 1)
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
			
			if (strlen (ptr->t_description) > tdescriptionmax && tcore->t_descriptionbreak == 1)
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
