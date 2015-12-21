/* Copyright 2015 Marc Volker Dickmann */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/dutils.h"
#include "bask_core.h"
#include "bask_errors.h"

/*
	Function: config_set_str (bask_core* tcore, char* str);
	Description: Sets config values with a str.
	InitVersion: 0.0.1
*/
void config_set_str (bask_core* tcore, char* str)
{
	short tmpsvalue;
	char *token, *saveptr, baskbin[151];
	
	token = strtok_r (str, BASKSEP, &saveptr);
	
	if (parser_get_str (token, "baskbin", baskbin, sizeof (baskbin), BASKSEP, saveptr) == 0)
	{
		if (utils_streq (baskbin, "default") != 0)
		{
			strcpy (tcore->path_baskbin, baskbin);
		}
	}
	else if (parser_get_short (token, "task_project_min", &tmpsvalue, BASKSEP, saveptr) == 0)
	{
		if (tmpsvalue < 0 || tmpsvalue > 200)
		{
			errors_outofrange ("task_project_min", 0, 200);
		}
		else
		{
			tcore->t_projectmin = tmpsvalue;
		}
	}
	else if (parser_get_short (token, "task_description_max", &tmpsvalue, BASKSEP, saveptr) == 0)
	{
		if (tmpsvalue < 0 || tmpsvalue > 200)
		{
			errors_outofrange ("task_description_max", 0, 200);
		}
		else
		{
			tcore->t_descriptionmax = tmpsvalue;
		}
	}
	else if (parser_get_short (token, "task_description_min", &tmpsvalue, BASKSEP, saveptr) == 0)
	{
		if (tmpsvalue < 0 || tmpsvalue > 200)
		{
			errors_outofrange ("task_description_min", 0, 200);
			exit (EXIT_FAILURE);
		}
		else
		{
			tcore->t_descriptionmin = tmpsvalue;
		}
	}
	else if (parser_get_short (token, "task_description_break", &tmpsvalue, BASKSEP, saveptr) == 0)
	{
		if (tmpsvalue < 0 || tmpsvalue > 1)
		{
			errors_outofrange ("task_description_break", 0, 1);
			exit (EXIT_FAILURE);
		}
		else
		{
			tcore->t_options ^= BITCOPY (tmpsvalue, 0, tcore->t_options, T_O_DESCRIPTIONBREAK);
		}
	}
}

/*
	Function: config_init_file (bask_core* tcore);
	Description: Inits a config file at tcore->path_baskconf.
	InitVersion: 0.0.1
*/
void config_init_file (bask_core* tcore)
{
	FILE* baskfile;
	
	if (bask_init_local_file (&baskfile, tcore->path_baskconf) == 0)
	{	
		fprintf (baskfile, "# Path to the baskbin.\nbaskbin=default;\n#\n");
		
		fprintf (baskfile, "The minimum length of the project field in characters (0-200; default: 15;)\ntask_project_min=15;\n#\n");
		
		fprintf (baskfile, "# The maximum length of descriptions in characters (0-200; default: 50;)\ntask_description_max=50;\n#\n");
		fprintf (baskfile, "# The minimum length of the description field in characters (0-200; default: 50;)\ntask_description_min=50;\n#\n");
		fprintf (baskfile, "# Should longer lines be broken when viewed? (0/1; default: 1)\ntask_description_break=1;\n");
	
		fclose (baskfile);	
	}
}

/*
	Function: config_save (bask_core* tcore);
	Description: Save the config file from bask.
	InitVersion: 0.0.1
*/
int config_save (bask_core* tcore)
{
	char line[200];
	FILE *baskconf;
	
	baskconf = fopen (tcore->path_baskconf, "r+");
	
	if (baskconf == NULL)
	{
		return -1;
	}
	
	while (fgets (line, sizeof (line), baskconf) != NULL)
	{
		if (line[0] != '#')
		{
			if (strncmp ("baskbin=", line, strlen ("baskbin=")) == 0)
			{
				fseek (baskconf, -(strlen (line)), SEEK_CUR);
				fprintf (baskconf, "baskbin=%s;", tcore->path_baskbin);
			}
			else if (strncmp ("task_project_min=", line, strlen ("task_project_min=")) == 0)
			{
				fseek (baskconf, -(strlen (line)), SEEK_CUR);
				fprintf (baskconf, "task_project_min=%i;", (int)tcore->t_projectmin);
			}
			else if (strncmp ("task_description_max=", line, strlen ("task_description_max=")) == 0)
			{
				fseek (baskconf, -(strlen (line)), SEEK_CUR);
				fprintf (baskconf, "task_description_max=%i;", (int)tcore->t_descriptionmax);
			}
			else if (strncmp ("task_description_min=", line, strlen ("task_description_min=")) == 0)
			{
				fseek (baskconf, -(strlen (line)), SEEK_CUR);
				fprintf (baskconf, "task_description_min=%i;", (int)tcore->t_descriptionmin);
			}
		}
	}
	
	fclose (baskconf);
	
	return 0;
}

/*
	Function: config_load (bask_core* tcore);
	Description: Loads the config file from bask.
	InitVersion: 0.0.1
*/
void config_load (bask_core* tcore)
{
	int tmp;
	char line[200], baskbin[151];
	char *token, *saveptr;
	FILE *baskconf;
	
	tcore->t_projectmin = 15;
	tcore->t_descriptionmax = 50;
	tcore->t_descriptionmin = 50;
	
	tcore->t_options = 0;
	tcore->t_options ^= BITCOPY (1, 0, tcore->t_options, T_O_DESCRIPTIONBREAK);
	
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
			parser_get_short (token, "task_project_min", &tcore->t_projectmin, BASKSEP, saveptr);
			parser_get_short (token, "task_description_max", &tcore->t_descriptionmax, BASKSEP, saveptr);
			parser_get_short (token, "task_description_min", &tcore->t_descriptionmin, BASKSEP, saveptr);
			
			if (parser_get_int (token, "task_description_break", &tmp, BASKSEP, saveptr) == 0)
			{
				tcore->t_options ^= BITCOPY (tmp, 0, tcore->t_options, T_O_DESCRIPTIONBREAK);
			}
		}
	}
	
	fclose (baskconf);
	
	if (utils_streq (baskbin, "default") != 0)
	{
		strcpy (tcore->path_baskbin, baskbin);
	}
	
	if (tcore->t_projectmin < 0 || tcore->t_projectmin > 200)
	{
		errors_outofrange ("task_project_min", 0, 200);
		exit (EXIT_FAILURE);
	}
	
	if (tcore->t_descriptionmax < 0 || tcore->t_descriptionmax > 200)
	{
		errors_outofrange ("task_description_max", 0, 200);
		exit (EXIT_FAILURE);
	}
	
	if (tcore->t_descriptionmin < 0 || tcore->t_descriptionmin > 200)
	{
		errors_outofrange ("task_description_min", 0, 200);
		exit (EXIT_FAILURE);
	}
	
}
