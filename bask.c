/* Copyright 2015 Marc Volker Dickmann */
/* Project: Bask */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include <sys/stat.h>
#include "lib/dutils.h"
#include "src/bask_core.h"
#include "src/bask_errors.h"
#include "src/bask_task.h"
#include "src/bask_project.h"
#include "src/bask_ui.h"
#include "src/bask_views.h"
#include "src/bask_export.h"
#include "bask.h"

/* |--------------------------------------------|
   |			Search			|
   |--------------------------------------------| */

/*
	Function: search_view_tasklist (bask_core* tcore, bask_theme* btheme, struct bask_task** first, char* searchtag);
	Description: Finds tasks with searchtag in the description and uses the view tasklist to display the results.
	InitVersion: 0.0.1
*/
static void search_view_tasklist (bask_core* tcore, bask_theme* btheme, struct bask_task** first, char* searchtag)
{
	struct bask_task* haystack = NULL;
	
	task_search (tcore, first, &haystack, searchtag);
	
	view_tasklist (tcore, btheme, &haystack);
	
	task_free_ll (&haystack);
}

/*
	Function: search_view_summary (bask_core* tcore, bask_theme* btheme, struct bask_task** first, char* searchtag);
	Description: Finds tasks with searchtag in the description and uses the view summary to display the results.
	InitVersion: 0.0.1
*/
static void search_view_summary (bask_core* tcore, bask_theme* btheme, struct bask_task** first, char* searchtag)
{
	struct bask_task* haystack = NULL;
	
	task_search (tcore, first, &haystack, searchtag);
	
	view_summary (tcore, btheme, &haystack);
	
	task_free_ll (&haystack);
}

/* |--------------------------------------------|
   |			Core			|
   |--------------------------------------------| */

/*
	Function: bask_get_baskpath (bask_core* tcore, char* out, char* filename);
	Description: Set the out to the path of filename file in the bask dir.
	InitVersion: 0.0.1
*/
static void bask_get_baskpath (bask_core* tcore, char* out, char* filename)
{
	strcpy (out, tcore->path_baskpath);

	strcat (out, filename);
}

/*
	Function: bask_init_local_file (char* filename, char* content);
	Description: Creates a file if not exist filled the content.
	InitVersion: 0.0.1
*/
static int bask_init_local_file (char* filename, char* content)
{
	FILE *baskfile;
	
	baskfile = fopen (filename, "w+");
	
	if (baskfile == NULL)
	{
		errors_filenotwritten (filename);
		return -1;
	}
	
	if (content != NULL)
	{
		fprintf (baskfile, "%s", content);
	}
	
	fclose (baskfile);
	
	return 0;
}

/*
	Function: bask_init_local (bask_core* tcore);
	Description: Inits Bask local, creates the needed files.
	InitVersion: 0.0.1
*/
static int bask_init_local (bask_core* tcore)
{
	struct stat sb;

	if (stat (tcore->path_baskpath, &sb) == -1)
	{
		mkdir (tcore->path_baskpath, 0700);
		bask_init_local (tcore);
		return 1;
	}

	bask_init_local_file (tcore->path_baskconf, "baskbin=default;");
	bask_init_local_file (tcore->path_baskbin, "");
	bask_init_local_file (tcore->path_basktheme, "color_normal=default;\ncolor_important=default;\ncolor_today=default;\ncolor_critical=default;\ncolor_finished=default;\ncolor_pbarbak=default;");
	
	return 0;
}

/*
	Function: bask_load_conf (bask_core* tcore);
	Description: Loads the config file from bask.
	InitVersion: 0.0.1
*/
static void bask_load_conf (bask_core* tcore)
{
	char line[200], baskbin[151];
	char *token, *saveptr;
	FILE *baskconf;
	
	baskconf = fopen (tcore->path_baskconf, "r");
	
	if (baskconf == NULL)
	{
		errors_filenotopened (tcore->path_baskconf);
		exit (EXIT_FAILURE);
	}
	
	while (fgets (line, sizeof (line), baskconf) != NULL)
	{
		token = strtok_r (line, BASKSEP, &saveptr);
		
		parser_get_str (token, "baskbin", baskbin, 150, saveptr);
	}
	
	if (strncmp (baskbin, "default", strlen(baskbin)) != 0)
	{
		strcpy (tcore->path_baskbin, baskbin);
	}
	
	fclose (baskconf);
}

