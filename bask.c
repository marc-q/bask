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
	Function: bask_init_baskconf (bask_core* tcore);
	Description: Inits a config file at tcore->path_baskconf.
	InitVersion: 0.0.1
*/
static void bask_init_baskconf (bask_core* tcore)
{
	bask_init_local_file (tcore->path_baskconf, "# Path to the baskbin.\nbaskbin=default;\n# The maximum length of descriptions (0-200; default: 50;)\ntask_description_max=50;\n# Should longer lines be broken when viewed? (0/1; default: 1)\ntask_description_break=1;\n");
}

/*
	Function: bask_init_baskbin (bask_core* tcore);
	Description: Inits a baskbin at tcore->path_baskbin.
	InitVersion: 0.0.1
*/
static void bask_init_baskbin (bask_core* tcore)
{
	bask_init_local_file (tcore->path_baskbin, "BASKBIN\nbbuid=0;\nBBEND");
}

/*
	Function: bask_init_basktheme (bask_core* tcore);
	Description: Inits a basktheme at tcore->path_basktheme.
	InitVersion: 0.0.1
*/
static void bask_init_basktheme (bask_core* tcore)
{
	bask_init_local_file (tcore->path_basktheme, "color_normal=default;\ncolor_important=default;\ncolor_today=default;\ncolor_critical=default;\ncolor_finished=default;\ncolor_pbarbak=default;\ncolor_seclinesbak=default;");
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

	bask_init_baskconf (tcore);
	bask_init_baskbin (tcore);
	bask_init_basktheme (tcore);
	
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
	
	tcore->t_descriptionmax = 50;
	
	baskconf = fopen (tcore->path_baskconf, "r");
	
	if (baskconf == NULL)
	{
		errors_filenotopened (tcore->path_baskconf);
		errors_useinit ();
		exit (EXIT_FAILURE);
	}
	
	while (fgets (line, sizeof (line), baskconf) != NULL)
	{
		if (line[0] != '#')
		{
			token = strtok_r (line, BASKSEP, &saveptr);
		
			parser_get_str (token, "baskbin", baskbin, sizeof (baskbin), BASKSEP, saveptr);
			parser_get_int (token, "task_description_max", &tcore->t_descriptionmax, BASKSEP, saveptr);
			parser_get_int (token, "task_description_break", &tcore->t_descriptionbreak, BASKSEP, saveptr);
		}
	}
	
	fclose (baskconf);
	
	if (utils_streq (baskbin, "default") != 0)
	{
		strcpy (tcore->path_baskbin, baskbin);
	}
}

