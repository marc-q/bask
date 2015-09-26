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
#include "src/bask_import.h"
#include "bask.h"

/* |--------------------------------------------|
   |			Search			|
   |--------------------------------------------| */

/*
	Function: search_view (bask_core* tcore, bask_theme* btheme, struct bask_task** first, char* searchtag, int view);
	Description: Finds tasks with searchtag in the description, project or finished and uses the view <view> to display the results.
	InitVersion: 0.0.1
*/
static void search_view (bask_core* tcore, bask_theme* btheme, struct bask_task** first, char* searchtag, int view)
{
	struct bask_task* haystack = NULL;
	
	task_search (tcore, first, &haystack, searchtag);
	
	switch (view)
	{
		case BVIEW_TASKLIST:
			view_tasklist (tcore, btheme, &haystack);
			break;
		case BVIEW_SUMMARY:
			view_summary (tcore, btheme, &haystack);
			break;
		default:
			view_tasklist (tcore, btheme, &haystack);
			break;
	}
	
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
	bask_init_local_file (tcore->path_basktheme, "color_normal=default;\ncolor_important=default;\ncolor_today=default;\ncolor_critical=default;\ncolor_finished=default;\ncolor_pbarbak=default;\ncolor_seclinesbak=default;");
	
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
	tcore->tc_amount = 0;
	
	if (import_baskbin (tcore, first, tcore->path_baskbin) != 0)
	{
		exit (EXIT_FAILURE);
	}
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
	printf ("\texport <EXPORT> [FILENAME]\t\tExports the data from baskbin to an file with the format <EXPORT> or baskbin.\n");
	printf ("\timport <IMPORT> [FILENAME]\t\tImports the data from an file with the format <EXPORT> or baskbin to the baskbin.\n");
	
	printf ("\nARGUMENTS\n");
	printf ("\t-a [active]\t\tIf the task is active.\n");
	printf ("\t-s [state]\t\tThe state of the task.\n");
	printf ("\t-p [priority]\t\tThe priority of the task.\n");
	printf ("\t-P [PROJECT]\t\tThe projectname of the task.\n");
	printf ("\t-D [DESCRIPTION]\tThe description of the task.\n");
	
	printf ("\nVIEWS\n");
	printf ("\ttasklist\t\tThe default view, a list of tasks.\n");
	printf ("\tsummary\t\t\tA summary of projects.\n");
	
	printf ("\nEXPORTS\n");
	printf ("\tbaskbin\t\t\tExports to an baskbin file. (default)\n");
	printf ("\tcsv\t\t\tExports to an csv file.\n");
	
	printf ("\nIMPORTS\n");
	printf ("\tbaskbin\t\t\tImports from an baskbin file. (default)\n");
	printf ("\tcsv\t\t\tImports from an csv file.\n");
	
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
	view_theme_load (&tcore, &btheme);
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
			search_view (&tcore, &btheme, &first, argv[optind+1], BVIEW_TASKLIST);
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
			export_baskbin (&tcore, &first, argv[optind+1]);
		}
		else if (strncmp (argv[optind], "import", strlen ("import")) == 0)
		{
			import_baskbin_cmd (&tcore, &first, argv[optind+1]);
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
				search_view (&tcore, &btheme, &first, argv[optind+2], BVIEW_TASKLIST);
			}
			else if (strncmp (argv[optind+1], "summary", strlen ("summary")) == 0)
			{
				search_view (&tcore, &btheme, &first, argv[optind+2], BVIEW_SUMMARY);
			}
			else
			{
				usage ();
			}
		}
		else if (strncmp (argv[optind], "export", strlen ("export")) == 0)
		{
			if (strncmp (argv[optind+1], "baskbin", strlen ("baskbin")) == 0)
			{
				export_baskbin (&tcore, &first, argv[optind+2]);
			}
			else if (strncmp (argv[optind+1], "csv", strlen ("csv")) == 0)
			{
				export_csv (&tcore, &first, argv[optind+2]);
			}
			else if (strncmp (argv[optind+1], "web", strlen ("web")) == 0)
			{
				export_web (&tcore, &first, argv[optind+2]);
			}
			else
			{
				usage ();
			}
		}
		else if (strncmp (argv[optind], "import", strlen ("import")) == 0)
		{
			if (strncmp (argv[optind+1], "baskbin", strlen ("baskbin")) == 0)
			{
				import_baskbin_cmd (&tcore, &first, argv[optind+2]);
			}
			else if (strncmp (argv[optind+1], "csv", strlen ("csv")) == 0)
			{
				import_csv_cmd (&tcore, &first, argv[optind+2]);
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
