/* Copyright 2015 - 2017 Marc Volker Dickmann */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "../lib/dutils.h"
#include "bask_core.h"
#include "bask_errors.h"
#include "bask_time.h"
#include "bask_task.h"
#include "bask_export.h"

/* |--------------------------------------------|
   |			Utils			|
   |--------------------------------------------| */

/*
	Function: task_check_input_nbrs (bask_core* tcore, int id, short priority, short active, short printout);
	Description: Checks if the input nbrs is correct and displays the error messages if printout equals 1!
	InitVersion: 0.0.1
*/

short task_check_input_nbrs (bask_core* tcore, int id, short priority, short active, short printout)
{
	if (id < 0)
	{
		if (printout == TRUE)
		{
			errors_notunsigned ("id");
		}
		return TASK_ERR_CHECK_ID;
	}
	else if (priority < tcore->priority_min || priority > tcore->priority_max)
	{
		if (printout == TRUE)
		{
			errors_outofrange_int ("priority", tcore->priority_min, tcore->priority_max);
		}
		return TASK_ERR_CHECK_PRIORITY;
	}
	else if (ISBOOL (active) != TRUE)
	{
		if (printout == TRUE)
		{
			errors_outofrange_int ("active", FALSE, TRUE);
		}
		return TASK_ERR_CHECK_ACTIVE;
	}
	
	return 0;
}

/*
	Function: task_check_input (bask_core* tcore, char* added, char* finished, char* project, char* description, short printout);
	Description: Checks if the input is correct and displays the error messages if printout equals 1!
	InitVersion: 0.0.1
*/
short task_check_input (bask_core* tcore, char* added, char* due, char* finished, char* project, char* description, short printout)
{
	if (strlen (added) >= T_S_ADDED)
	{
		if (printout == TRUE)
		{
			errors_lengthtobig ("added");
		}
		return TASK_ERR_CHECK_ADDED;
	}
	else if (strlen (due) >= T_S_DUE)
	{
		if (printout == TRUE)
		{
			errors_lengthtobig ("due");
		}
		return TASK_ERR_CHECK_DUE;
	}
	else if (strlen (finished) >= T_S_FINISHED)
	{
		if (printout == TRUE)
		{
			errors_lengthtobig ("finished");
		}
		return TASK_ERR_CHECK_FINISHED;
	}
	else if (strlen (project) >= T_S_PROJECT)
	{
		if (printout == TRUE)
		{
			errors_lengthtobig ("project");
		}
		return TASK_ERR_CHECK_PROJECT;
	}
	else if (strlen (description) >= T_S_DESCRIPTION ||
		 strlen (description) > tcore->t_descriptionmax && BITGET (tcore->t_options, T_O_DESCRIPTIONBREAK) == FALSE)
	{
		if (printout == TRUE)
		{
			errors_lengthtobig ("description");
		}
		return TASK_ERR_CHECK_DESCRIPTION;
	}
	
	return 0;
}

/* |--------------------------------------------|
   |			Core			|
   |--------------------------------------------| */

/*
	Function: task_free_ll_object (struct bask_task* object);
	Description: Free's a element from a bask_task linked list.
	InitVersion: 0.0.1
*/
static void task_free_ll_object (struct bask_task* object)
{	
	if (object != NULL)
	{
		free (object->t_project);
		free (object->t_description);
		free (object);
	}
}

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
		task_free_ll_object (preptr);
	}
}

/*
	Function: task_upgrade (struct bask_task* task);
	Description: Upgrades a task. (Internal use f.e. new versions upgrades.)
	InitVersion: 0.0.1
*/
static short task_upgrade (struct bask_task* task)
{
	return 0;
}

