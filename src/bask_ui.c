/* Copyright 2015 - 2016 Marc Volker Dickmann */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../lib/dutils.h"
#include "bask_core.h"
#include "bask_errors.h"
#include "bask_ui.h"

/* |--------------------------------------------|
   |			Core			|
   |--------------------------------------------| */

/*
	Function: ui_print_nspaces (int amount);
	Description: Prints amount spaces.
	InitVersion: 0.0.1
*/
void ui_print_nspaces (int amount)
{
	if (amount > 0)
	{
		while (amount > 0)
		{
			printf (" ");
			amount--;
		}
	}
}

/*
	Function: ui_print_nspaces_str (char* tag, int length);
	Description: Prints length spaces minus the length of tag.
	InitVersion: 0.0.1
*/
void ui_print_nspaces_str (char* tag, int length)
{	
	ui_print_nspaces (length - strlen (tag));
}

/* |--------------------------------------------|
   |			Theme			|
   |--------------------------------------------| */

/*
	Function: ui_theme_get_color (char* color);
	Description: Replaces the color name with its code.
	InitVersion: 0.0.1
*/
void ui_theme_get_color (char* color)
{	
	short tmp;
	char precolor[UI_S_THEMECOLOR];
	
	tmp = 0;
	
	if (color != NULL && strlen (color) < UI_S_THEMECOLOR)
	{
		strcpy (precolor, color);
		
		if (strncmp (precolor, "txt", strlen ("txt")) == 0)
		{
			strcpy (color, "\033[0;");
			tmp = 1;
		}
		else if (strncmp (precolor, "bld", strlen ("bld")) == 0)
		{
			strcpy (color, "\033[1;");
			tmp = 1;
		}
		else if (strncmp (precolor, "und", strlen ("und")) == 0)
		{
			strcpy (color, "\033[4;3");
		}
		else if (strncmp (precolor, "bak", strlen ("bak")) == 0)
		{
			strcpy (color, "\033[");
			tmp = 2;
		}
	
		if (tmp == 1)
		{
			strcat (color, (strstr (precolor, "_h") != NULL ? BC_HIGH_TXT : BC_LOW_TXT));
		}
		else if (tmp == 2)
		{
			strcat (color, (strstr (precolor, "_h") != NULL ? BC_HIGH_BAK : BC_LOW_BAK));
		}
		
		if (strstr (precolor, "black") != NULL)
		{
			strcat (color, BC_BLACK);
		}
		else if (strstr (precolor, "red") != NULL)
		{
			strcat (color, BC_RED);
		}
		else if (strstr (precolor, "green") != NULL)
		{
			strcat (color, BC_GREEN);
		}
		else if (strstr (precolor, "yellow") != NULL)
		{
			strcat (color, BC_YELLOW);
		}
		else if (strstr (precolor, "blue") != NULL)
		{
			strcat (color, BC_BLUE);
		}
		else if (strstr (precolor, "purple") != NULL)
		{
			strcat (color, BC_PURPLE);
		}
		else if (strstr (precolor, "cyan") != NULL)
		{
			strcat (color, BC_CYAN);
		}
		else if (strstr (precolor, "white") != NULL)
		{
			strcat (color, BC_WHITE);
		}
		
		strcat (color, "m");
	}
}

