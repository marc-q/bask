/* Copyright 2015 Marc Volker Dickmann */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bask_core.h"
#include "bask_errors.h"
#include "bask_task.h"
#include "bask_export.h"
#include "bask_import.h"

/* |--------------------------------------------|
   |			Baskbin			|
   |--------------------------------------------| */

/*
	Function: import_baskbin (bask_core* tcore, struct bask_task** first, char* filename);
	Description: Imports the tasks from a baskbin formated file.
	InitVersion: 0.0.1
*/
int import_baskbin (bask_core* tcore, struct bask_task** first, char* filename)
{
	unsigned int tid;
	int tactive, tpriority, tstate, bb_state;
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
		
		if (utils_streq (token, "BASKBIN\n") == 0)
		{
			bb_state = 1;
		}
		else if (utils_streq (token, "BBEND\n") == 0)
		{
			bb_state = 0;
		}
			
		if (bb_state == 1)
		{
			parser_get_int (token, "bbuid", &tcore->baskbin_uid, BASKSEP, saveptr);
		}
		else if (utils_streq (token, "END\n") == 0)
		{
			task_insert (first, tcore->tc_amount, tid, tactive, tpriority, tstate, tadded, tfinished, tproject, tdescription);
			tcore->tc_amount++;
		}
		else
		{
			parser_get_int (token, "tid", &tid, BASKSEP, saveptr);
			parser_get_int (token, "tactive", &tactive, BASKSEP, saveptr);
			parser_get_int (token, "tpriority", &tpriority, BASKSEP, saveptr);
			parser_get_str (token, "tadded", tadded, sizeof (tadded), BASKSEP, saveptr);
			if (parser_get_str (token, "tfinished", tfinished, sizeof (tfinished), BASKSEP, saveptr) == 0)
			{
				/* Use this instead of tstate because this tells us already the state. */
				if (strlen (tfinished) == F_BB_S_DATE-1)
				{
					tstate = 1;
				}
				else
				{
					tstate = 0;
				}
			}
			
			parser_get_str (token, "tproject", tproject, sizeof (tproject), BASKSEP, saveptr);
			parser_get_str (token, "tdescription", tdescription, sizeof (tdescription), BASKSEP, saveptr);
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

/* |--------------------------------------------|
   |			CSV			|
   |--------------------------------------------| */

/*
	Function: import_csv_parser (bask_core* tcore, struct bask_task** first, char* token, char* saveptr);
	Description: Parses a task data line from a csv formated file. 
	InitVersion: 0.0.1
*/
static int import_csv_parser (bask_core* tcore, struct bask_task** first, char* token, char* saveptr)
{
	unsigned int tid;
	int tactive, tpriority, tstate;
	char tadded[T_S_ADDED], tfinished[T_S_FINISHED], tproject[T_S_PROJECT], tdescription[T_S_DESCRIPTION];
	
	tid = tactive = tpriority = tstate = 0;
	
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
	else
	{
		strcpy (tadded, " ");
	}
	
	token = strtok_r (NULL, ";", &saveptr);
	
	if (strlen (token) < T_S_FINISHED)
	{
		strcpy (tfinished, token); 
	}
	else
	{
		strcpy (tfinished, " ");
	}
	
	token = strtok_r (NULL, ";", &saveptr);
	
	if (strlen (token) < T_S_PROJECT)
	{
		strcpy (tproject, token); 
	}
	else
	{
		strcpy (tproject, " ");
	}
	
	token = strtok_r (NULL, ";\n", &saveptr);
	
	if (strlen (token) < T_S_DESCRIPTION)
	{
		strcpy (tdescription, token); 
	}
	else
	{
		strcpy (tdescription, " ");
	}
	
	tcore->baskbin_uid++;
	task_insert (first, tcore->tc_amount, tid, tactive, tpriority, tstate, tadded, tfinished, tproject, tdescription);
	tcore->tc_amount++;
	
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
		else
		{
			import_csv_parser (tcore, first, token, saveptr);
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

/* |--------------------------------------------|
   |			iCal			|
   |--------------------------------------------| */

/* Warning: The ical import functions are under construction,
	    Dont use them until this warning is gone!
	    Some things need to be worked out! */
/*
	Function: import_ical_getdatestr (char* out, char* datestr);
	Description: Converts an ical datestr to an baskbin datestr.
	InitVersion: 0.0.1
*/
int import_ical_getdatestr (char* out, char* datestr)
{	
	char bdate[F_BB_S_DATE];
	
	if (strlen (datestr) != F_ICAL_S_DATE-1)
	{
		return -1;
	}
	
	bdate[0] = datestr[9];  /* Hour */
	bdate[1] = datestr[10];
	bdate[2] = '/';
	bdate[3] = datestr[11]; /* Minute */
	bdate[4] = datestr[12];
	bdate[5] = '/';
	bdate[6] = datestr[13]; /* Second */
	bdate[7] = datestr[14];
	bdate[8] = '/';
	bdate[9] = datestr[6]; 	/* Day */
	bdate[10] = datestr[7];
	bdate[11] = '/';
	bdate[12] = datestr[4]; /* Month */
	bdate[13] = datestr[5];
	bdate[14] = '/';
	bdate[15] = datestr[0]; /* Year */
	bdate[16] = datestr[1];
	bdate[17] = datestr[2];
	bdate[18] = datestr[3];
	bdate[19] = '\0';
	
	strcpy (out, bdate);
	
	return 0;
}

/*
	Function: import_ical (bask_core* tcore, struct bask_task** first, char* filename);
	Description: Imports tasks from a ical formated file.
	InitVersion: 0.0.1
*/
int import_ical (bask_core* tcore, struct bask_task** first, char* filename)
{
	char line[200], tadded[T_S_ADDED], tfinished[T_S_FINISHED], tproject[T_S_PROJECT], tdescription[T_S_DESCRIPTION];
	char tt_tmp[50];
	char *token, *saveptr;
	FILE* importfile;
	
	strcpy (tt_tmp, " ");
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
		token = strtok_r (line, ICALSEP, &saveptr);
		
		if (parser_get_str (token, "END", tt_tmp, sizeof (tt_tmp), ICALSEP, saveptr) == 0)
		{
			if (utils_streq (tt_tmp, "VEVENT") == 0)
			{
				tcore->baskbin_uid++;
				task_insert (first, tcore->tc_amount, tcore->baskbin_uid, 1, 0, 0, tadded, tfinished, tproject, tdescription);
				tcore->tc_amount++;
			}
		}
		else
		{
			if (parser_get_str (token, "CREATED", tt_tmp, sizeof (tt_tmp), ICALSEP, saveptr) == 0)
			{
				import_ical_getdatestr (tadded, tt_tmp);
			}
			else if (parser_get_str (token, "DTEND", tt_tmp, sizeof (tt_tmp), ICALSEP, saveptr) == 0)
			{
				import_ical_getdatestr (tfinished, tt_tmp);
			}
			
			parser_get_str (token, "DESCRIPTION", tdescription, sizeof (tdescription), ICALSEP, saveptr);
		}
	}
	
	fclose (importfile);
	
	return 0;
}

/*
	Function: import_ical_cmd (bask_core* tcore, struct bask_task** first, char* filename);
	Description: Cmd handle for importing tasks from a ical formated file.
	InitVersion: 0.0.1
*/
void import_ical_cmd (bask_core* tcore, struct bask_task** first, char* filename)
{
	import_ical (tcore, first, filename);
	export_baskbin (tcore, first, tcore->path_baskbin);
}
