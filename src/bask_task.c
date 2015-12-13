/* Copyright 2015 Marc Volker Dickmann */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bask_core.h"
#include "bask_errors.h"
#include "bask_time.h"
#include "bask_task.h"
#include "bask_export.h"

/* |--------------------------------------------|
   |			Utils			|
   |--------------------------------------------| */

/*
	Function: task_check_input (bask_core* tcore, char* added, char* finished, char* project, char* description, int printout);
	Description: Checks if the input is correct and displays the error messages if printout equals 1!
	InitVersion: 0.0.1
*/
int task_check_input (bask_core* tcore, char* added, char* finished, char* project, char* description, int printout)
{
	if (strlen (added) >= T_S_ADDED)
	{
		if (printout == 1)
		{
			errors_lengthtobig ("added");
		}
		return -1;
	}
	else if (strlen (finished) >= T_S_FINISHED)
	{
		if (printout == 1)
		{
			errors_lengthtobig ("finished");
		}
		return -2;
	}
	else if (strlen (project) >= T_S_PROJECT)
	{
		if (printout == 1)
		{
			errors_lengthtobig ("project");
		}
		return -3;
	}
	else if (strlen (description) >= T_S_DESCRIPTION ||
		 strlen (description) > tcore->t_descriptionmax)
	{
		if (printout == 1)
		{
			errors_lengthtobig ("description");
		}
		return -4;
	}
	
	return 0;
}

/* |--------------------------------------------|
   |			Core			|
   |--------------------------------------------| */

/*
	Function: task_free_ll (struct bask_task** first);
	Description: Free's all elements from a bask_task linked list.
	InitVersion: 0.0.1
*/
void task_free_ll (struct bask_task** first)
{
	struct bask_task* ptr = *first, *preptr;
	
	while (ptr != NULL)
	{
		preptr = ptr;
		ptr = ptr->next;
		free (preptr);
	}
}

/*
	Function: task_upgrade (struct bask_task* task);
	Description: Upgrades a task. (Internal use f.e. new versions upgrades.)
	InitVersion: 0.0.1
*/
static int task_upgrade (struct bask_task* task)
{
	return 0;
}

/*
	Function: task_insert (struct bask_task** first, int n, int tid, int tactive, int tpriority, int tstate, char* tadded, char* tfinished, char* tproject, char* tdescription);
	Description: Inserts a new task with data.
	InitVersion: 0.0.1
*/
int task_insert (struct bask_task** first, int n, int tid, int tactive, int tpriority, int tstate, char* tadded, char* tfinished, char* tproject, char* tdescription)
{
	struct bask_task *newobj = malloc (sizeof (struct bask_task)), *preobj;
	
	if (newobj == NULL)
	{
		return -1;
	}
	
	newobj->n = n;
	newobj->t_active = tactive;
	newobj->t_id = tid;
	newobj->t_priority = tpriority;
	newobj->t_state = tstate;
	newobj->next = NULL;
	
	if (strlen (tadded) >= T_S_ADDED || strlen (tfinished) >= T_S_FINISHED || strlen (tproject) >= T_S_PROJECT || strlen (tdescription) >= T_S_DESCRIPTION)
	{
		return -2;
	}

	strcpy (newobj->t_added, tadded);
	strcpy (newobj->t_finished, tfinished);
	strcpy (newobj->t_project, tproject);
	strcpy (newobj->t_description, tdescription);
	
	task_upgrade (newobj);
	
	if (*first == NULL)
	{
		*first = newobj;
	}
	else
	{
		preobj = *first;
		
		while (1)
		{
			if (preobj->n == newobj->n)
			{
				free (newobj);
				break;
			}
			else if (preobj->next == NULL)
			{
				preobj->next = newobj;
				break;
			}
			else if (preobj->next->n > newobj->n)
			{
				newobj->next = preobj->next;
				preobj->next = newobj;
				break;
			}
			
			preobj = preobj->next;
		}
	}
	
	return 0;
}