/*
	Function: task_insert (struct bask_task** first, unsigned int n, unsigned int tid, short tactive, short tpriority, int tstate, char* tadded, char* tdue, char* tfinished, char* tproject, char* tdescription);
	Description: Inserts a new task with data.
	InitVersion: 0.0.1
*/
short task_insert (struct bask_task** first, unsigned int n, unsigned int tid, short tactive, short tpriority, int tstate, char* tadded, char* tdue, char* tfinished, char* tproject, char* tdescription)
{
	struct bask_task *newobj = malloc (sizeof (struct bask_task)), *preobj;
	
	if (newobj == NULL)
	{
		return -1;
	}
	
	newobj->n = n;
	newobj->t_id = tid;
	newobj->t_priority = tpriority;
	newobj->t_flags = 0;
	newobj->t_flags ^= BITCOPY (tactive, 0, newobj->t_flags, TASK_FLAG_ACTIVE);
	newobj->t_flags ^= BITCOPY (tstate, 0, newobj->t_flags, TASK_FLAG_FINISHED);
	newobj->next = NULL;
	
	if (strlen (tadded) >= T_S_ADDED || strlen (tdue) >= T_S_DUE || strlen (tfinished) >= T_S_FINISHED || strlen (tproject) >= T_S_PROJECT || strlen (tdescription) >= T_S_DESCRIPTION)
	{
		/* No strings have allocated memory so we only need to free the structure. */
		free (newobj);
		return -2;
	}

	strcpy (newobj->t_added, tadded);
	strcpy (newobj->t_due, tdue);
	strcpy (newobj->t_finished, tfinished);
	
	utils_mkstr (strlen (tproject), &newobj->t_project);
	strcpy (newobj->t_project, tproject);
	
	utils_mkstr (strlen (tdescription), &newobj->t_description);
	strcpy (newobj->t_description, tdescription);
	
	task_upgrade (newobj);
	
	if (*first == NULL)
	{
		*first = newobj;
	}
	else
	{
		preobj = *first;
		
		while (TRUE)
		{
			if (preobj->n == newobj->n)
			{
				task_free_ll_object (newobj);
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
	Function: task_remove (struct bask_task** first, unsigned int id);
	Description: Removes a task!
	InitVersion: 0.0.1
*/
short task_remove (struct bask_task** first, unsigned int id)
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
				task_free_ll_object (ptr);
			}
			else
			{
				pre->next = ptr->next;
				task_free_ll_object (ptr);
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
	Function: task_modificate (struct bask_task** first, unsigned int id, short active, int state, short priority, char* added, char* due, char* finished, char* project, char* description);
	Description: Modificates a task!
	InitVersion: 0.0.1
*/
short task_modificate (struct bask_task** first, unsigned int id, short active, int state, short priority, char* added, char* due, char* finished, char* project, char* description)
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
				ptr->t_flags ^= BITCOPY (active, 0, ptr->t_flags, TASK_FLAG_ACTIVE);
			}
			
			if (state >= 0)
			{
				ptr->t_flags ^= BITCOPY (state, 0, ptr->t_flags, TASK_FLAG_FINISHED);
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
			
			if (due != NULL)
			{
				if (strlen (due) < T_S_DUE && strcmp (due, "") != 0 )
				{
					strcpy (ptr->t_due, due);
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
					utils_chstr (&ptr->t_project, project);
				}
			}
			
			if (description != NULL)
			{
				if (strlen (description) < T_S_DESCRIPTION && strcmp (description, "") != 0)
				{
					utils_chstr (&ptr->t_description, description);
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
	Function: task_create (bask_core* tcore, struct bask_task** first, short priority, char* project, char* description);
	Description: Creates a task!
	InitVersion: 0.0.1
*/
short task_create (bask_core* tcore, struct bask_task** first, short priority, char* project, char* description)
{	
	char added[T_S_ADDED];

	if (time_get_str (added, sizeof (added)) != 0)
	{
		return -1;
	}
	
	tcore->baskbin_uid++;
	
	task_insert (first, tcore->tc_amount, tcore->baskbin_uid, TRUE, priority, FALSE, added, "NONE", "NONE", project, description);
	
	tcore->tc_amount++;

	return 0;
}

/*
	Function: task_deactivate (struct bask_task** first, unsigned int id);
	Description: Deactivates a task!
	InitVersion: 0.0.1
*/
void task_deactivate (struct bask_task** first, unsigned int id)
{	
	task_modificate (first, id, FALSE, -1, -1, "", "", "", "", "");
}

/*
	Function: task_due (struct bask_task** first, unsigned int id, char* due);
	Description: Set the due date!
	InitVersion: 0.0.1
*/
void task_due (struct bask_task** first, unsigned int id, char* due)
{	
	task_modificate (first, id, -1, -1, -1, "", due, "", "", "");
}

/*
	Function: task_finish (struct bask_task** first, unsigned int id);
	Description: Finished a task!
	InitVersion: 0.0.1
*/
short task_finish (struct bask_task** first, unsigned int id)
{
	char finished[T_S_FINISHED];

	if (time_get_str (finished, sizeof (finished)) != 0)
	{
		return -1;
	}
	
	task_modificate (first, id, -1, TRUE, -1, "", "", finished, "", "");
	
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
short task_search (bask_core* tcore, struct bask_task** first, struct bask_task** haystack, char* searchtag)
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
			task_insert (haystack, i, ptr->t_id, BITGET (ptr->t_flags, TASK_FLAG_ACTIVE), ptr->t_priority, BITGET (ptr->t_flags, TASK_FLAG_FINISHED), ptr->t_added, ptr->t_due, ptr->t_finished, ptr->t_project, ptr->t_description);
		}
		else if (strstr (ptr->t_due, searchtag) != NULL)
		{
			task_insert (haystack, i, ptr->t_id, BITGET (ptr->t_flags, TASK_FLAG_ACTIVE), ptr->t_priority, BITGET (ptr->t_flags, TASK_FLAG_FINISHED), ptr->t_added, ptr->t_due, ptr->t_finished, ptr->t_project, ptr->t_description);
		}
		else if (strstr (ptr->t_finished, searchtag) != NULL)
		{
			task_insert (haystack, i, ptr->t_id, BITGET (ptr->t_flags, TASK_FLAG_ACTIVE), ptr->t_priority, BITGET (ptr->t_flags, TASK_FLAG_FINISHED), ptr->t_added, ptr->t_due, ptr->t_finished, ptr->t_project, ptr->t_description);
		}
		else if (strstr (ptr->t_project, searchtag) != NULL)
		{
			task_insert (haystack, i, ptr->t_id, BITGET (ptr->t_flags, TASK_FLAG_ACTIVE), ptr->t_priority, BITGET (ptr->t_flags, TASK_FLAG_FINISHED), ptr->t_added, ptr->t_due, ptr->t_finished, ptr->t_project, ptr->t_description);
		}
		else if (strstr (ptr->t_description, searchtag) != NULL)
		{
			task_insert (haystack, i, ptr->t_id, BITGET (ptr->t_flags, TASK_FLAG_ACTIVE), ptr->t_priority, BITGET (ptr->t_flags, TASK_FLAG_FINISHED), ptr->t_added, ptr->t_due, ptr->t_finished, ptr->t_project, ptr->t_description);
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
	Function: task_create_cmd (bask_core* tcore, struct bask_task** first, short priority, char* project, char* description);
	Description: Cmd handle for creating a task!
	InitVersion: 0.0.1
*/
void task_create_cmd (bask_core* tcore, struct bask_task** first, short priority, char* project, char* description)
{
	/* TODO: Move the range check of the priority variable into the task_check_input function. */
	if (task_check_input (tcore, "", "", "", project, description, TRUE) == 0 &&
	    task_check_input_nbrs (tcore, 0, priority, TRUE, TRUE) == 0)
	{
		task_create (tcore, first, priority, project, description);
		printf ("Created task %i.\n", tcore->baskbin_uid);
		export_baskbin (tcore, first, tcore->path_baskbin);
	}
}

/*
	Function: task_remove_cmd (bask_core* tcore, struct bask_task** first, unsigned int id);
	Description: Cmd handle for removing a task!
	InitVersion: 0.0.1
*/
void task_remove_cmd (bask_core* tcore, struct bask_task** first, unsigned int id)
{
	task_remove (first, id);
	printf ("Removed task %i.\n", id);
	export_baskbin (tcore, first, tcore->path_baskbin);
}

/*
	Function: task_modificate_cmd (bask_core* tcore, struct bask_task** first, unsigned int id, short active, int state, short priority, char* added, char* due, char* finished, char* project, char* description);
	Description: Cmd handle for modifying a task!
	InitVersion: 0.0.1
*/
void task_modificate_cmd (bask_core* tcore, struct bask_task** first, unsigned int id, short active, int state, short priority, char* added, char* due, char* finished, char* project, char* description)
{
	/* TODO: Move the range check of the priority variable into the task_check_input function. */
	if (task_check_input (tcore, added, due, finished, project, description, TRUE) == 0 &&
	    task_check_input_nbrs (tcore, id, (priority == -1 ? 0 : priority), (active == -1 ? FALSE : active), TRUE) == 0)
	{
		task_modificate (first, id, active, state, priority, added, due, finished, project, description);
		printf ("Modificated task %i.\n", id);
		export_baskbin (tcore, first, tcore->path_baskbin);
	}
}

/*
	Function: task_deactivate_cmd (bask_core* tcore, struct bask_task** first, unsigned int id);
	Description: Cmd handle for deactivating a task!
	InitVersion: 0.0.1
*/
void task_deactivate_cmd (bask_core* tcore, struct bask_task** first, unsigned int id)
{	
	task_deactivate (first, id);
	printf ("Deactivated task %i.\n", id);
	export_baskbin (tcore, first, tcore->path_baskbin);
}

/*
	Function: task_due_cmd (bask_core* tcore, struct bask_task** first, unsigned int id, char* due);
	Description: Cmd handle for setting a due date!
	InitVersion: 0.0.1
*/
void task_due_cmd (bask_core* tcore, struct bask_task** first, unsigned int id, char* due)
{
	if (task_check_input (tcore, "", due, "", "", "", TRUE) == 0)
	{
		task_due (first, id, due);
		printf ("Updated task %i.\n", id);
		export_baskbin (tcore, first, tcore->path_baskbin);
	}
}

/*
	Function: task_finish_cmd (bask_core* tcore, struct bask_task** first, unsigned int id);
	Description: Cmd handle for finishing a task!
	InitVersion: 0.0.1
*/
void task_finish_cmd (bask_core* tcore, struct bask_task** first, unsigned int id)
{
	task_finish (first, id);
	printf ("Finished task %i.\n", id);
	export_baskbin (tcore, first, tcore->path_baskbin);
}
