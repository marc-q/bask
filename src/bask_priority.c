/* Copyright 2016 Marc Volker Dickmann */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../lib/dutils.h"
#include "bask_core.h"
#include "bask_ui.h"
#include "bask_priority.h"

/* |--------------------------------------------|
   |			Core			|
   |--------------------------------------------| */

/*
	Function: priority_free_ll (bask_priority** first);
	Description: Free's all elements from a bask_priority linked list.
	InitVersion: 0.0.1
*/
void priority_free_ll (bask_priority** first)
{
	bask_priority* ptr = *first, *preptr;
	
	while (ptr != NULL)
	{
		preptr = ptr;
		ptr = ptr->next;
		free (preptr);
	}
}

/*
	Function: priority_insert (bask_priority** first, short p_id, char* p_color, char* p_alias, char* p_name);
	Description: Inserts a new priority with data.
	InitVersion: 0.0.1
*/
short priority_insert (bask_priority** first, short p_id, char* p_color, char* p_alias, char* p_name)
{
	bask_priority *newobj = malloc (sizeof (bask_priority)), *preobj;
	
	if (newobj == NULL)
	{
		return -1;
	}
	
	newobj->p_id = p_id;
	newobj->next = NULL;
	
	if (strlen (p_color) >= PRI_S_PCOLOR || strlen (p_alias) >= PRI_S_PALIAS || strlen (p_name) >= PRI_S_PNAME)
	{
		/* Free the new obj and return the error code. */
		free (newobj);
		return -2;
	}

	strcpy (newobj->p_color, p_color);
	strcpy (newobj->p_alias, p_alias);
	strcpy (newobj->p_name, p_name);
	
	if (*first == NULL)
	{
		*first = newobj;
	}
	else
	{
		preobj = *first;
		
		while (TRUE)
		{
			if (preobj->p_id == newobj->p_id)
			{
				free (newobj);
				break;
			}
			else if (preobj->next == NULL)
			{
				preobj->next = newobj;
				break;
			}
			else if (preobj->next->p_id > newobj->p_id)
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

/* |--------------------------------------------|
   |			Base			|
   |--------------------------------------------| */

/*
	Function: priority_get_idfromstr (bask_priority** first, char* str);
	Description: Converts the str to the priority number.
	InitVersion: 0.0.1
*/
short priority_get_idfromstr (bask_priority** first, char* str)
{
	short tmp;
	bask_priority* ptr = *first;
	
	tmp = 0;
	
	if (ptr == NULL)
	{
		/* If the priority couldnt be determed, its 0 = normal */
		return 0;
	}
	
	if (isdigit (str[0]) != 0  || isdigit (str[1]) != 0 && (str[0] == '-' || str[0] == '+'))
	{
		utils_atos (&tmp, str);
	}
	else if (isalpha (str[0]) != 0)
	{
		while (ptr != NULL)
		{
			if (utils_streq (ptr->p_alias, str) == 0)
			{
				tmp = ptr->p_id;
				break;
			}
		
			ptr = ptr->next;
		}
	}
	
	return tmp;
}

/*
	Function: priority_get_viewdata (bask_priority** first, short p_id, char* p_color, size_t p_color_size, char* p_alias, size_t p_alias_size);
	Description: Gets the data required for the views.
	InitVersion: 0.0.1
*/
short priority_get_viewdata (bask_priority** first, short p_id, char* p_color, size_t p_color_size, char* p_alias, size_t p_alias_size)
{
	bask_priority* ptr = *first;
	
	if (ptr == NULL)
	{
		return -1;
	}
	
	while (ptr != NULL)
	{
		if (ptr->p_id == p_id)
		{
			if (strlen (ptr->p_color) < p_color_size)
			{
				strcpy (p_color, ptr->p_color);
			}
			
			if (strlen (ptr->p_alias) < p_alias_size)
			{
				strcpy (p_alias, ptr->p_alias);
			}
			break;
		}
		
		ptr = ptr->next;
	}
	
	return 0;
}
   
/*
	Function: priority_create (bask_core* tcore, bask_priority** first, short p_id, char* p_color, char* p_alias, char* p_name);
	Description: Created a new priority with the given data and updates the limits.
	InitVersion: 0.0.1
*/
void priority_create (bask_core* tcore, bask_priority** first, short p_id, char* p_color, char* p_alias, char* p_name)
{
	if (priority_insert (first, p_id, p_color, p_alias, p_name) == 0)
	{
		if (tcore->priority_max < p_id)
		{
			tcore->priority_max = p_id;
		}
		
		if (tcore->priority_min > p_id)
		{
			tcore->priority_min = p_id;
		}
	}
}