/*
	Function: bask_init (bask_core* tcore, struct bask_task** first);
	Description: Inits Bask and loads the tasks from the baskbin.
	InitVersion: 0.0.1
*/
static void bask_init (bask_core* tcore, struct bask_task** first)
{
	tcore->tc_amount = 0;
	
	if (import_baskbin (tcore, first, tcore->path_baskbin) == -1)
	{
		errors_useinit ();
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
	
	printf ("\tinit <FILE>\t\t\t\tInits the FILE or all files if no FILE is given.\n");
	printf ("\tadd [priority] [PROJECT] [DESCRIPTION]\tAdd a task.\n");
	printf ("\tremove [id]\t\t\t\tRemoves a task.\n");
	
	printf ("\tshow [id]\t\t\t\tShows informations about a single task.\n");
	printf ("\tmod [id] ARGS\t\t\t\tModifies a task.\n");
	printf ("\tfinish [id]\t\t\t\tSet's a task to finished.\n");
	printf ("\tstop [id]\t\t\t\tDeactivates (hides) a task.\n");
	
	printf ("\tsearch <VIEW> [SEARCHTAG]\t\tSearches through the tasks and optional uses <VIEW> to show the results.\n");
	printf ("\texport <EXPORT> [FILENAME]\t\tExports the data from the baskbin to an file with the format <EXPORT> or baskbin.\n");
	printf ("\timport <IMPORT> [FILENAME]\t\tImports the data from an file with the format <IMPORT> or baskbin to the baskbin.\n");
	
	printf ("\nARGUMENTS\n");
	printf ("\t-a [active]\t\tIf the task is active.\n");
	printf ("\t-s [state]\t\tThe state of the task.\n");
	printf ("\t-p [priority]\t\tThe priority of the task.\n");
	printf ("\t-P [PROJECT]\t\tThe projectname of the task.\n");
	printf ("\t-D [DESCRIPTION]\tThe description of the task.\n");
	printf ("\t-F [FINISHED]\t\tThe finished date of the task.\n");
	printf ("\t-A [ADDED]\t\tThe added date of the task.\n");
	
	printf ("\nVIEWS\n");
	printf ("\ttasklist\t\tThe default view, a list of tasks.\n");
	printf ("\tsummary\t\t\tA summary of projects.\n");
	
	printf ("\nEXPORTS\n");
	printf ("\tbaskbin\t\t\tExports to an baskbin file. (default)\n");
	printf ("\tcsv\t\t\tExports to an csv file.\n");
	printf ("\tical\t\t\tExports to an iCalendar file.\n");
	
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
	char padded[T_S_ADDED], pfinished[T_S_FINISHED], pproject[T_S_PROJECT], pdescription[T_S_DESCRIPTION];
	bask_core tcore;
	bask_theme btheme;
	struct bask_task* first = NULL;
	
	ppri = pact = pstate = -1;
	
	strcpy (padded, "");
	strcpy (pfinished, "");
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
	
	/* NOTE: These are cmd's that don't need the tasks data, so it wont be loaded. */
	if (argc <= 1)
	{
		usage ();
		exit (EXIT_FAILURE);
	}
	else if (argc == 2)
	{
		if (utils_streq (argv[optind], "help") == 0)
		{
			print_help ();
			exit (EXIT_SUCCESS);
		}
		else if (utils_streq (argv[optind], "about") == 0)
		{
			print_about ();
			exit (EXIT_SUCCESS);
		}
		else if (utils_streq (argv[optind], "init") == 0)
		{
			bask_init_local (&tcore);
			exit (EXIT_SUCCESS);
		}
	}
	else if (argc == 3)
	{
		if (utils_streq (argv[optind], "init") == 0)
		{
			if (utils_streq (argv[optind+1], "baskconf") == 0)
			{
				bask_init_baskconf (&tcore);
				exit (EXIT_SUCCESS);
			}
			else if (utils_streq (argv[optind+1], "baskbin") == 0)
			{
				bask_init_baskbin (&tcore);
				exit (EXIT_SUCCESS);
			}
			else if (utils_streq (argv[optind+1], "basktheme") == 0)
			{
				bask_init_basktheme (&tcore);
				exit (EXIT_SUCCESS);
			}
		}
	}
	
	bask_load_conf (&tcore);
	ui_theme_load (&tcore, &btheme);
	bask_init (&tcore, &first);
	
	while ((optc = getopt (argc, argv, "p:P:a:D:s:F:A:")) != -1)
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
			case 'F':
				if (strlen (optarg) < sizeof (pfinished))
				{
					strcpy (pfinished, optarg);
				}
				break;
			case 'A':
				if (strlen (optarg) < sizeof (padded))
				{
					strcpy (padded, optarg);
				}
				break;
			default:
				break;
		}
	}
	
	if (optind > 1)
	{
		if (argc-optind == 2)
		{
			if (utils_streq (argv[optind], "mod") == 0)
			{
				task_modificate_cmd (&tcore, &first, atoi (argv[optind+1]), pact, pstate, ppri, padded, pfinished, pproject, pdescription);
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
		if (utils_streq (argv[optind], "list") == 0)
		{
			view_tasklist (&tcore, &btheme, &first);
		}
		else if (utils_streq (argv[optind], "summary") == 0)
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
		if (utils_streq (argv[optind], "finish") == 0)
		{
			task_finish_cmd (&tcore, &first, atoi (argv[optind+1]));
		}
		else if (utils_streq (argv[optind], "remove") == 0)
		{
			task_remove_cmd (&tcore, &first, atoi (argv[optind+1]));
		}
		else if (utils_streq (argv[optind], "search") == 0)
		{
			search_view (&tcore, &btheme, &first, argv[optind+1], BVIEW_TASKLIST);
		}
		else if (utils_streq (argv[optind], "stop") == 0)
		{
			task_deactivate_cmd (&tcore, &first, atoi (argv[optind+1]));
		}
		else if (utils_streq (argv[optind], "show") == 0)
		{
			view_single (&tcore, &first, atoi (argv[optind+1]));
		}
		else if (utils_streq (argv[optind], "export") == 0)
		{
			export_baskbin (&tcore, &first, argv[optind+1]);
		}
		else if (utils_streq (argv[optind], "import") == 0)
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
		if (utils_streq (argv[optind], "search") == 0)
		{
			if (utils_streq (argv[optind+1], "tasklist") == 0)
			{
				search_view (&tcore, &btheme, &first, argv[optind+2], BVIEW_TASKLIST);
			}
			else if (utils_streq (argv[optind+1], "summary") == 0)
			{
				search_view (&tcore, &btheme, &first, argv[optind+2], BVIEW_SUMMARY);
			}
			else
			{
				usage ();
			}
		}
		else if (utils_streq (argv[optind], "export") == 0)
		{
			if (utils_streq (argv[optind+1], "baskbin") == 0)
			{
				export_baskbin (&tcore, &first, argv[optind+2]);
			}
			else if (utils_streq (argv[optind+1], "csv") == 0)
			{
				export_csv (&tcore, &first, argv[optind+2]);
			}
			else if (utils_streq (argv[optind+1], "ical") == 0)
			{
				export_ical (&tcore, &first, argv[optind+2]);
			}
			else if (utils_streq (argv[optind+1], "web") == 0)
			{
				export_web (&tcore, &first, argv[optind+2]);
			}
			else
			{
				usage ();
			}
		}
		else if (utils_streq (argv[optind], "import") == 0)
		{
			if (utils_streq (argv[optind+1], "baskbin") == 0)
			{
				import_baskbin_cmd (&tcore, &first, argv[optind+2]);
			}
			else if (utils_streq (argv[optind+1], "csv") == 0)
			{
				import_csv_cmd (&tcore, &first, argv[optind+2]);
			}
			else if (utils_streq (argv[optind+1], "ical") == 0)
			{
				import_ical_cmd (&tcore, &first, argv[optind+2]);
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
		if (utils_streq (argv[optind], "add") == 0)
		{
			task_create_cmd (&tcore, &first, atoi (argv[optind+1]), argv[optind+2], argv[optind+3]);
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
