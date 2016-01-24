/* Copyright 2015 - 2016 Marc Volker Dickmann */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/dutils.h"
#include "bask_core.h"
#include "bask_errors.h"
#include "bask_config.h"

/*
	Function: config_set_str_raw (bask_core* tcore, char* line, short* tmpsvalue, char* baskbin, size_t baskbin_size, char* saveptr);
	Description: Sets config values with a str without declaration of new variables. (internal use)
	InitVersion: 0.0.1
*/
short config_set_str_raw (bask_core* tcore, char* line, short* tmpsvalue, char* baskbin, size_t baskbin_size, char* saveptr)
{
	/* Little fix to mark false bools. */
	*tmpsvalue = 2;
	
	if (parser_get_str (line, "baskbin=", baskbin, baskbin_size, BASKSEP, saveptr) != CORE_ERR_PARSER_NOTFOUND)
	{
		if (utils_streq (baskbin, "default") != 0)
		{
			strcpy (tcore->path_baskbin, baskbin);
		}
	}
	else if (parser_get_short (line, "task_project_min=", tmpsvalue, BASKSEP, saveptr) != CORE_ERR_PARSER_NOTFOUND)
	{
		if (*tmpsvalue < 0 || *tmpsvalue > 200)
		{
			return CONFIG_ERR_SS_PROJMIN;
		}
		else
		{
			tcore->t_projectmin = *tmpsvalue;
		}
	}
	else if (parser_get_short (line, "task_description_max=", tmpsvalue, BASKSEP, saveptr) != CORE_ERR_PARSER_NOTFOUND)
	{
		if (*tmpsvalue < 0 || *tmpsvalue > 200)
		{
			return CONFIG_ERR_SS_DESCMAX;
		}
		else
		{
			tcore->t_descriptionmax = *tmpsvalue;
		}
	}
	else if (parser_get_short (line, "task_description_min=", tmpsvalue, BASKSEP, saveptr) != CORE_ERR_PARSER_NOTFOUND)
	{
		if (*tmpsvalue < 0 || *tmpsvalue > 200)
		{
			return CONFIG_ERR_SS_DESCMIN;
		}
		else
		{
			tcore->t_descriptionmin = *tmpsvalue;
		}
	}
	else if (parser_get_bool (line, "task_description_break=", tmpsvalue, BASKSEP, saveptr) != CORE_ERR_PARSER_NOTFOUND)
	{
		if (ISBOOL (*tmpsvalue) != TRUE)
		{
			return CONFIG_ERR_SS_DESCBREAK;
		}
		else
		{
			tcore->t_options ^= BITCOPY (*tmpsvalue, 0, tcore->t_options, T_O_DESCRIPTIONBREAK);
		}
	}
	else if (parser_get_bool (line, "automatic_due_today=", tmpsvalue, BASKSEP, saveptr) != CORE_ERR_PARSER_NOTFOUND)
	{
		if (ISBOOL (*tmpsvalue) != TRUE)
		{
			return CONFIG_ERR_SS_AUTODUETODAY;
		}
		else
		{
			tcore->t_options ^= BITCOPY (*tmpsvalue, 0, tcore->t_options, T_O_AUTODUETODAY);
		}
	}
	else if (parser_get_bool (line, "color=", tmpsvalue, BASKSEP, saveptr) != CORE_ERR_PARSER_NOTFOUND)
	{
		if (ISBOOL (*tmpsvalue) != TRUE)
		{
			return CONFIG_ERR_SS_COLOR;
		}
		else
		{
			tcore->t_options ^= BITCOPY (*tmpsvalue, 0, tcore->t_options, T_O_COLOR);
		}
	}
	
	return 0;
}

/*
	Function: config_set_str (bask_core* tcore, char* str);
	Description: Sets config values with a str.
	InitVersion: 0.0.1
*/
short config_set_str (bask_core* tcore, char* str)
{
	short tmpsvalue;
	char saveptr[200], baskbin[151], cline[200];
	
	/* NOTE: This is a fix to support using this function while given a string instead of an variable for the str argument. */
	if (strlen (str) < sizeof (cline))
	{
		strcpy (cline, str);
	}
	else
	{
		return CONFIG_ERR_SS_CONFLINE;
	}
	
	return config_set_str_raw (tcore, cline, &tmpsvalue, baskbin, sizeof (baskbin), saveptr);
}

/*
	Function: config_set_str_cmd (bask_core* tcore, char* str);
	Description: Cmd handle for changing options via string!
	InitVersion: 0.0.1
*/
void config_set_str_cmd (bask_core* tcore, char* str)
{
	short errorcode;

	errorcode = config_set_str (tcore, str);
	
	if (errorcode != 0)
	{
		config_print_set_str_errors (errorcode);
	}
	else
	{
		printf ("Config succefully changed.\n");
	}
}

