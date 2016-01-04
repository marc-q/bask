/* Copyright 2015 - 2016 Marc Volker Dickmann */
/* Project: Bask */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <getopt.h>
#include <sys/stat.h>
#include "lib/dutils.h"
#include "src/bask_core.h"
#include "src/bask_errors.h"
#include "src/bask_config.h"
#include "src/bask_task.h"
#include "src/bask_project.h"
#include "src/bask_filter.h"
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
	bask_filter bfilter;
	struct bask_task* haystack = NULL;
	
	filter_init (&bfilter, 1, -1, -1, -1);
	
	task_search (tcore, first, &haystack, searchtag);
	
	switch (view)
	{
		case BVIEW_TASKLIST:
			view_tasklist (tcore, btheme, &haystack, &bfilter);
			break;
		case BVIEW_SUMMARY:
			view_summary (tcore, btheme, &haystack, &bfilter);
			break;
		default:
			view_tasklist (tcore, btheme, &haystack, &bfilter);
			break;
	}
	
	task_free_ll (&haystack);
}

/* |--------------------------------------------|
   |			Core			|
   |--------------------------------------------| */

/*
	Function: bask_init_baskbin (bask_core* tcore);
	Description: Inits a baskbin at tcore->path_baskbin.
	InitVersion: 0.0.1
*/
static void bask_init_baskbin (bask_core* tcore)
{
	FILE* baskfile;
	
	if (bask_init_local_file (&baskfile, tcore->path_baskbin) == 0)
	{
		fprintf (baskfile, "BASKBIN\nbbuid=0\nBBEND\n");
		
		fclose (baskfile);
	}
}

