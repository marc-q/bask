/* Copyright 2015 Marc Volker Dickmann */
#include <stdio.h>
#include <string.h>
#include "bask_core.h"
#include "bask_errors.h"
#include "bask_task.h"
#include "bask_export.h"
#include "bask_import.h"

/*
	Function: import_baskbin (bask_core* tcore, struct bask_task** first, char* filename);
	Description: Imports the tasks from a baskbin formated file.
	InitVersion: 0.0.1
*/
int import_baskbin (bask_core* tcore, struct bask_task** first, char* filename)
{
	int tid, tactive, tpriority, tstate, bb_state;
	char line[200], tadded[T_S_ADDED], tfinished[T_S_FINISHED], tproject[T_S_PROJECT], tdescription[T_S_DESCRIPTION];
	char *token, *saveptr;
	FILE* importfile;

	bb_state = tid = tactive = tpriority = tstate = 0;
	
	strcpy (tadded, " ");
	strcpy (tfinished, " ");
	strcpy (tproject, " ");
	strcpy (tdescription, " "); 

	importfile = fopen (filename, "r");

	if (importfile == NULL)
	{
		errors_filenotopened (filename);
		return -1;
	}
	
	while (fgets (line, sizeof (line), importfile) != NULL)
	{
		token = strtok_r (line, BASKSEP, &saveptr);
		
		if (strncmp (token, "BASKBIN", strlen ("BASKBIN")) == 0)
		{
			bb_state = 1;
		}
		else if (strncmp (token, "BBEND", strlen ("BBEND")) == 0)
		{
			bb_state = 0;
		}
		
		if (bb_state == 1)
		{
			parser_get_int (token, "bbuid", &tcore->baskbin_uid, saveptr);
		}
		else if (strncmp (token, "END", strlen ("END")) == 0)
		{
			task_insert (first, tcore->tc_amount, tid, tactive, tpriority, tstate, tadded, tfinished, tproject, tdescription);
			tcore->tc_amount++;
		}
		else
		{
			parser_get_int (token, "tid", &tid, saveptr);
			parser_get_int (token, "tactive", &tactive, saveptr);
			parser_get_int (token, "tstate", &tstate, saveptr);
			parser_get_int (token, "tpriority", &tpriority, saveptr);
			parser_get_str (token, "tadded", tadded, sizeof (tadded), saveptr);
			parser_get_str (token, "tfinished", tfinished, sizeof (tfinished), saveptr);
			parser_get_str (token, "tproject", tproject, sizeof (tproject), saveptr);
			parser_get_str (token, "tdescription", tdescription, sizeof (tdescription), saveptr);
		}
	}
	
	fclose (importfile);
	
	return 0;
}

/*
	Function: import_baskbin_cmd (bask_core* tcore, struct bask_task** first, char* filename);
	Description: Cmd handle for importing tasks from a baskbin formated file.
	InitVersion: 0.0.1
*/
void import_baskbin_cmd (bask_core* tcore, struct bask_task** first, char* filename)
{
	import_baskbin (tcore, first, filename);
	export_baskbin (tcore, first, tcore->path_baskbin);
}

/*
	Function: import_csv_parser (bask_core* tcore, struct bask_task** first, char* token, char* saveptr);
	Description: Parses a task data line from a csv formated file. 
	InitVersion: 0.0.1
*/
static int import_csv_parser (bask_core* tcore, struct bask_task** first, char* token, char* saveptr)
{
	int tid, tactive, tpriority, tstate;
	char tadded[T_S_ADDED], tfinished[T_S_FINISHED], tproject[T_S_PROJECT], tdescription[T_S_DESCRIPTION];
	
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
	
	if (strlen (token) < T_S_ADDED)
	{
		strcpy (tadded, token); 
	}
	
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
	
	task_insert (first, tcore->baskbin_uid, tid, tactive, tpriority, tstate, tadded, tfinished, tproject, tdescription);
	
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
	
	return 0;
}

/*
	Function: import_csv_cmd (bask_core* tcore, struct bask_task** first, char* filename);
	Description: Cmd handle for importing tasks from a csv formated file.
	InitVersion: 0.0.1
*/
void import_csv_cmd (bask_core* tcore, struct bask_task** first, char* filename)
{
	import_csv (tcore, first, filename);
	export_baskbin (tcore, first, tcore->path_baskbin);
}
