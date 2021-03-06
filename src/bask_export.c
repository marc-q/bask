/* Copyright 2015 - 2017 Marc Volker Dickmann */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../lib/dutils.h"
#include "bask_core.h"
#include "bask_errors.h"
#include "bask_time.h"
#include "bask_task.h"

/* |--------------------------------------------|
   |			Utils			|
   |--------------------------------------------| */
/*
	Function: export_msg_success (char* filename);
	Description: Prints the message for when the export is successfull.
	InitVersion: 0.0.1
*/
static void export_msg_success (char* filename)
{
	printf ("Baskbin successfully exportet as %s.\n", filename);
}

/* |--------------------------------------------|
   |			Baskbin			|
   |--------------------------------------------| */
   
/*
	Function: export_baskbin (bask_core* tcore, struct bask_task** first, char* filename);
	Description: Exports all tasks to the baskbin <filename>.
	InitVersion: 0.0.1
*/
short export_baskbin (bask_core* tcore, struct bask_task** first, char* filename)
{
	FILE* exportfile;
	struct bask_task* ptr = *first;
	
	exportfile = fopen (filename, "w");

	if (exportfile == NULL)
	{
		errors_filenotwritten (filename);
		/* Note: At this point, were already have data in the linked list.
			 So we must use this to free the allocated memory. */
		return -1;
	}
	
	fprintf (exportfile, "BASKBIN\n");
	fprintf (exportfile, "bbuid=%i\n", tcore->baskbin_uid);
	fprintf (exportfile, "BBEND\n");
	
	while (ptr != NULL)
	{
		fprintf (exportfile, "tid=%i\n", ptr->t_id);
		fprintf (exportfile, "tactive=%hi\n", BITGET (ptr->t_flags, TASK_FLAG_ACTIVE));
		fprintf (exportfile, "tpriority=%hi\n", ptr->t_priority);
		fprintf (exportfile, "tadded=%s\n", ptr->t_added);
		fprintf (exportfile, "tdue=%s\n", ptr->t_due);
		fprintf (exportfile, "tfinished=%s\n", ptr->t_finished);
		fprintf (exportfile, "tproject=%s\n", ptr->t_project);
		fprintf (exportfile, "tdescription=%s\n", ptr->t_description);
		fprintf (exportfile, "END\n");
		ptr = ptr->next;
	}
	
	fclose (exportfile);
	
	return 0;
}

/*
	Function: export_baskbin_cmd (bask_core* tcore, struct bask_task** first, char* filename);
	Description: Cmd handle for exporting to a baskbin file.
	InitVersion: 0.0.1
*/
void export_baskbin_cmd (bask_core* tcore, struct bask_task** first, char* filename)
{
	if (export_baskbin (tcore, first, filename) == 0)
	{
		export_msg_success (filename);
	}
}

/* |--------------------------------------------|
   |			CSV			|
   |--------------------------------------------| */

/*
	Function: export_csv (bask_core* tcore, struct bask_task** first, char* filename);
	Description: Exports all tasks to an csv file named filename.
	InitVersion: 0.0.1
*/
short export_csv (bask_core* tcore, struct bask_task** first, char* filename)
{
	FILE *exportfile;
	struct bask_task* ptr = *first;
	
	exportfile = fopen (filename, "w");
	
	if (exportfile == NULL)
	{
		errors_filenotwritten (filename);
		return -1;
	}
	
	fprintf (exportfile, "ID;Active;Priority;Added;Due;Finished;Project;Description\n");
	
	while (ptr != NULL)
	{
		fprintf (exportfile, "%i;%hi;%hi;\"%s\";\"%s\";\"%s\";\"%s\";\"%s\"\n", ptr->t_id, BITGET (ptr->t_flags, TASK_FLAG_ACTIVE), ptr->t_priority, ptr->t_added, ptr->t_due, ptr->t_finished, ptr->t_project, ptr->t_description);
		
		ptr = ptr->next;
	}
	
	fclose (exportfile);
	
	return 0;
}

/*
	Function: export_csv_cmd (bask_core* tcore, struct bask_task** first, char* filename);
	Description: Cmd handle for exporting to a csv file.
	InitVersion: 0.0.1
*/
void export_csv_cmd (bask_core* tcore, struct bask_task** first, char* filename)
{
	if (export_csv (tcore, first, filename) == 0)
	{
		export_msg_success (filename);
	}
}