/*
	Function: bask_init (bask_core* tcore, struct bask_task** first);
	Description: Inits Bask and loads the tasks from filename.
	InitVersion: 0.0.1
*/
static void bask_init (bask_core* tcore, struct bask_task** first)
{
	int i, tid, tactive, tpriority, tstate, bb_state;
	char line[200], tfinished[T_S_FINISHED], tproject[T_S_PROJECT], tdescription[T_S_DESCRIPTION];
	char *token, *saveptr;
	FILE* baskfile;

	i = bb_state = tid = tactive = tpriority = tstate = 0;

	baskfile = fopen (tcore->path_baskbin, "r");

	if (baskfile == NULL)
	{
		errors_filenotopened (tcore->path_baskbin);
		exit (EXIT_FAILURE);
	}
	
	while (fgets (line, sizeof (line), baskfile) != NULL)
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
			task_insert (first, i, tid, tactive, tpriority, tstate, tfinished, tproject, tdescription);
			i++;
		}
		else
		{
			parser_get_int (token, "tid", &tid, saveptr);
			parser_get_int (token, "tactive", &tactive, saveptr);
			parser_get_int (token, "tstate", &tstate, saveptr);
			parser_get_int (token, "tpriority", &tpriority, saveptr);
			parser_get_str (token, "tfinished", tfinished, sizeof (tfinished), saveptr);
			parser_get_str (token, "tproject", tproject, sizeof (tproject), saveptr);
			parser_get_str (token, "tdescription", tdescription, sizeof (tdescription), saveptr);
		}
	}

	tcore->tc_amount = i;
	
	fclose (baskfile);
}

/*
	Function: bask_write (bask_core* tcore, struct bask_task** first);
	Description: Writes all tasks to the baskfile.
	InitVersion: 0.0.1
*/
int bask_write (bask_core* tcore, struct bask_task** first)
{
	FILE* baskfile;
	struct bask_task* ptr = *first;
	
	baskfile = fopen (tcore->path_baskbin, "w");

	if (baskfile == NULL)
	{
		errors_filenotopened (tcore->path_baskbin);
		/* Note: At this point, were already have data in the linked list.
			 So we must use this to free the allocated memory. */
		return -1;
	}
	
	fprintf (baskfile, "BASKBIN\n");
	fprintf (baskfile, "bbuid=%i;\n", tcore->baskbin_uid);
	fprintf (baskfile, "BBEND\n");
	
	while (ptr != NULL)
	{
		fprintf (baskfile, "tid=%i;\n", ptr->t_id);
		fprintf (baskfile, "tactive=%i;\n", ptr->t_active);
		fprintf (baskfile, "tstate=%i;\n", ptr->t_state);
		fprintf (baskfile, "tpriority=%i;\n", ptr->t_priority);
		fprintf (baskfile, "tfinished=%s;\n", ptr->t_finished);
		fprintf (baskfile, "tproject=%s;\n", ptr->t_project);
		fprintf (baskfile, "tdescription=%s;\n", ptr->t_description);
		fprintf (baskfile, "END\n");
		ptr = ptr->next;
	}
	
	fclose (baskfile);
	
	return 0;
}

