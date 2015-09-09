/* Copyright 2015 Marc Volker Dickmann */
/* Project: Bask */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include "lib/dutils.h"
#include "src/bask_core.h"
#include "src/bask_task.h"
#include "src/bask_project.h"
#include "src/bask_views.h"
#include "bask.h"

#define P_CMD "bask"
#define P_VERSION "0.0.1"
#define BASKSEP "=;"
#define BASKFILE "basktest.txt"

/* |--------------------------------------------|
   |			Utils			|
   |--------------------------------------------| */
   
/*
	Function: parser_get_str (char* token, char* key, char* out, size_t outsize, char* saveptr);
	Description: Parses a row and return the value if the key is right.
	InitVersion: 0.0.1
*/
static int parser_get_str (char* token, char* key, char* out, size_t outsize, char* saveptr)
{
	if (strncmp (token, key, strlen (token)) == 0)
	{
		token = strtok_r (NULL, BASKSEP, &saveptr);

		if (strlen (token) < outsize)
		{
			strcpy (out, token);
		}

		token = strtok_r (NULL, BASKSEP, &saveptr);

		return 0;
	}

	return -1;
}

/*
	Function: parser_get_int (char* token, char* key, int* out, char* saveptr);
	Description: Parses a row and return the value if the key is right.
	InitVersion: 0.0.1
*/
static int parser_get_int (char* token, char* key, int* out, char* saveptr)
{
	if (strncmp (token, key, strlen (token)) == 0)
	{
		token = strtok_r (NULL, BASKSEP, &saveptr);

		if (isdigit (token[0]) != 0)
		{
			*out = atoi (token);
		}

		token = strtok_r (NULL, BASKSEP, &saveptr);

		return 0;
	}

	return -1;
}

/* |--------------------------------------------|
   |			Search			|
   |--------------------------------------------| */

/*
	Function: search_view_tasklist (bask_core* tcore, struct bask_task** first, char* searchtag);
	Description: Finds tasks with searchtag in the description and uses the view projects to display the results.
	InitVersion: 0.0.1
*/
static void search_view_tasklist (bask_core* tcore, struct bask_task** first, char* searchtag)
{
	struct bask_task* haystack = NULL;
	
	task_search (tcore, first, &haystack, searchtag);
	
	view_tasklist (tcore, &haystack);
	
	task_free_ll (&haystack);
}

/*
	Function: search_view_summary (bask_core* tcore, struct bask_task** first, char* searchtag);
	Description: Finds tasks with searchtag in the description and uses the view summary to display the results.
	InitVersion: 0.0.1
*/
static void search_view_summary (bask_core* tcore, struct bask_task** first, char* searchtag)
{
	struct bask_task* haystack = NULL;
	
	task_search (tcore, first, &haystack, searchtag);
	
	view_summary (tcore, &haystack);
	
	task_free_ll (&haystack);
}

/* |--------------------------------------------|
   |			Core			|
   |--------------------------------------------| */

/*
	Function: bask_init (bask_core* tcore, struct bask_task** first);
	Description: Inits Bask and loads the tasks from filename.
	InitVersion: 0.0.1
*/
static int bask_init (bask_core* tcore, struct bask_task** first)
{
	int i, tid, tactive, tpriority, tstate;
	char line[200], tproject[50], tdescription[200];
	char *token, *saveptr;

	i = 0;

	tcore->baskfile = fopen (BASKFILE, "r+");

	if (tcore->baskfile == NULL)
	{
		printf ("ERROR: Could'nt open the baskfile!\n");
		exit (EXIT_FAILURE);
	}

	while (fgets (line, sizeof (line)-1, tcore->baskfile) != NULL)
	{
		token = strtok_r (line, BASKSEP, &saveptr);
		
		if (strncmp (token, "END", strlen ("END")) == 0)
		{
			task_insert (first, i, tid, tactive, tpriority, tstate, tproject, tdescription);
			i++;
		}
		else
		{
			parser_get_int (token, "tid", &tid, saveptr);
			parser_get_int (token, "tactive", &tactive, saveptr);
			parser_get_int (token, "tstate", &tstate, saveptr);
			parser_get_int (token, "tpriority", &tpriority, saveptr);
			parser_get_str (token, "tproject", tproject, 50, saveptr);
			parser_get_str (token, "tdescription", tdescription, 200, saveptr);
		}
	}

	tcore->tc_amount = i;
	
	return 0;
}