/*
	Function: task_remove (struct bask_task** first, int id);
	Description: Removes a task!
	InitVersion: 0.0.1
*/
int task_remove (struct bask_task** first, int id)
{
	struct bask_task* ptr = *first, *pre = NULL;
	
	if (ptr == NULL)
	{
		return -1;
	}
	
	while (ptr != NULL)
	{
		if (ptr->t_id == id)
		{
			if (ptr == *first)
			{
				*first = ptr->next;
				free (ptr);
			}
			else
			{
				pre->next = ptr->next;
				free (ptr);
			}
			
			break;
		}
		else
		{
			pre = ptr;
			ptr = ptr->next;
		}
	}
	
	return 0;
}

/*
	Function: task_modificate (struct bask_task** first, int id, int active, int state, int priority, char* added, char* finished, char* project, char* description);
	Description: Modificates a task!
	InitVersion: 0.0.1
*/
int task_modificate (struct bask_task** first, int id, int active, int state, int priority, char* added, char* finished, char* project, char* description)
{
	struct bask_task* ptr = *first;
	
	if (ptr == NULL)
	{
		return -1;
	}
	
	while (ptr != NULL)
	{
		if (ptr->t_id == id)
		{
			if (active >= 0)
			{
				ptr->t_active = active;
			}
			
			if (state >= 0)
			{
				ptr->t_state = state;
			}
			
			if (priority >= 0)
			{
				ptr->t_priority = priority;
			}
			
			if (added != NULL)
			{
				if (strlen (added) < T_S_ADDED && strcmp (added, "") != 0 )
				{
					strcpy (ptr->t_added, added);
				}
			}
			
			if (finished != NULL)
			{
				if (strlen (finished) < T_S_FINISHED && strcmp (finished, "") != 0 )
				{
					strcpy (ptr->t_finished, finished);
				}
			}
			
			if (project != NULL)
			{
				if (strlen (project) < T_S_PROJECT && strcmp (project, "") != 0 )
				{
					strcpy (ptr->t_project, project);
				}
			}
			
			if (description != NULL)
			{
				if (strlen (description) < T_S_DESCRIPTION && strcmp (description, "") != 0)
				{
					strcpy (ptr->t_description, description);
				}
			}
			break;
		}
		
		ptr = ptr->next;
	}
	
	return 0;
}

/* |--------------------------------------------|
   |			Base			|
   |--------------------------------------------| */

/*
	Function: task_create (bask_core* tcore, struct bask_task** first, int priority, char* project, char* description);
	Description: Creates a task!
	InitVersion: 0.0.1
*/
int task_create (bask_core* tcore, struct bask_task** first, int priority, char* project, char* description)
{	
	char added[T_S_ADDED];

	if (time_get_str (added, sizeof (added)) != 0)
	{
		return -1;
	}
	
	tcore->baskbin_uid++;
	
	task_insert (first, tcore->tc_amount, tcore->baskbin_uid, 1, priority, 0, added, " ", project, description);
	
	tcore->tc_amount++;

	return 0;
}

/*
	Function: task_deactivate (struct bask_task** first, int id);
	Description: Deactivates a task!
	InitVersion: 0.0.1
*/
void task_deactivate (struct bask_task** first, int id)
{	
	task_modificate (first, id, 0, -1, -1, "", "", "", "");
}

/*
	Function: task_finish (struct bask_task** first, int id);
	Description: Finished a task!
	InitVersion: 0.0.1
*/
int task_finish (struct bask_task** first, int id)
{
	char finished[T_S_FINISHED];

	if (time_get_str (finished, sizeof (finished)) != 0)
	{
		return -1;
	}
	
	task_modificate (first, id, -1, 1, -1, "", finished, "", "");
	
	return 0;
}

/* |--------------------------------------------|
   |			Search			|
   |--------------------------------------------| */
   