/*
	Function: bask_unload (struct bask_task** first);
	Description: Unloads the Bask application.
	InitVersion: 0.0.1
*/
static void bask_unload (struct bask_task** first)
{
	task_free_ll (first);
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
	printf ("Licence: %s\n", P_LICENCE);
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
	
	printf ("\tadd [priority] [PROJECT] [DESCRIPTION]\tAdd a task.\n");
	printf ("\tremove [id]\t\t\t\tRemoves the task with id [id].\n");
	
	printf ("\tshow [id]\t\t\t\tShows informations about a single task.\n");
	printf ("\tmod [id] ARGS\t\t\t\tModifies all tasks!\n");
	printf ("\tfinish [id]\t\t\t\tSet the task to finished.\n");
	printf ("\tstop [id]\t\t\t\tDeactivates (hide) the task.\n");
	
	printf ("\tsearch <VIEW> [SEARCHTAG]\t\tSearches through the tasks and optional uses <VIEW> to show the results.\n");
	
	printf ("\nARGUMENTS\n");
	printf ("\t-a [active]\t\tIf the task is active.\n");
	printf ("\t-s [state]\t\tThe state of the task.\n");
	printf ("\t-p [priority]\t\tThe priority of the task.\n");
	printf ("\t-P [PROJECT]\t\tThe projectname of the task.\n");
	printf ("\t-D [DESCRIPTION]\tThe description of the task.\n");
	
	printf ("\nVIEWS\n");
	printf ("\ttasklist\t\tThe default view, a list of tasks.\n");
	printf ("\tsummary\t\t\tA summary of projects.\n");
	
	printf ("\nPRIORITYS\n");
	printf ("\t0\tL\tNormal\n");
	printf ("\t1\tI\tImportant\n");
	printf ("\t2\tT\tToday\n");
	printf ("\t3\tC\tCritical\n");
	
	printf ("\nLEGEND: <optional> [necessary] [integer] [STRING]\n");
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
	char pproject[T_S_PROJECT], pdescription[T_S_DESCRIPTION];
	bask_core tcore;
	bask_theme btheme;
	struct bask_task* first = NULL;
	
	ppri = pact = pstate = -1;
	
	strcpy (pproject, "");
	strcpy (pdescription, "");
	
	strcpy (tcore.path_baskpath, getenv("HOME"));
	
	if (tcore.path_baskpath == NULL || *tcore.path_baskpath == '\0')
	{
		errors_homedirnotgot ();
		exit (EXIT_FAILURE);
	}

	strcat (tcore.path_baskpath, "/.local/share/bask/");
	
	bask_get_baskpath (&tcore, tcore.path_baskconf, BASKCONFFILE);
	bask_get_baskpath (&tcore, tcore.path_basktheme, BASKTHEMEFILE);
	bask_get_baskpath (&tcore, tcore.path_baskbin, BASKBINFILE);
	
	if (argc == 2)
	{
		if (strncmp (argv[optind], "init", strlen ("init")) == 0)
		{
			bask_init_local (&tcore);
			exit (EXIT_SUCCESS);
		}
	}
	
	bask_load_conf (&tcore);
	bask_init (&tcore, &first);
	view_theme_load (&tcore, &btheme);
	
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
			view_tasklist (&tcore, &btheme, &first);
		}
		else if (strncmp (argv[optind], "summary", strlen ("summary")) == 0)
		{
			view_summary (&tcore, &btheme, &first);
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
			search_view_tasklist (&tcore, &btheme, &first, argv[optind+1]);
		}
		else if (strncmp (argv[optind], "stop", strlen ("stop")) == 0)
		{
			task_deactivate (&tcore, &first, atoi (argv[optind+1]));
		}
		else if (strncmp (argv[optind], "show", strlen ("show")) == 0)
		{
			view_single (&tcore, &first, atoi (argv[optind+1]));
		}
		else if (strncmp (argv[optind], "export", strlen ("export")) == 0)
		{
			export_web (&tcore, &first, argv[optind+1]);
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
				search_view_tasklist (&tcore, &btheme, &first, argv[optind+2]);
			}
			else if (strncmp (argv[optind+1], "summary", strlen ("summary")) == 0)
			{
				search_view_summary (&tcore, &btheme, &first, argv[optind+2]);
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
	
	bask_unload (&first);
	return 0;
}