/*
	Function: bask_write (bask_core* tcore, struct bask_task** first);
	Description: Writes all tasks to the baskfile.
	InitVersion: 0.0.1
*/
int bask_write (bask_core* tcore, struct bask_task** first)
{
	struct bask_task* ptr = *first;
		
	if (tcore->baskfile == NULL)
	{
		return -1;
	}
	
	rewind (tcore->baskfile);
	
	while (ptr != NULL)
	{
		fprintf (tcore->baskfile, "tid=%i;\n", ptr->t_id);
		fprintf (tcore->baskfile, "tactive=%i;\n", ptr->t_active);
		fprintf (tcore->baskfile, "tstate=%i;\n", ptr->t_state);
		fprintf (tcore->baskfile, "tpriority=%i;\n", ptr->t_priority);
		fprintf (tcore->baskfile, "tproject=%s;\n", ptr->t_project);
		fprintf (tcore->baskfile, "tdescription=%s;\n", ptr->t_description);
		fprintf (tcore->baskfile, "END\n");
		ptr = ptr->next;
	}
}

/*
	Function: bask_unload (bask_core* tcore);
	Description: Unloads the Bask application.
	InitVersion: 0.0.1
*/
static void bask_unload (bask_core* tcore)
{
	fclose (tcore->baskfile);
}

/* |--------------------------------------------|
   |			Prints			|
   |--------------------------------------------| */

/*
	Function: print_about (void);
	Description: Prints the about informations!
	InitVersion: 0.0.1
*/
static void print_about (void)
{
	printf ("Bask v. %s (c) 2015 Marc Volker Dickmann\n", P_VERSION);
	printf ("A simple taskmanager for real life written in C.\n");
}

/*
	Function: print_help (void);
	Description: Prints the help!
	InitVersion: 0.0.1
*/
static void print_help (void)
{
	printf ("%s :\n", P_CMD);
	printf ("\thelp\t\t\t\t\tThis help.\n");
	printf ("\tabout\t\t\t\t\tAbout the programm.\n");
	printf ("\tlist\t\t\t\t\tLists all tasks.\n");
	printf ("\tsummary\t\t\t\t\tSummary of all projects.\n");
	printf ("\tadd <priority> <project> <description>\tAdd a task.\n");
	
	printf ("\tmod <id>\t\t\t\tModifies all tasks!\n");
	printf ("\tfinish <id>\t\t\t\tSet the task to finished.\n");
	printf ("\tstop <id>\t\t\t\tDeactivates (hide) the task.\n");
	
	printf ("\tsearch <searchtag>\t\t\tSearches through the tasks.\n");
	printf ("\tsearch <view> <searchtag>\t\tSearches through the tasks and uses <view> to show the results.\n");
	
	printf ("\nARGUMENTS\n");
	printf ("\t-a [active]\t\tIf the task is active.\n");
	printf ("\t-s [state]\t\tThe state of the task.\n");
	printf ("\t-p [priority]\t\tThe priority of the task.\n");
	printf ("\t-P [PROJECT]\t\tThe projectname of the task.\n");
	printf ("\t-D [DESCRIPTION]\tThe description of the task.\n");
	
	printf ("\nVIEWS:\n");
	printf ("\ttasklist\t\tThe default view, a list of tasks.\n");
	printf ("\tsummary\t\t\tA summary of projects.\n");
}

