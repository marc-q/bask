/* Copyright 2015 Marc Volker Dickmann */
#include <stdio.h>
#include <string.h>
#include "bask_core.h"
#include "bask_errors.h"
#include "bask_task.h"
#include "bask_import.h"

/*
	Function: import_csv_parser (bask_core* tcore, struct bask_task** first, char* token, char* saveptr);
	Description: Parses a task data line from a csv formated file. 
	InitVersion: 0.0.1
*/
static int import_csv_parser (bask_core* tcore, struct bask_task** first, char* token, char* saveptr)
{
	int tid, tactive, tpriority, tstate;
	char tfinished[T_S_FINISHED], tproject[T_S_PROJECT], tdescription[T_S_DESCRIPTION];
	
	tid = tactive = tpriority = tstate = 0;
	
	tcore->baskbin_uid++;
	
	tid = atoi (token);
	
	token = strtok_r (NULL, ";", &saveptr);
	
	tactive = atoi (token);
	
	token = strtok_r (NULL, ";", &saveptr);
	
	tstate = atoi (token);
	
	token = strtok_r (NULL, ";", &saveptr);
	
	tpriority = atoi (token);
	
	token = strtok_r (NULL, ";", &saveptr);
	
	if (strlen (token) < T_S_FINISHED)
	{
		strcpy (tfinished, token); 
	}
	
	token = strtok_r (NULL, ";", &saveptr);
	
	if (strlen (token) < T_S_PROJECT)
	{
		strcpy (tproject, token); 
	}
	
	token = strtok_r (NULL, ";\n", &saveptr);
	
	if (strlen (token) < T_S_DESCRIPTION)
	{
		strcpy (tdescription, token); 
	}
	
	task_insert (first, tcore->baskbin_uid, tid, tactive, tpriority, tstate, tfinished, tproject, tdescription);
	
	return 0;
}

/*
	Function: import_csv (bask_core* tcore, struct bask_task** first, char* filename);
	Description: Imports tasks from a csv formated file.
	InitVersion: 0.0.1
*/
int import_csv (bask_core* tcore, struct bask_task** first, char* filename)
{
	int tt_state;
	char line[200];
	char *token, *saveptr;
	FILE* importfile;
	
	tt_state = 0;

	importfile = fopen (filename, "r");

	if (importfile == NULL)
	{
		errors_filenotopened (filename);
		return -1;
	}
	
	while (fgets (line, sizeof (line), importfile) != NULL)
	{
		token = strtok_r (line, ";", &saveptr);
		
		if (tt_state == 0)
		{
			tt_state = 1;
		}
		else if (import_csv_parser (tcore, first, token, saveptr) == 0)
		{
			tcore->tc_amount++;
		}
		
	}
	
	fclose (importfile);
	
	bask_write (tcore, first);
	
	return 0;
}