/* |--------------------------------------------|
   |			iCal			|
   |--------------------------------------------| */

/*
	Function: export_ical_getdatestr (char* out, char* datestr);
	Description: Converts an baskbin datestr to an ical datestr.
	InitVersion: 0.0.1
*/
short export_ical_getdatestr (char* out, char* datestr)
{
	char idate[F_ICAL_S_DATE];
	
	if (strlen (datestr) != F_BB_S_DATE-1)
	{
		return -1;
	}
	
	idate[0] = datestr[15]; /* Year */
	idate[1] = datestr[16];
	idate[2] = datestr[17];
	idate[3] = datestr[18];
	idate[4] = datestr[12]; /* Month */
	idate[5] = datestr[13];
	idate[6] = datestr[9];  /* Day */
	idate[7] = datestr[10];
	idate[8] = 'T';
	idate[9] = datestr[0];  /* Hour */
	idate[10] = datestr[1];
	idate[11] = datestr[3]; /* Minute */
	idate[12] = datestr[4];
	idate[13] = datestr[6]; /* Second */
	idate[14] = datestr[7];
	idate[15] = '\0';
	
	strcpy (out, idate);
	
	return 0;
}

/*
	Function: export_ical_event (FILE* exportfile, struct bask_task* task);
	Description: Exports a tasks with an vevent to an ical file.
	InitVersion: 0.0.1
*/
static short export_ical_event (FILE* exportfile, struct bask_task* task)
{
	char tadded[F_ICAL_S_DATE];
	char tfinished[F_ICAL_S_DATE];
	
	if (exportfile == NULL)
	{
		return -1;
	}
	
	strcpy (tadded, "");
	strcpy (tfinished, "");
	
	export_ical_getdatestr (tadded, task->t_added);
	export_ical_getdatestr (tfinished, task->t_finished);
	
	if (strlen (tadded) == 0 && strlen (tfinished) == 0 || strlen (tadded) == 0)
	{
		return -2;
	}
	else if (strlen (tadded) != 0 && strlen (tfinished) == 0)
	{
		strcpy (tfinished, tadded);
	}
	
	fprintf (exportfile, "BEGIN:VEVENT\n");
	fprintf (exportfile, "DTSTART:%s\n", tadded);
	fprintf (exportfile, "DTEND:%s\n", tfinished);
	fprintf (exportfile, "DSTSTAMP:%s\n", tadded);
	fprintf (exportfile, "UID:%i\n", task->t_id);
	fprintf (exportfile, "CREATED:%s\n", tadded);
	fprintf (exportfile, "DESCRIPTION:%s\n", task->t_description);
	fprintf (exportfile, "LAST-MODIFIED:%s\n", tadded);
	fprintf (exportfile, "SUMMARY:%s - %s\n", task->t_project, task->t_description);
	fprintf (exportfile, "END:VEVENT\n");
	
	return 0;
}

/*
	Function: export_ical_todo (FILE* exportfile, struct bask_task* task);
	Description: Exports a tasks with an vtodo to an ical file.
	InitVersion: 0.0.1
*/
static short export_ical_todo (FILE* exportfile, struct bask_task* task)
{
	char tadded[F_ICAL_S_DATE];
	char tdue[F_ICAL_S_DATE];
	char tfinished[F_ICAL_S_DATE];
	
	if (exportfile == NULL)
	{
		return -1;
	}
	
	strcpy (tadded, "");
	strcpy (tdue, "");
	strcpy (tfinished, "");
	
	export_ical_getdatestr (tadded, task->t_added);
	export_ical_getdatestr (tdue, task->t_due);
	export_ical_getdatestr (tfinished, task->t_finished);
	
	if (strlen (tadded) == 0)
	{
		return -2;
	}
	
	fprintf (exportfile, "BEGIN:VTODO\n");
	fprintf (exportfile, "UID:%i\n", task->t_id);
	fprintf (exportfile, "DSTSTAMP:%s\n", tadded);
	fprintf (exportfile, "DTSTART:%s\n", tadded);
	
	if (strlen (tdue) > 1)
	{
		fprintf (exportfile, "DUE:%s\n", tdue);
	}
	
	if (strlen (tfinished) > 1)
	{
		fprintf (exportfile, "COMPLETED:%s\n", tfinished);
	}
	
	fprintf (exportfile, "PRIORITY:%hi\n", task->t_priority);
	fprintf (exportfile, "DESCRIPTION:%s\n", task->t_description);
	fprintf (exportfile, "SUMMARY:%s\n", task->t_project);
	
	fprintf (exportfile, "STATUS:%s\n", (BITGET (task->t_flags, TASK_FLAG_ACTIVE) == TRUE ? "IN-PROCESS" : "NEEDS-ACTION"));
	
	fprintf (exportfile, "END:VTODO\n");
	
	return 0;
}