/*
	Function: config_print_set_str_errors (int error_id);
	Description: Prints the errormessages for the function config_set_str.
	InitVersion: 0.0.1
*/
void config_print_set_str_errors (short error_id)
{
	switch (error_id)
	{
		case CONFIG_ERR_SS_CONFLINE:
			errors_lengthtobig ("Configline");
			break;
		case CONFIG_ERR_SS_PROJMIN:
			errors_outofrange_int ("task_project_min", 0, 200);
			break;
		case CONFIG_ERR_SS_DESCMAX:
			errors_outofrange_int ("task_description_max", 0, 200);
			break;
		case CONFIG_ERR_SS_DESCMIN:
			errors_outofrange_int ("task_description_min", 0, 200);
			break;
		case CONFIG_ERR_SS_DESCBREAK:
			errors_outofrange_int ("task_description_break", FALSE, TRUE);
			break;
		case CONFIG_ERR_SS_AUTODUETODAY:
			errors_outofrange_int ("automatic_due_today", FALSE, TRUE);
			break;
		case CONFIG_ERR_SS_COLOR:
			errors_outofrange_int ("color", FALSE, TRUE);
			break;
		default:
			break;
			
	}
}

/*
	Function: config_init (bask_core* tcore);
	Description: Init the config by setting default values.
	InitVersion: 0.0.1
*/
void config_init (bask_core* tcore)
{
	tcore->t_projectmin = 15;
	tcore->t_descriptionmax = 50;
	tcore->t_descriptionmin = 50;
	
	tcore->t_options = 0;
	tcore->t_options ^= BITCOPY (TRUE, 0, tcore->t_options, T_O_DESCRIPTIONBREAK);
	tcore->t_options ^= BITCOPY (TRUE, 0, tcore->t_options, T_O_AUTODUETODAY);
	tcore->t_options ^= BITCOPY (TRUE, 0, tcore->t_options, T_O_COLOR);
}

/*
	Function: config_save (bask_core* tcore);
	Description: Save the config file from bask.
	InitVersion: 0.0.1
*/
void config_save (bask_core* tcore)
{
	char tmp[200];
	FILE *baskfile;
	
	if (bask_init_local_file (&baskfile, tcore->path_baskconf) == 0)
	{	
		bask_get_baskpath (tcore, tmp, BASKBINFILE);
				
		if (utils_streq (tcore->path_baskbin, tmp) == 0)
		{
			strcpy (tmp, "default");
		}
		else
		{
			strcpy (tmp, tcore->path_baskbin);
		}
		
		fprintf (baskfile, "# Path to the baskbin.\nbaskbin=%s\n#\n", tmp);
		
		fprintf (baskfile, "# The minimum length of the project field in characters (0-200; default: 15)\ntask_project_min=%hi\n#\n", tcore->t_projectmin);
		
		fprintf (baskfile, "# The maximum length of descriptions in characters (0-200; default: 50)\ntask_description_max=%hi\n#\n", tcore->t_descriptionmax);
		fprintf (baskfile, "# The minimum length of the description field in characters (0-200; default: 50)\ntask_description_min=%hi\n#\n", tcore->t_descriptionmin);
		fprintf (baskfile, "# Should longer lines be broken when viewed? (0/1; default: 1)\ntask_description_break=%hi\n#\n", BITGET (tcore->t_options, T_O_DESCRIPTIONBREAK));
		
		fprintf (baskfile, "# Should tasks with the due date of today automaticly set to the priority today? (0/1; default: 1)\nautomatic_due_today=%hi\n#\n", BITGET (tcore->t_options, T_O_AUTODUETODAY));
		fprintf (baskfile, "# Should Bask use colors? (0/1; default: 1)\ncolor=%hi\n", BITGET (tcore->t_options, T_O_COLOR));
		
		fclose (baskfile);	
	}
}

/*
	Function: config_load (bask_core* tcore);
	Description: Loads the config file from bask.
	InitVersion: 0.0.1
*/
void config_load (bask_core* tcore)
{
	short error, tmpsvalue;
	char line[200], baskbin[151];
	char saveptr[200];
	FILE *baskconf;
	
	error = tmpsvalue = 0;
	
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
			/* NOTE: Using this we only must change one function in order to add options. */
			if ((error = config_set_str_raw (tcore, line, &tmpsvalue, baskbin, sizeof (baskbin), saveptr)) != 0)
			{
				config_print_set_str_errors (error);
				
				fclose (baskconf);
				exit (EXIT_FAILURE);
				break;
			}
		}
	}
	
	fclose (baskconf);
}
