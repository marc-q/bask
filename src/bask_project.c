/* Copyright 2015 - 2017 Marc Volker Dickmann */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/dutils.h"
#include "bask_core.h"
#include "bask_project.h"

/*
	Function: project_free_ll_object (struct bask_project* object);
	Description: Free's a element from a bask_project linked list.
	InitVersion: 0.0.1
*/
static void project_free_ll_object (struct bask_project* object)
{	
	if (object != NULL)
	{
		free (object->p_name);
		free (object);
	}
}

/*
	Function: project_free_ll (struct bask_project** first);
	Description: Free's all elements from a bask_project linked list.
	InitVersion: 0.0.1
*/
void project_free_ll (struct bask_project** first)
{
	struct bask_project* ptr = *first, *preptr;
	
	while (ptr != NULL)
	{
		preptr = ptr;
		ptr = ptr->next;
		project_free_ll_object (preptr);
	}
}

/*
	Function: project_insert (struct bask_project** first, short t_active, short t_state, char* tproject);
	Description: Inserts a new project with data.
	InitVersion: 0.0.1
*/
short project_insert (struct bask_project** first, short t_active, short t_state, char* t_project)
{
	struct bask_project *newobj = malloc (sizeof (struct bask_project)), *preobj;
	
	if (newobj == NULL)
	{
		return -1;
	}
	
	newobj->p_active = t_active;
	newobj->next = NULL;
	
	if (strlen (t_project) >= T_S_PROJECT)
	{
		free (newobj);
		return -2;
	}

	utils_mkstr (strlen (t_project), &newobj->p_name);
	strcpy (newobj->p_name, t_project);
	
	if (*first == NULL)
	{
		if (t_state == TRUE)
		{
			newobj->p_complete = 1.0;
		}
		else
		{
			newobj->p_complete = 0.0;
		}
		
		newobj->p_tasks = 1.0;
		
		*first = newobj;
	}
	else
	{
		preobj = *first;
		
		while (TRUE)
		{
			if (utils_streq (preobj->p_name, newobj->p_name) == 0)
			{
				if (t_state == TRUE)
				{
					preobj->p_complete++;
				}
				
				preobj->p_tasks++;
				project_free_ll_object (newobj);
				break;
			}
			else if (preobj->next == NULL)
			{
				if (t_state == TRUE)
				{
					newobj->p_complete = 1.0;
				}
				else
				{
					newobj->p_complete = 0.0;
				}
				
				newobj->p_tasks = 1.0;
				
				preobj->next = newobj;
				break;
			}
			
			preobj = preobj->next;
		}
	}
	
	return 0;
}