/*
	Function: export_ical (bask_core* tcore, struct bask_task** first, char* filename);
	Description: Exports all tasks to an ical file named filename.
	InitVersion: 0.0.1
*/
short export_ical (bask_core* tcore, struct bask_task** first, char* filename)
{
	FILE *exportfile;
	struct bask_task* ptr = *first;
	
	exportfile = fopen (filename, "w");
	
	if (exportfile == NULL)
	{
		errors_filenotwritten (filename);
		return -1;
	}
	
	fprintf (exportfile, "BEGIN:VCALENDAR\nVERSION:2.0\nPRODID:-//Bask//NONSGML Baskbin//EN\n");
	
	while (ptr != NULL)
	{
		export_ical_todo (exportfile, ptr);
		
		ptr = ptr->next;
	}
	
	fprintf (exportfile, "END:VCALENDAR");
	
	fclose (exportfile);
	
	return 0;
}

/*
	Function: export_ical_cmd (bask_core* tcore, struct bask_task** first, char* filename);
	Description: Cmd handle for exporting to a ical file.
	InitVersion: 0.0.1
*/
void export_ical_cmd (bask_core* tcore, struct bask_task** first, char* filename)
{
	if (export_ical (tcore, first, filename) == 0)
	{
		export_msg_success (filename);
	}
}

/* |--------------------------------------------|
   |			Web			|
   |--------------------------------------------| */

/*
	Function: export_web_strrpl (char* str);
	Description: Replaces incompatible char for the web export.
	InitVersion: 0.0.1
*/
static void export_web_strrpl (char* str)
{
	/* TODO: Find a solution for this problem! */
	if (str != NULL)
	{
		while (*str)
		{
			switch (*str)
			{
				case '<':
					*str = ' ';
					break;
				case '>':
					*str = ' ';
					break;
				default:
					break;
			}
			*str++;
		}
	}
}

/*
	Function: export_web (bask_core* tcore, struct bask_task** first, char* filename);
	Description: Exports all tasks to an html file named filename.
	InitVersion: 0.0.1
*/
short export_web (bask_core* tcore, struct bask_task** first, char* filename)
{
	int i = 0;
	char prefix[30], pri[4], description[200];
	FILE *webfile;
	struct bask_task* ptr = *first;
	
	webfile = fopen (filename, "w");
	
	if (webfile == NULL)
	{
		errors_filenotwritten (filename);
		return -1;
	}
	
	fprintf (webfile, "<html>\n<head>\n<title>BaskExport</title>\n");
	
	fprintf (webfile, "</head>\n<body>\n<h1>BaskExport</h1>\n<table>\n<tbody>\n");
	fprintf (webfile, "<tr><td>ID</td><td>Project</td><td>Priority</td><td>Description</td></tr>\n");
	
	while (ptr != NULL)
	{
		if (BITGET (ptr->t_flags, TASK_FLAG_ACTIVE) == TRUE)
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

			/* TODO: Find a solution for this problem! */
			if (ptr->t_description != NULL)
			{
				strcpy (description, ptr->t_description);
				export_web_strrpl (description);
			}

			fprintf (webfile, "<tr><td>%i</td><td>%s</td><td>%s</td><td>%s</td></tr>\n", ptr->t_id, ptr->t_project, pri, description);
			
			i++;
		}
		
		ptr = ptr->next;
	}
	
	fprintf (webfile, "</tbody>\n</table>\n</body>\n</html>\n");
	
	fclose (webfile);
	
	return 0;
}