/*
	Function: usage (void);
	Description: Prints the usage message!
	InitVersion: 0.0.1
*/
static void usage (void)
{
	printf ("Usage: %s help\n", P_CMD);
}

int main (int argc, char* argv[])
{
	int optc, ppri, pact, pstate;
	char pproject[50], pdescription[200];
	bask_core tcore;
	struct bask_task* first = NULL;
	
	ppri = pact = pstate = -1;
	
	strcpy (pproject, "");
	strcpy (pdescription, "");
	
	bask_init (&tcore, &first);
	
	while ((optc = getopt (argc, argv, "p:P:a:D:s:")) != -1)
	{
		switch (optc)
		{
			case 'p':
				ppri = atoi (optarg);
				break;
			case 'P':
				if (strlen (optarg) < sizeof (pproject))
				{
					strcpy (pproject, optarg);
				}
				break;
			case 'a':
				pact = atoi (optarg);
				break;
			case 'D':
				if (strlen (optarg) < sizeof (pdescription))
				{
					strcpy (pdescription, optarg);
				}
				break;
			case 's':
				pstate = atoi (optarg);
				break;
			default:
				break;
		}
	}
	
	if (argc <= 1)
	{
		usage ();
	}
	else if (optind > 1)
	{
		if (argc-optind == 2)
		{
			if (strncmp (argv[optind], "mod", strlen ("mod")) == 0)
			{
				task_modify (&tcore, &first, atoi (argv[optind+1]), pact, pstate, ppri, pproject, pdescription);
			}
			else
			{
				usage ();
			}
		}
		else
		{
			usage ();
		}
	}
	else if (argc == 2)
	{
		
		if (strncmp (argv[optind], "help", strlen ("help")) == 0)
		{
			print_help ();
		}
		else if (strncmp (argv[optind], "about", strlen ("about")) == 0)
		{
			print_about ();
		}
		else if (strncmp (argv[optind], "list", strlen ("list")) == 0)
		{
			view_tasklist (&tcore, &first);
		}
		else if (strncmp (argv[optind], "summary", strlen ("summary")) == 0)
		{
			view_summary (&tcore, &first);
		}
		else
		{
			usage ();
		}
	}
	else if (argc == 3)
	{
		if (strncmp (argv[optind], "finish", strlen ("finish")) == 0)
		{
			task_finish (&tcore, &first, atoi (argv[optind+1]));
		}
		else if (strncmp (argv[optind], "remove", strlen ("remove")) == 0)
		{
			task_remove (&tcore, &first, atoi (argv[optind+1]));
		}
		else if (strncmp (argv[optind], "search", strlen ("search")) == 0)
		{
			search_view_tasklist (&tcore, &first, argv[optind+1]);
		}
		else if (strncmp (argv[optind], "stop", strlen ("stop")) == 0)
		{
			task_deactivate (&tcore, &first, atoi (argv[optind+1]));
		}
		else
		{
			usage ();
		}
	}
	else if (argc == 4)
	{
		if (strncmp (argv[optind], "search", strlen ("search")) == 0)
		{
			if (strncmp (argv[optind+1], "tasklist", strlen ("tasklist")) == 0)
			{
				search_view_tasklist (&tcore, &first, argv[optind+2]);
			}
			else if (strncmp (argv[optind+1], "summary", strlen ("summary")) == 0)
			{
				search_view_summary (&tcore, &first, argv[optind+2]);
			}
			else
			{
				usage ();
			}
		}
		else
		{
			usage ();
		}
	}
	else if (argc == 5)
	{	
		if (strncmp (argv[optind], "add", strlen ("add")) == 0)
		{
			task_create (&tcore, &first, atoi (argv[optind+1]), argv[optind+2], argv[optind+3]);
		}
		else
		{
			usage ();
		}
	}
	else
	{
		usage ();
	}
	
	bask_unload (&tcore);
	task_free_ll (&first);
	return 0;
}