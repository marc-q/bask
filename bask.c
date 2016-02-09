/* Copyright 2015 - 2016 Marc Volker Dickmann */
/* Project: Bask */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <getopt.h>
#include <sys/stat.h>
#include "lib/dutils.h"
#include "src/bask_core.h"
#include "src/bask_time.h"
#include "src/bask_errors.h"
#include "src/bask_config.h"
#include "src/bask_task.h"
#include "src/bask_project.h"
#include "src/bask_filter.h"
#include "src/bask_ui.h"
#include "src/bask_priority.h"
#include "src/bask_views.h"
#include "src/bask_export.h"
#include "src/bask_import.h"
#include "bask.h"

/* |--------------------------------------------|
   |			Search			|
   |--------------------------------------------| */

/*
	Function: search_view (bask_core* tcore, bask_theme* btheme, bask_priority** bprioritys, bask_filter* bfilter, struct bask_task** first, char* searchtag, short view);
	Description: Finds tasks with searchtag in the description, project or finished and uses the view <view> to display the results.
	InitVersion: 0.0.1
*/
static void search_view (bask_core* tcore, bask_theme* btheme, bask_priority** bprioritys, bask_filter* bfilter, struct bask_task** first, char* searchtag, short view)
{
	struct bask_task* haystack = NULL;
	
	task_search (tcore, first, &haystack, searchtag);
	
	switch (view)
	{
		case BVIEW_TASKLIST:
			view_tasklist (tcore, btheme, bprioritys, &haystack, bfilter);
			break;
		case BVIEW_SUMMARY:
			view_summary (tcore, btheme, &haystack, bfilter);
			break;
		case BVIEW_DETAILED:
			view_single (tcore, &haystack, bfilter, 0, TRUE);
			break;
		default:
			view_tasklist (tcore, btheme, bprioritys, &haystack, bfilter);
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
static short bask_init_local (bask_core* tcore)
{
	struct stat sb;

	if (stat (tcore->path_baskpath, &sb) == -1)
	{
		mkdir (tcore->path_baskpath, 0700);
		bask_init_local (tcore);
		return -1;
	}

	config_save (tcore);
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
	Function: bask_unload (bask_priority** bprioritys, struct bask_task** first);
	Description: Unloads the Bask application.
	InitVersion: 0.0.1
*/
static void bask_unload (bask_priority** bprioritys, struct bask_task** first)
{
	priority_free_ll (bprioritys);
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
	printf ("\t%s\t\t\t\t\tThis help.\n", B_CMD_HELP);
	printf ("\t%s\t\t\t\t\tAbout the programm.\n", B_CMD_ABOUT);
	printf ("\t%s <ARGS>\t\t\t\tLists all tasks.\n", B_CMD_LIST);
	printf ("\t%s <ARGS>\t\t\t\tSummary of all projects.\n", B_CMD_SUMMARY);
	printf ("\t%s <ARGS>\t\t\t\tHistory of monthly stats.\n", B_CMD_HISTORY);
	
	printf ("\t%s <FILE>\t\t\t\tInits the FILE or all files if no FILE is given.\n", B_CMD_INIT);
	printf ("\t%s [priority] [PROJECT] [DESCRIPTION]\tAdd a task.\n", B_CMD_ADD);
	printf ("\t%s [id]\t\t\t\tRemoves a task.\n", B_CMD_REMOVE);
	
	printf ("\t%s [id]\t\t\t\tShows informations about a single task.\n", B_CMD_SHOW);
	printf ("\t%s [id] ARGS\t\t\t\tModifies a task.\n", B_CMD_MOD);
	printf ("\t%s [id]\t\t\t\tSet's a task to finished.\n", B_CMD_FINISH);
	printf ("\t%s [id]\t\t\t\tDeactivates (hides) a task.\n", B_CMD_STOP);
	printf ("\t%s [CONFIGLINE]\t\t\tChanges the settings using a line from the config file.\n", B_CMD_CONFIG);
	
	printf ("\t%s <VIEW> [SEARCHTAG] <ARGS>\tSearches through the tasks and optional uses <VIEW> to show the results.\n", B_CMD_SEARCH);
	printf ("\t%s <EXPORT> [FILENAME]\t\tExports the data from the baskbin to an file with the format <EXPORT> or baskbin.\n", B_CMD_EXPORT);
	printf ("\t%s <IMPORT> [FILENAME]\t\tImports the data from an file with the format <IMPORT> or baskbin to the baskbin.\n", B_CMD_IMPORT);
	printf ("\t%s [id] [DUEDATE]\t\t\tSet the due date of a task.\n", B_CMD_DUE);
	
	printf ("\nARGUMENTS\n");
	printf ("\t-a [active]\t\tIf the task is active.\n");
	printf ("\t-p [priority]\t\tThe priority of the task.\n");
	printf ("\t-P [PROJECT]\t\tThe projectname of the task.\n");
	printf ("\t-D [DESCRIPTION]\tThe description of the task.\n");
	printf ("\t-F [FINISHED]\t\tThe finished date of the task.\n");
	printf ("\t-A [ADDED]\t\tThe added date of the task.\n");
	printf ("\t-f [ARGS]\t\tActivate the filtermode.\n");
	printf ("\t--due [DUEDATE]\t\tThe due date of the task.\n");
	
	printf ("\nVIEWS\n");
	printf ("\t%s\t\tThe default view, a list of tasks.\n", B_CMD_TASKLIST);
	printf ("\t%s\t\t\tA summary of projects.\n", B_CMD_SUMMARY);
	printf ("\t%s\t\tDetailed informations about each task.\n", B_CMD_DETAILED);
	
	printf ("\nEXPORTS\n");
	printf ("\t%s\t\t\tExports to an baskbin file. (default)\n", B_CMD_BASKBIN);
	printf ("\t%s\t\t\tExports to an csv file.\n", B_CMD_CSV);
	printf ("\t%s\t\t\tExports to an iCalendar file.\n", B_CMD_ICAL);
	
	printf ("\nIMPORTS\n");
	printf ("\t%s\t\t\tImports from an baskbin file. (default)\n", B_CMD_BASKBIN);
	printf ("\t%s\t\t\tImports from an csv file.\n", B_CMD_CSV);
	printf ("\t%s\t\t\tImports from an iCalendar file.\n", B_CMD_ICAL);
	
	printf ("\nPRIORITYS\n");
	printf ("\t0\tL\tNormal\n");
	printf ("\t1\tI\tImportant\n");
	printf ("\t2\tT\tToday\n");
	printf ("\t3\tC\tCritical\n");
	
	printf ("\nFILTERS\n");
	printf ("\t-a, -p\t\t\tThese args can be used, look at ARGUMENTS for more info.\n");
	printf ("\t-s [finished]\t\tUse only tasks if there finished flag equals [finished].\n");
	printf ("\t--day [day]\t\tUse only tasks if there day of the added date equals day.\n");
	printf ("\t--month [month]\t\tUse only tasks if there month of the added date equals month.\n");
	printf ("\t--year [year]\t\tUse only tasks if there year of the added date equals year.\n");
	printf ("\t--today\t\t\tUse only tasks if there added today.\n");
	
	printf ("\nLEGEND: <optional> [necessary] [integer] [STRING]\n");
}

/*
	Function: usage (void);
	Description: Prints the usage message!
	InitVersion: 0.0.1
*/
static void usage (void)
{
	printf ("Usage: %s %s\n", P_CMD, B_CMD_HELP);
}

int main (int argc, char* argv[])
{
	int optc, optindex;
	short filter, pact, ppri, pday, pmonth, pyear, pfin;
	char padded[T_S_ADDED], pdue[T_S_DUE], pfinished[T_S_FINISHED], pproject[T_S_PROJECT], pdescription[T_S_DESCRIPTION];
	bask_core tcore;
	bask_theme btheme;
	bask_priority* bprioritys = NULL;
	bask_filter bfilter;
	struct bask_task* first = NULL;
	
	struct option long_options[] = {
		 {"help", no_argument, 0, B_CMD_ARG_HELP},
		 {"about", no_argument, 0, B_CMD_ARG_ABOUT},
		 {"day", required_argument, 0, B_CMD_ARG_DAY},
		 {"month", required_argument, 0, B_CMD_ARG_MONTH},
		 {"year", required_argument, 0, B_CMD_ARG_YEAR},
		 {"due", required_argument, 0, B_CMD_ARG_DUE},
		 {"today", no_argument, 0, B_CMD_ARG_TODAY},
		 {0,0,0,0}
	};
	
	optindex = filter = tcore.priority_min = tcore.priority_max = 0;
	ppri = pact = pday = pmonth = pyear = pfin = -1;
	
	strcpy (padded, "");
	strcpy (pdue, "");
	strcpy (pfinished, "");
	strcpy (pproject, "");
	strcpy (pdescription, "");
	
	strcpy (tcore.path_baskpath, secure_getenv("HOME"));
	
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
		if (utils_streq (argv[optind], B_CMD_HELP) == 0)
		{
			print_help ();
			exit (EXIT_SUCCESS);
		}
		else if (utils_streq (argv[optind], B_CMD_ABOUT) == 0)
		{
			print_about ();
			exit (EXIT_SUCCESS);
		}
		else if (utils_streq (argv[optind], B_CMD_INIT) == 0)
		{
			bask_init_local (&tcore);
			exit (EXIT_SUCCESS);
		}
	}
	else if (argc == 3)
	{
		if (utils_streq (argv[optind], B_CMD_INIT) == 0)
		{
			if (utils_streq (argv[optind+1], B_CMD_BASKCONF) == 0)
			{
				/* Saving the default values and exit. */
				config_save (&tcore);
				exit (EXIT_SUCCESS);
			}
			else if (utils_streq (argv[optind+1], B_CMD_BASKBIN) == 0)
			{
				bask_init_baskbin (&tcore);
				exit (EXIT_SUCCESS);
			}
			else if (utils_streq (argv[optind+1], B_CMD_BASKTHEME) == 0)
			{
				bask_init_basktheme (&tcore);
				exit (EXIT_SUCCESS);
			}
		}
	}
	
	config_load (&tcore);
	ui_theme_load (&tcore, &btheme);
	
	/* Default Prioritys */
	priority_create (&tcore, &bprioritys, 0, btheme.color_normal, "L", "Normal");
	priority_create (&tcore, &bprioritys, 1, btheme.color_important, "I", "Important");
	priority_create (&tcore, &bprioritys, 2, btheme.color_today, "T", "Today");
	priority_create (&tcore, &bprioritys, 3, btheme.color_critical, "C", "Critical");
	
	while ((optc = getopt_long (argc, argv, "p:P:a:D:F:A:s:fh", long_options, &optindex)) != -1)
	{
		switch (optc)
		{
			case 'p':
				ppri = priority_get_idfromstr (&bprioritys, optarg);
				break;
			case 'P':
				if (strlen (optarg) < sizeof (pproject))
				{
					strcpy (pproject, optarg);
				}
				break;
			case 'a':
				if (utils_atos (&pact, optarg) == -2 || ISBOOL (pact) != TRUE)
				{
					errors_outofrange_int ("-a", FALSE, TRUE);
				}
				break;
			case 'D':
				if (strlen (optarg) < sizeof (pdescription))
				{
					strcpy (pdescription, optarg);
				}
				break;
			case 'F':
				time_get_str_described (pfinished, sizeof (pfinished), optarg);
				break;
			case 'A':
				time_get_str_described (padded, sizeof (padded), optarg);
				break;
			case 's':
				if (utils_atos (&pfin, optarg) == -2 || ISBOOL (pfin) != TRUE)
				{
					errors_outofrange_int ("-s", FALSE, TRUE);
				}
				break;
			case 'f':
				filter = TRUE;
				break;
			case B_CMD_ARG_HELP:
				print_help ();
				exit (EXIT_SUCCESS);
				break;
			case B_CMD_ARG_ABOUT:
				print_about ();
				exit (EXIT_SUCCESS);
				break;
			case B_CMD_ARG_DAY:
				if (utils_atos (&pday, optarg) == -2 || pday < 1 || pday > 31)
				{
					errors_outofrange_int ("--day", 1, 31);
				}
				break;
			case B_CMD_ARG_MONTH:
				if (utils_atos (&pmonth, optarg) == -2 || pmonth < 1 || pmonth > 12)
				{
					errors_outofrange_int ("--month", 1, 12);
				}
				break;
			case B_CMD_ARG_YEAR:
				if (utils_atos (&pyear, optarg) == -2)
				{
					errors_outofrange_int ("--year", SHRT_MIN, SHRT_MAX);
				}
				break;
			case B_CMD_ARG_DUE:
				time_get_str_described (pdue, sizeof (pdue), optarg);
				break;
			case B_CMD_ARG_TODAY:
				/* TODO: Improve this. */
				if (time_get_str (padded, sizeof (padded)) == 0)
				{
					pday = (short) time_get_day (padded);
					pmonth = (short) time_get_month (padded);
					pyear = (short) time_get_year (padded);
				}
				break;
			default:
				break;
		}
	}
	
	if (filter == TRUE)
	{
		filter_init (&bfilter, pact, pfin, ppri, pday, pmonth, pyear);
	}
	else
	{
		filter_init (&bfilter, TRUE, -1, -1, -1, -1, -1);
	}
	
	bask_init (&tcore, &first);
	
	if (optind > 1)
	{
		if (argc-optind == 1)
		{
			if (utils_streq (argv[optind], B_CMD_LIST) == 0)
			{
				view_tasklist (&tcore, &btheme, &bprioritys, &first, &bfilter);
			}
			else if (utils_streq (argv[optind], B_CMD_SUMMARY) == 0)
			{
				view_summary (&tcore, &btheme, &first, &bfilter);
			}
			else if (utils_streq (argv[optind], B_CMD_HISTORY) == 0)
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
			if (utils_streq (argv[optind], B_CMD_MOD) == 0)
			{
				task_modificate_cmd (&tcore, &first, atoi (argv[optind+1]), pact, -1, ppri, padded, pdue, pfinished, pproject, pdescription);
			}
			else if (utils_streq (argv[optind], B_CMD_SEARCH) == 0)
			{
				search_view (&tcore, &btheme, &bprioritys, &bfilter, &first, argv[optind+1], BVIEW_TASKLIST);
			}
			else
			{
				usage ();
			}
		}
		else if (argc-optind == 3)
		{
			if (utils_streq (argv[optind], B_CMD_SEARCH) == 0)
			{
				if (utils_streq (argv[optind+1], B_CMD_TASKLIST) == 0)
				{
					search_view (&tcore, &btheme, &bprioritys, &bfilter, &first, argv[optind+2], BVIEW_TASKLIST);
				}
				else if (utils_streq (argv[optind+1], B_CMD_SUMMARY) == 0)
				{
					search_view (&tcore, &btheme, &bprioritys, &bfilter, &first, argv[optind+2], BVIEW_SUMMARY);
				}
				else if (utils_streq (argv[optind+1], B_CMD_DETAILED) == 0)
				{
					search_view (&tcore, &btheme, &bprioritys, &bfilter, &first, argv[optind+2], BVIEW_DETAILED);
				}
				else
				{
					usage ();
				}
			}
		}
		else
		{
			usage ();
		}
	}
	else if (argc == 2)
	{
		if (utils_streq (argv[optind], B_CMD_LIST) == 0)
		{
			view_tasklist (&tcore, &btheme, &bprioritys, &first, &bfilter);
		}
		else if (utils_streq (argv[optind], B_CMD_SUMMARY) == 0)
		{
			view_summary (&tcore, &btheme, &first, &bfilter);
		}
		else if (utils_streq (argv[optind], B_CMD_HISTORY) == 0)
		{
			/* This allows History to use all tasks including the hidden ones. */
			filter_init (&bfilter, -1, -1, -1, -1, -1, -1);
			view_history (&tcore, &btheme, &first, &bfilter);
		}
		else
		{
			usage ();
		}
	}
	else if (argc == 3)
	{
		if (utils_streq (argv[optind], B_CMD_FINISH) == 0)
		{
			task_finish_cmd (&tcore, &first, atoi (argv[optind+1]));
		}
		else if (utils_streq (argv[optind], B_CMD_REMOVE) == 0)
		{
			task_remove_cmd (&tcore, &first, atoi (argv[optind+1]));
		}
		else if (utils_streq (argv[optind], B_CMD_SEARCH) == 0)
		{
			search_view (&tcore, &btheme, &bprioritys, &bfilter, &first, argv[optind+1], BVIEW_TASKLIST);
		}
		else if (utils_streq (argv[optind], B_CMD_STOP) == 0)
		{
			task_deactivate_cmd (&tcore, &first, atoi (argv[optind+1]));
		}
		else if (utils_streq (argv[optind], B_CMD_SHOW) == 0)
		{
			view_single (&tcore, &first, &bfilter, atoi (argv[optind+1]), FALSE);
		}
		else if (utils_streq (argv[optind], B_CMD_EXPORT) == 0)
		{
			export_baskbin_cmd (&tcore, &first, argv[optind+1]);
		}
		else if (utils_streq (argv[optind], B_CMD_IMPORT) == 0)
		{
			import_baskbin_cmd (&tcore, &first, argv[optind+1]);
		}
		else if (utils_streq (argv[optind], B_CMD_CONFIG) == 0)
		{
			config_set_str_cmd (&tcore, argv[optind+1]);
		}
		else
		{
			usage ();
		}
	}
	else if (argc == 4)
	{
		if (utils_streq (argv[optind], B_CMD_SEARCH) == 0)
		{
			if (utils_streq (argv[optind+1], B_CMD_TASKLIST) == 0)
			{
				search_view (&tcore, &btheme, &bprioritys, &bfilter, &first, argv[optind+2], BVIEW_TASKLIST);
			}
			else if (utils_streq (argv[optind+1], B_CMD_SUMMARY) == 0)
			{
				search_view (&tcore, &btheme, &bprioritys, &bfilter, &first, argv[optind+2], BVIEW_SUMMARY);
			}
			else if (utils_streq (argv[optind+1], B_CMD_DETAILED) == 0)
			{
				search_view (&tcore, &btheme, &bprioritys, &bfilter, &first, argv[optind+2], BVIEW_DETAILED);
			}
			else
			{
				usage ();
			}
		}
		else if (utils_streq (argv[optind], B_CMD_EXPORT) == 0)
		{
			if (utils_streq (argv[optind+1], B_CMD_BASKBIN) == 0)
			{
				export_baskbin_cmd (&tcore, &first, argv[optind+2]);
			}
			else if (utils_streq (argv[optind+1], B_CMD_CSV) == 0)
			{
				export_csv_cmd (&tcore, &first, argv[optind+2]);
			}
			else if (utils_streq (argv[optind+1], B_CMD_ICAL) == 0)
			{
				export_ical_cmd (&tcore, &first, argv[optind+2]);
			}
			else if (utils_streq (argv[optind+1], B_CMD_WEB) == 0)
			{
				export_web (&tcore, &first, argv[optind+2]);
			}
			else
			{
				usage ();
			}
		}
		else if (utils_streq (argv[optind], B_CMD_IMPORT) == 0)
		{
			if (utils_streq (argv[optind+1], B_CMD_BASKBIN) == 0)
			{
				import_baskbin_cmd (&tcore, &first, argv[optind+2]);
			}
			else if (utils_streq (argv[optind+1], B_CMD_CSV) == 0)
			{
				import_csv_cmd (&tcore, &first, argv[optind+2]);
			}
			else if (utils_streq (argv[optind+1], B_CMD_ICAL) == 0)
			{
				import_ical_cmd (&tcore, &first, argv[optind+2]);
			}
			else
			{
				usage ();
			}
		}
		else if (utils_streq (argv[optind], B_CMD_DUE) == 0)
		{
			time_get_str_described (pdue, sizeof (pdue), argv[optind+2]);
			task_due_cmd (&tcore, &first, atoi (argv[optind+1]), pdue);
		}
		else
		{
			usage ();
		}
	}
	else if (argc == 5)
	{	
		if (utils_streq (argv[optind], B_CMD_ADD) == 0)
		{
			task_create_cmd (&tcore, &first, priority_get_idfromstr (&bprioritys, argv[optind+1]), argv[optind+2], argv[optind+3]);
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
	bask_unload (&bprioritys, &first);
	return 0;
}
