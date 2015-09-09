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

/*
	Function: view_print_legend_tag (char* tagname, char* tagcolor, char* txtcolor);
	Description: Prints a single tag labeld tagname with bg color tagcolor and txt color txtcolor.
	InitVersion: 0.0.1
*/
void view_print_legend_tag (char* tagname, char* tagcolor, char* txtcolor)
{
	printf ("%s \b%s \b%s \b%s, ", txtcolor, tagcolor, tagname, BC_TXT_RST);
}

/*
	Function: view_legend (void);
	Description: Displays a legend for the meanings of colors.
	InitVersion: 0.0.1
*/
void view_legend (void)
{
	printf ("\n\nLegend: Normal, ");
	view_print_legend_tag ("Critical", BC_BAK_RED, BC_BLD_WHITE);
	view_print_legend_tag ("Today", BC_BAK_BLUE, BC_BLD_WHITE);
	view_print_legend_tag ("Important", BC_BAK_GREEN, BC_TXT_BLACK);
	view_print_legend_tag ("Finished", BC_BAK_BLACK, BC_TXT_WHITE);
	printf ("\n");
}

/*
	Function: view_single (bask_core* tcore, struct bask_task* task);
	Description: Displays a single task.
	InitVersion: 0.0.1
*/
void view_single (bask_core* tcore, struct bask_task* task)
{
	printf ("\nTask ID: %i\n", task->t_id);
	printf ("Task project: %s\n", task->t_project);
	printf ("Task description: %s\n", task->t_description);
}

/*
	Function: view_summary (bask_core* tcore, struct bask_task** first);
	Description: Displays the summary of the projects!
	InitVersion: 0.0.1
*/
void view_summary (bask_core* tcore, struct bask_task** first)
{
	int ppercent, premaining, pid;
	struct bask_project* tprojects = NULL, *pptr;
	struct bask_task* ptr = *first;
	
	printf ("\n%s", BC_UND_WHITE);
	view_tbl_print_title ("Project", 15, 1);
	view_tbl_print_title ("Remaining", 0, 1);
	view_tbl_print_title ("Complete", 0, 1);
	printf ("%s0%%               100%%\n", BC_TXT_RST);
	
	while (ptr != NULL)
	{
		project_insert (&tprojects, 1, ptr->t_id, ptr->t_state, ptr->t_project);
		
		ptr = ptr->next;
	}
	
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
			
			view_tbl_print_field_str (pptr->p_name, -1, 16);
			view_tbl_print_field_int (premaining, 8-GETDIGITS (premaining), -1);
			view_tbl_print_field_int (ppercent, 7-GETDIGITS (ppercent), -1);
			view_tbl_print_field_str ("%", -1, 2);
			
			view_misc_print_progress (100.0*(pptr->p_complete/pptr->p_tasks), BC_BAK_GREEN);
		}
		
		pptr = pptr->next;
	}
	
	project_free_ll (&tprojects);
}

/*
	Function: view_tasklist (bask_core* tcore, struct bask_task** first);
	Description: Displays all tasks in a table!
	InitVersion: 0.0.1
*/
void view_tasklist (bask_core* tcore, struct bask_task** first)
{
	int i = 0;
	char prefix[30], pri[4];
	struct bask_task* ptr = *first;
	
	printf ("\n%s", BC_UND_WHITE);
	view_tbl_print_title ("ID", -1, GETDIGITS (tcore->tc_amount)+1);
	view_tbl_print_title ("Project", 15, 1);
	view_tbl_print_title ("Pri", 3, 1);
	view_tbl_print_title ("Description", 50, 1);
	printf ("%s\n", BC_TXT_RST);
	
	while (ptr != NULL)
	{
		if (ptr->t_active != 0)
		{
			switch (ptr->t_priority)
			{
				case 0:
					strcpy (prefix, BC_BLD_WHITE);
					strcpy (pri, "L");
					break;
				case 1:
					strcpy (prefix, BC_BLD_GREEN);
					strcpy (pri, "I");
					break;
				case 2:
					strcpy (prefix, BC_BLD_BLUE);
					strcpy (pri, "T");
					break;
				case 3:
					strcpy (prefix, BC_BLD_RED);
					strcpy (pri, "C");
					break;
				default:
					strcpy (prefix, BC_TXT_WHITE);
					strcpy (pri, "N");
					break;
			}
		
			if (ptr->t_state == 1)
			{
				strcpy (prefix, BC_BLD_BLACK);
			}
		
			if (i%2 == 1)
			{	
				strcat (prefix, T_COLOR_LINEBAK);
			
			}
		
			printf ("%s", prefix);
			view_tbl_print_field_int (ptr->t_id, GETDIGITS (tcore->tc_amount) - GETDIGITS (ptr->t_id), DENULL (GETDIGITS (tcore->tc_amount))+1);
			view_tbl_print_field_str (ptr->t_project, 1, 16);
			view_tbl_print_field_str (pri, -1, 4);
			view_tbl_print_field_str (ptr->t_description, -1, 50);
			printf ("%s\n", BC_TXT_RST);
			
			i++;
		}
		
		ptr = ptr->next;
	}
	
	printf ("\n%i tasks\n", i);
	view_legend ();
}