/*
	Function: task_search (bask_core* tcore, struct bask_task** first, struct bask_task** haystack, char* searchtag);
	Description: Build a tasklist haystack with bask whos description containing searchtag.
	InitVersion: 0.0.1
*/
int task_search (bask_core* tcore, struct bask_task** first, struct bask_task** haystack, char* searchtag)
{
	int i = 0;
	struct bask_task* ptr = *first;
	
	if (ptr == NULL)
	{
		return -1;
	}
	
	while (ptr != NULL)
	{
		if (strstr (ptr->t_added, searchtag) != NULL)
		{
			task_insert (haystack, i, ptr->t_id, ptr->t_active, ptr->t_priority, ptr->t_state, ptr->t_added, ptr->t_finished, ptr->t_project, ptr->t_description);
		}
		else if (strstr (ptr->t_finished, searchtag) != NULL)
		{
			task_insert (haystack, i, ptr->t_id, ptr->t_active, ptr->t_priority, ptr->t_state, ptr->t_added, ptr->t_finished, ptr->t_project, ptr->t_description);
		}
		else if (strstr (ptr->t_project, searchtag) != NULL)
		{
			task_insert (haystack, i, ptr->t_id, ptr->t_active, ptr->t_priority, ptr->t_state, ptr->t_added, ptr->t_finished, ptr->t_project, ptr->t_description);
		}
		else if (strstr (ptr->t_description, searchtag) != NULL)
		{
			task_insert (haystack, i, ptr->t_id, ptr->t_active, ptr->t_priority, ptr->t_state, ptr->t_added, ptr->t_finished, ptr->t_project, ptr->t_description);
		}
		
		i++;
		ptr = ptr->next;
	}
	
	return 0;
}

/* |--------------------------------------------|
   |			CMD's			|
   |--------------------------------------------| */

/*
	Function: task_create_cmd (bask_core* tcore, struct bask_task** first, int priority, char* project, char* description);
	Description: Cmd handle for creating a task!
	InitVersion: 0.0.1
*/
void task_create_cmd (bask_core* tcore, struct bask_task** first, int priority, char* project, char* description)
{
	if (task_check_input (tcore, "", "", project, description, 1) == 0)
	{
		task_create (tcore, first, priority, project, description);
		printf ("Created task %i.\n", tcore->baskbin_uid);
		export_baskbin (tcore, first, tcore->path_baskbin);
	}
}

/*
	Function: task_remove_cmd (bask_core* tcore, struct bask_task** first, int id);
	Description: Cmd handle for removing a task!
	InitVersion: 0.0.1
*/
void task_remove_cmd (bask_core* tcore, struct bask_task** first, int id)
{
	task_remove (first, id);
	printf ("Removed task %i.\n", id);
	export_baskbin (tcore, first, tcore->path_baskbin);
}

/*
	Function: task_modificate_cmd (bask_core* tcore, struct bask_task** first, int id, int active, int state, int priority, char* added, char* finished, char* project, char* description);
	Description: Cmd handle for modifying a task!
	InitVersion: 0.0.1
*/
void task_modificate_cmd (bask_core* tcore, struct bask_task** first, int id, int active, int state, int priority, char* added, char* finished, char* project, char* description)
{
	if (task_check_input (tcore, added, finished, project, description, 1) == 0)
	{
		task_modificate (first, id, active, state, priority, added, finished, project, description);
		printf ("Modificated task %i.\n", id);
		export_baskbin (tcore, first, tcore->path_baskbin);
	}
}

/*
	Function: task_deactivate_cmd (bask_core* tcore, struct bask_task** first, int id);
	Description: Cmd handle for deactivating a task!
	InitVersion: 0.0.1
*/
void task_deactivate_cmd (bask_core* tcore, struct bask_task** first, int id)
{	
	task_deactivate (first, id);
	printf ("Deactivated task %i.\n", id);
	export_baskbin (tcore, first, tcore->path_baskbin);
}

/*
	Function: task_finish_cmd (bask_core* tcore, struct bask_task** first, int id);
	Description: Cmd handle for finishing a task!
	InitVersion: 0.0.1
*/
void task_finish_cmd (bask_core* tcore, struct bask_task** first, int id)
{
	task_finish (first, id);
	printf ("Finished task %i.\n", id);
	export_baskbin (tcore, first, tcore->path_baskbin);
}
