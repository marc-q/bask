/* Copyright 2015 - 2016 Marc Volker Dickmann */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/dutils.h"
#include "bask_core.h"
#include "bask_errors.h"
#include "bask_task.h"
#include "bask_export.h"
#include "bask_import.h"

/* |--------------------------------------------|
   |			Utils			|
   |--------------------------------------------| */
/*
	Function: import_msg_success (char* filename);
	Description: Prints the message for when the import is successfull.
	InitVersion: 0.0.1
*/
static void import_msg_success (char* filename)
{
	printf ("%s successfully imported to the Baskbin.\n", filename);
}


/* |--------------------------------------------|
   |			Baskbin			|
   |--------------------------------------------| */

/*
	Function: import_baskbin (bask_core* tcore, struct bask_task** first, char* filename);
	Description: Imports the tasks from a baskbin formated file.
	InitVersion: 0.0.1
*/
short import_baskbin (bask_core* tcore, struct bask_task** first, char* filename)
{
	unsigned int tid;
	int tstate, bb_state;
	short tactive, tpriority;
	char line[200], tadded[T_S_ADDED], tdue[T_S_DUE], tfinished[T_S_FINISHED], tproject[T_S_PROJECT], tdescription[T_S_DESCRIPTION];
	char saveptr[200];
	FILE* importfile;

	bb_state = tid = tactive = tpriority = tstate = 0;
	
	strcpy (tadded, "NONE");
	strcpy (tdue, "NONE");
	strcpy (tfinished, "NONE");
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
		if (utils_streq (line, "BASKBIN\n") == 0)
		{
			bb_state = TRUE;
		}
		else if (utils_streq (line, "BBEND\n") == 0)
		{
			bb_state = FALSE;
		}
			
		if (bb_state == TRUE)
		{
			parser_get_int (line, "bbuid=", &tcore->baskbin_uid, BASKSEP, saveptr);
		}
		else if (utils_streq (line, "END\n") == 0)
		{
			task_insert (first, tcore->tc_amount, tid, tactive, tpriority, tstate, tadded, tdue, tfinished, tproject, tdescription);
			tcore->tc_amount++;
		}
		else
		{
			parser_get_int (line, "tid=", &tid, BASKSEP, saveptr);
			parser_get_short (line, "tactive=", &tactive, BASKSEP, saveptr);
			parser_get_short (line, "tpriority=", &tpriority, BASKSEP, saveptr);
			parser_get_str (line, "tadded=", tadded, sizeof (tadded), BASKSEP, saveptr);
			parser_get_str (line, "tdue=", tdue, sizeof (tdue), BASKSEP, saveptr);
			
			if (parser_get_str (line, "tfinished=", tfinished, sizeof (tfinished), BASKSEP, saveptr) == 0)
			{
				/* Use this instead of tstate because this tells us already the state. */
				if (strlen (tfinished) == F_BB_S_DATE-1)
				{
					tstate = TRUE;
				}
				else
				{
					tstate = FALSE;
				}
			}
			
			parser_get_str (line, "tproject=", tproject, sizeof (tproject), BASKSEP, saveptr);
			parser_get_str (line, "tdescription=", tdescription, sizeof (tdescription), BASKSEP, saveptr);
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
	if (import_baskbin (tcore, first, filename) == 0)
	{
		import_msg_success (filename);
	}
	
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
void import_csv_parser (bask_core* tcore, struct bask_task** first, char* token, char* saveptr)
{
	unsigned int tid;
	int tstate;
	short tactive, tpriority;
	char tadded[T_S_ADDED], tdue[T_S_DUE], tfinished[T_S_FINISHED], tproject[T_S_PROJECT], tdescription[T_S_DESCRIPTION];
	char tmp[200];
	
	tid = tactive = tpriority = tstate = 0;
	
	tid = atoi (token);
	
	token = strtok_r (NULL, ";", &saveptr);
	
	utils_atos (&tactive, token);
	
	token = strtok_r (NULL, ";", &saveptr);
	
	utils_atos (&tpriority, token);
	
	token = strtok_r (NULL, ";", &saveptr);
	
	/* Subtracting 2 byte for the " chars. */
	if (strlen (token)-2 < T_S_ADDED)
	{
		strcpy (tmp, token); 
		
		if (tmp[0] == '"')
		{
			strcpy (tadded, &tmp[1]);
		}
		else
		{
			strcpy (tadded, tmp);
		}
		
		if (tadded[strlen (tadded)-1] == '"')
		{
			tadded[strlen (tadded)-1] = '\0';
		}
	}
	else
	{
		strcpy (tadded, "NONE");
	}
	
	token = strtok_r (NULL, ";", &saveptr);
	
	/* Subtracting 2 byte for the " chars. */
	if (strlen (token)-2 < T_S_DUE)
	{
		strcpy (tmp, token); 
		
		if (tmp[0] == '"')
		{
			strcpy (tdue, &tmp[1]);
		}
		else
		{
			strcpy (tdue, tmp);
		}
		
		if (tdue[strlen (tdue)-1] == '"')
		{
			tdue[strlen (tdue)-1] = '\0';
		}
	}
	else
	{
		strcpy (tdue, "NONE");
	}
	
	token = strtok_r (NULL, ";", &saveptr);
	
	/* Subtracting 2 byte for the " chars. */
	if (strlen (token)-2 < T_S_FINISHED)
	{
		strcpy (tmp, token); 
		
		if (tmp[0] == '"')
		{
			strcpy (tfinished, &tmp[1]);
		}
		else
		{
			strcpy (tfinished, tmp);
		}
		
		if (tfinished[strlen (tfinished)-1] == '"')
		{
			tfinished[strlen (tfinished)-1] = '\0';
		}
		
		/* If the string isnt set to NONE its finished. */
		if (strlen (tfinished) > 4)
		{
			tstate = TRUE;
		}
	}
	else
	{
		strcpy (tfinished, "NONE");
	}
	
	token = strtok_r (NULL, ";", &saveptr);
	
	/* Subtracting 2 byte for the " chars. */
	if (strlen (token)-2 < T_S_PROJECT)
	{
		strcpy (tmp, token); 
		
		if (tmp[0] == '"')
		{
			strcpy (tproject, &tmp[1]);
		}
		else
		{
			strcpy (tproject, tmp);
		}
		
		if (tproject[strlen (tproject)-1] == '"')
		{
			tproject[strlen (tproject)-1] = '\0';
		}
	}
	else
	{
		strcpy (tproject, " ");
	}
	
	token = strtok_r (NULL, ";\n", &saveptr);
	
	/* Subtracting 2 byte for the " chars. */
	if (strlen (token)-2 < T_S_DESCRIPTION)
	{
		strcpy (tmp, token); 
		
		if (tmp[0] == '"')
		{
			strcpy (tdescription, &tmp[1]);
		}
		else
		{
			strcpy (tdescription, tmp);
		}
		
		if (tdescription[strlen (tdescription)-1] == '"')
		{
			tdescription[strlen (tdescription)-1] = '\0';
		}
	}
	else
	{
		strcpy (tdescription, " ");
	}
	
	tcore->baskbin_uid++;
	task_insert (first, tcore->tc_amount, tid, tactive, tpriority, tstate, tadded, tdue, tfinished, tproject, tdescription);
	tcore->tc_amount++;
}

/*
	Function: import_csv (bask_core* tcore, struct bask_task** first, char* filename);
	Description: Imports tasks from a csv formated file.
	InitVersion: 0.0.1
*/
short import_csv (bask_core* tcore, struct bask_task** first, char* filename)
{
	int tt_state;
	char line[200];
	char *token, *saveptr;
	FILE* importfile;
	
	tt_state = FALSE;

	importfile = fopen (filename, "r");

	if (importfile == NULL)
	{
		errors_filenotopened (filename);
		return -1;
	}
	
	while (fgets (line, sizeof (line), importfile) != NULL)
	{
		token = strtok_r (line, ";", &saveptr);
		
		if (tt_state == FALSE)
		{
			tt_state = TRUE;
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
	if (import_csv (tcore, first, filename) == 0)
	{
		import_msg_success (filename);
	}
	
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
short import_ical_getdatestr (char* out, char* datestr)
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
short import_ical (bask_core* tcore, struct bask_task** first, char* filename)
{
	unsigned int tid;
	short tactive, tpriority, tstate;
	char line[200], tadded[T_S_ADDED], tdue[T_S_DUE], tfinished[T_S_FINISHED], tproject[T_S_PROJECT], tdescription[T_S_DESCRIPTION];
	char tt_tmp[50];
	char saveptr[200];
	FILE* importfile;
	
	tid = tpriority = tstate = 0;
	tactive = 1;
	
	strcpy (tt_tmp, " ");
	strcpy (tadded, " ");
	strcpy (tdue, " ");
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
		
		if (parser_get_str (line, "END:", tt_tmp, sizeof (tt_tmp), ICALSEP, saveptr) == 0)
		{
			if (utils_streq (tt_tmp, "VTODO") == 0)
			{
				if (tid == 0)
				{
					tcore->baskbin_uid++;
					tid = tcore->baskbin_uid;
				}
				
				task_insert (first, tcore->tc_amount, tid, tactive, tpriority, tstate, tadded, tdue, tfinished, tproject, tdescription);
				
				tcore->tc_amount++;
				tid = tpriority= 0;
				
				/* If there is no clue to tactive we enable it by default. */
				tactive = 1;
			}
		}
		else
		{
			parser_get_int (line, "UID:", &tid, ICALSEP, saveptr);
			
			/* Try multiple ways to find the data. */
			if (parser_get_str (line, "CREATED:", tt_tmp, sizeof (tt_tmp), ICALSEP, saveptr) == 0)
			{
				import_ical_getdatestr (tadded, tt_tmp);
			}
			else if (parser_get_str (line, "DTSTART:", tt_tmp, sizeof (tt_tmp), ICALSEP, saveptr) == 0)
			{
				import_ical_getdatestr (tadded, tt_tmp);
			}
			else if (parser_get_str (line, "COMPLETED:", tt_tmp, sizeof (tt_tmp), ICALSEP, saveptr) == 0)
			{
				if (import_ical_getdatestr (tfinished, tt_tmp) == -1)
				{
					strcpy (tfinished, "NONE");
					tstate = 0;
				}
				else
				{
					tstate = 1;
				}
			}
			else if (parser_get_str (line, "DTEND:", tt_tmp, sizeof (tt_tmp), ICALSEP, saveptr) == 0)
			{
				if (import_ical_getdatestr (tfinished, tt_tmp) == -1)
				{
					strcpy (tfinished, "NONE");
					tstate = 0;
				}
				else
				{
					tstate = 1;
				}
			}
			else if (parser_get_str (line, "DUE:", tt_tmp, sizeof (tt_tmp), ICALSEP, saveptr) == 0)
			{
				if (import_ical_getdatestr (tdue, tt_tmp) == -1)
				{
					strcpy (tdue, "NONE");
				}
			}
			
			parser_get_short (line, "PRIORITY:", &tpriority, ICALSEP, saveptr);
			parser_get_str (line, "DESCRIPTION:", tdescription, sizeof (tdescription), ICALSEP, saveptr);
			parser_get_str (line, "SUMMARY:", tproject, sizeof (tproject), ICALSEP, saveptr);
			
			if (parser_get_str (line, "STATUS:", tt_tmp, sizeof (tt_tmp), ICALSEP, saveptr) == 0)
			{
				if (utils_streq (tt_tmp, "IN-PROCESS") == 0)
				{
					tactive = 1;
				}
				else if (utils_streq (tt_tmp, "NEEDS-ACTION") == 0)
				{
					tactive = 0;
				}
			}
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
	if (import_ical (tcore, first, filename) == 0)
	{
		import_msg_success (filename);
	}
	
	export_baskbin (tcore, first, tcore->path_baskbin);
}