/*
	Function: bask_init_basktheme (bask_core* tcore);
	Description: Inits a basktheme at tcore->path_basktheme.
	InitVersion: 0.0.1
*/
static void bask_init_basktheme (bask_core* tcore)
{
	FILE* baskfile;
	
	if (bask_init_local_file (&baskfile, tcore->path_basktheme) == 0)
	{	
		fprintf (baskfile, "color_normal=default\n");
		fprintf (baskfile, "color_important=default\n");
		fprintf (baskfile, "color_today=default\n");
		fprintf (baskfile, "color_critical=default\n");
		fprintf (baskfile, "color_finished=default\n");
		fprintf (baskfile, "color_pbarbak=default\n");
		fprintf (baskfile, "color_seclinesbak=default\n");
		
		fclose (baskfile);
	}
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

	config_init (tcore);
	bask_init_baskbin (tcore);
	bask_init_basktheme (tcore);
	
	return 0;
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
	printf ("Bask v. %s (c) 2015 - 2016 Marc Volker Dickmann\n", P_VERSION);
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
	printf ("\tlist <ARGS>\t\t\t\tLists all tasks.\n");
	printf ("\tsummary <ARGS>\t\t\t\tSummary of all projects.\n");
	printf ("\thistory <ARGS>\t\t\t\tHistory of monthly stats.\n");
	
	printf ("\tinit <FILE>\t\t\t\tInits the FILE or all files if no FILE is given.\n");
	printf ("\tadd [priority] [PROJECT] [DESCRIPTION]\tAdd a task.\n");
	printf ("\tremove [id]\t\t\t\tRemoves a task.\n");
	
	printf ("\tshow [id]\t\t\t\tShows informations about a single task.\n");
	printf ("\tmod [id] ARGS\t\t\t\tModifies a task.\n");
	printf ("\tfinish [id]\t\t\t\tSet's a task to finished.\n");
	printf ("\tstop [id]\t\t\t\tDeactivates (hides) a task.\n");
	printf ("\tconfig [CONFIGLINE]\t\t\tChanges the settings using a line from the config file.\n");
	
	printf ("\tsearch <VIEW> [SEARCHTAG]\t\tSearches through the tasks and optional uses <VIEW> to show the results.\n");
	printf ("\texport <EXPORT> [FILENAME]\t\tExports the data from the baskbin to an file with the format <EXPORT> or baskbin.\n");
	printf ("\timport <IMPORT> [FILENAME]\t\tImports the data from an file with the format <IMPORT> or baskbin to the baskbin.\n");
	printf ("\tdue [id] [DUEDATE]\t\tSet the due date of a task.\n");
	
	printf ("\nARGUMENTS\n");
	printf ("\t-a [active]\t\tIf the task is active.\n");
	/*printf ("\t-s [state]\t\tThe state of the task.\n");*/ /* We dont need this right now. */
	printf ("\t-p [priority]\t\tThe priority of the task.\n");
	printf ("\t-P [PROJECT]\t\tThe projectname of the task.\n");
	printf ("\t-D [DESCRIPTION]\tThe description of the task.\n");
	printf ("\t-F [FINISHED]\t\tThe finished date of the task.\n");
	printf ("\t-A [ADDED]\t\tThe added date of the task.\n");
	printf ("\t-f [ARGS]\t\tActivate the filtermode.\n");
	printf ("\t--due [DUEDATE]\t\tThe due date of the task.\n");
	
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
	
	printf ("\nFILTERS\n");
	printf ("\t-a, -p\t\t\tThese args can be used, look at ARGUMENTS for more info.\n");
	printf ("\t--month [month]\t\tUse only tasks if there month of the added date equals month.\n");
	
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
	int optc, ppri, pact, pstate, optindex, tmp;
	short filter, pmonth;
	char padded[T_S_ADDED], pdue[T_S_DUE], pfinished[T_S_FINISHED], pproject[T_S_PROJECT], pdescription[T_S_DESCRIPTION];
	bask_core tcore;
	bask_theme btheme;
	bask_filter bfilter;
	struct bask_task* first = NULL;
	
	struct option long_options[] = {
		 {"help", no_argument, 0, B_CMD_ARG_HELP},
		 {"about", no_argument, 0, B_CMD_ARG_ABOUT},
		 {"month", required_argument, 0, B_CMD_ARG_MONTH},
		 {"due", required_argument, 0, B_CMD_ARG_DUE},
		 {0,0,0,0}
	};
	
	tcore.flags = optindex = tmp = 0;
	ppri = pact = pstate = pmonth = filter = -1;
	
	strcpy (padded, "");
	strcpy (pdue, "");
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
	
	config_init (&tcore);
	
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
				/* Saving the default values and exit. */
				config_save (&tcore);
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
	
	config_load (&tcore);
	ui_theme_load (&tcore, &btheme);
	
	while ((optc = getopt_long (argc, argv, "p:P:a:D:s:F:A:fh", long_options, &optindex)) != -1)
	{
		switch (optc)
		{
			case 'p':
				ppri = task_get_priority (optarg);
				break;
			case 'P':
				if (strlen (optarg) < sizeof (pproject))
				{
					strcpy (pproject, optarg);
				}
				break;
			case 'a':
				if (isdigit (optarg[0]) != 0)
				{
					pact = atoi (optarg);
				}
				break;
			case 'D':
				if (strlen (optarg) < sizeof (pdescription))
				{
					strcpy (pdescription, optarg);
				}
				break;
			case 's':
				if (isdigit (optarg[0]) != 0)
				{
					pstate = atoi (optarg);
				}
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
			case 'f':
				filter = 1;
				break;
			case B_CMD_ARG_HELP:
				print_help ();
				exit (EXIT_SUCCESS);
				break;
			case B_CMD_ARG_ABOUT:
				print_about ();
				exit (EXIT_SUCCESS);
				break;
			case B_CMD_ARG_MONTH:
				if (isdigit (optarg[0]) != 0)
				{
					pmonth = (short) atoi (optarg);
				}
				break;
			case B_CMD_ARG_DUE:
				if (strlen (optarg) < sizeof (pdue))
				{
					strcpy (pdue, optarg);
				}
				break;
			default:
				break;
		}
	}
	
	if (filter == 1)
	{
		filter_init (&bfilter, (short) pact, -1, (short) ppri, pmonth);
	}
	else
	{
		filter_init (&bfilter, 1, -1, -1, -1);
	}
	
	bask_init (&tcore, &first);
	
	if (optind > 1)
	{
		if (argc-optind == 1)
		{
			if (utils_streq (argv[optind], "list") == 0)
			{
				view_tasklist (&tcore, &btheme, &first, &bfilter);
			}
			else if (utils_streq (argv[optind], "summary") == 0)
			{
				view_summary (&tcore, &btheme, &first, &bfilter);
			}
			else if (utils_streq (argv[optind], "history") == 0)
			{
				view_history (&tcore, &btheme, &first, &bfilter);
			}
			else
			{
				usage ();
			}
		}
		else if (argc-optind == 2)
		{
			if (utils_streq (argv[optind], "mod") == 0)
			{
				task_modificate_cmd (&tcore, &first, atoi (argv[optind+1]), pact, pstate, ppri, padded, pdue, pfinished, pproject, pdescription);
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
			view_tasklist (&tcore, &btheme, &first, &bfilter);
		}
		else if (utils_streq (argv[optind], "summary") == 0)
		{
			view_summary (&tcore, &btheme, &first, &bfilter);
		}
		else if (utils_streq (argv[optind], "history") == 0)
		{
			view_history (&tcore, &btheme, &first, &bfilter);
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
		else if (utils_streq (argv[optind], "config") == 0)
		{
			tmp = config_set_str (&tcore, argv[optind+1]);
			
			if (tmp != 0)
			{
				config_print_set_str_errors (tmp);
			}
			else
			{
				printf ("Config succefully changed.\n");
			}
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
		else if (utils_streq (argv[optind], "due") == 0)
		{
			task_due_cmd (&tcore, &first, atoi (argv[optind+1]), argv[optind+2]);
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
			task_create_cmd (&tcore, &first, task_get_priority (argv[optind+1]), argv[optind+2], argv[optind+3]);
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
	
	config_save (&tcore);
	bask_unload (&first);
	return 0;
}