/*
	Function: ui_theme_load (bask_core* tcore, bask_theme* btheme);
	Description: Loads the theme file.
	InitVersion: 0.0.1
*/
void ui_theme_load (bask_core* tcore, bask_theme* btheme)
{
	int i;
	char line[200];
	char colors[7][UI_S_THEMECOLOR];
	char saveptr[200];
	FILE *basktheme;
	
	/* NOTE: If an config doesn't exist, f.e. an old config is used, its using the default value instead of crashing. */
	for (i = 0; i < 7; i++)
	{
		strcpy (colors[i], "default");
	}
	
	basktheme = fopen (tcore->path_basktheme, "r");
	
	if (basktheme == NULL)
	{
		errors_filenotopened (tcore->path_basktheme);
		errors_useinit ();
		exit (EXIT_FAILURE);
	}
	
	while (fgets (line, sizeof (line), basktheme) != NULL)
	{
		if (line[0] != '#')
		{
			parser_get_str (line, "color_normal=", colors[0], UI_S_THEMECOLOR, BASKSEP, saveptr);
			parser_get_str (line, "color_important=", colors[1], UI_S_THEMECOLOR, BASKSEP, saveptr);
			parser_get_str (line, "color_today=", colors[2], UI_S_THEMECOLOR, BASKSEP, saveptr);
			parser_get_str (line, "color_critical=", colors[3], UI_S_THEMECOLOR, BASKSEP, saveptr);
			parser_get_str (line, "color_finished=", colors[4], UI_S_THEMECOLOR, BASKSEP, saveptr);
			parser_get_str (line, "color_pbarbak=", colors[5], UI_S_THEMECOLOR, BASKSEP, saveptr);
			parser_get_str (line, "color_seclinesbak=", colors[6], UI_S_THEMECOLOR, BASKSEP, saveptr);
		}
	}
	
	fclose (basktheme);
	
	if (utils_streq (colors[0], "default") != 0)
	{
		ui_theme_get_color (colors[0]);
		strcpy (btheme->color_normal, colors[0]);
	}
	else
	{
		strcpy (btheme->color_normal, BC_TXT_RST);
	}
	
	if (utils_streq (colors[1], "default") != 0)
	{
		ui_theme_get_color (colors[1]);
		strcpy (btheme->color_important, colors[1]);
	}
	else
	{
		strcpy (btheme->color_important, BC_BLD_GREEN);
	}
	
	if (utils_streq (colors[2], "default") != 0)
	{
		ui_theme_get_color (colors[2]);
		strcpy (btheme->color_today, colors[2]);
	}
	else
	{
		strcpy (btheme->color_today, BC_BLD_BLUE);
	}
	
	if (utils_streq (colors[3], "default") != 0)
	{
		ui_theme_get_color (colors[3]);
		strcpy (btheme->color_critical, colors[3]);
	}
	else
	{
		strcpy (btheme->color_critical, BC_BLD_RED);
	}
	
	if (utils_streq (colors[4], "default") != 0)
	{
		ui_theme_get_color (colors[4]);
		strcpy (btheme->color_finished, colors[4]);
	}
	else
	{
		strcpy (btheme->color_finished, BC_BLD_BLACK);
	}
	
	if (utils_streq (colors[5], "default") != 0)
	{
		ui_theme_get_color (colors[5]);
		strcpy (btheme->color_pbarbak, colors[5]);
	}
	else
	{
		strcpy (btheme->color_pbarbak, BC_BAK_GREEN);
	}
	
	if (utils_streq (colors[6], "default") != 0)
	{
		ui_theme_get_color (colors[6]);
		strcpy (btheme->color_seclinesbak, colors[6]);
	}
	else
	{
		strcpy (btheme->color_seclinesbak, "");
	}
}

/* |--------------------------------------------|
   |			Misc			|
   |--------------------------------------------| */

/*
	Function: ui_misc_print_progress (float p, char* bakcolor);
	Description: Prints a progress bar filled p%.
	InitVersion: 0.0.1
*/
void ui_misc_print_progress (float p, char* bakcolor)
{
	printf ("%s", bakcolor);
	
	ui_print_nspaces ((p/5)+1);
	
	printf ("%s\n", BC_TXT_RST);
}

/* |--------------------------------------------|
   |			Table			|
   |--------------------------------------------| */

/*
	Function: ui_tbl_print_title (char* name, int len_underline, int len_normal);
	Description: Prints a table title.
	InitVersion: 0.0.1
*/
void ui_tbl_print_title (char* name, int len_underline, int len_normal)
{	
	printf ("%s%s", BC_TXT_UND, name);
	ui_print_nspaces_str (name, len_underline);
	printf ("%s", BC_TXT_RST);
	
	ui_print_nspaces (len_normal);
}

/*
	Function: ui_tbl_print_field_str (char* value, int len_pre, int len_suf);
	Description: Prints a table field with a str value.
	InitVersion: 0.0.1
*/
void ui_tbl_print_field_str (char* value, int len_pre, int len_suf)
{
	ui_print_nspaces_str (value, len_pre);
	
	printf ("%s", value);
	
	ui_print_nspaces_str (value, len_suf);
}

/*
	Function: ui_tbl_print_field_int (int value, int len_pre, int len_suf);
	Description: Prints a table field with a int value.
	InitVersion: 0.0.1
*/
void ui_tbl_print_field_int (int value, int len_pre, int len_suf)
{	
	ui_print_nspaces (len_pre);
	
	printf ("%i", value);
	
	ui_print_nspaces (len_suf);
}
