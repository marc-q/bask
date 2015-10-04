/* Copyright 2015 Marc Volker Dickmann */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/dutils.h"
#include "../src/bask_core.h"
#include "../src/bask_ui.h"

/* |--------------------------------------------|
   |			Utils			|
   |--------------------------------------------| */

/*
	Function: tst_print_success (char* tstname);
	Description: Prints the test success message.
	InitVersion: 0.0.1
*/
static void tst_print_success (char* tstname)
{
	printf ("%s:\t%sSUCCESS%s\n", tstname, BC_BLD_GREEN, BC_TXT_RST);
}

/*
	Function: tst_print_fail (char* tstname);
	Description: Prints the test failure message.
	InitVersion: 0.0.1
*/
static void tst_print_fail (char* tstname)
{
	printf ("%s:\t%sFAIL%s\n", tstname, BC_BLD_RED, BC_TXT_RST);
}

/* |--------------------------------------------|
   |		    Tests-Core			|
   |--------------------------------------------| */

/*
	Function: tst_core_streq (void);
	Description: Tests the utils_streq function from bask_core.c.
	InitVersion: 0.0.1
*/
static void tst_core_streq (void)
{
	if (utils_streq ("this is a str test.", "this is a str test.") == 0 &&
	    utils_streq ("this is a str test", "this is a str test.") == -1 &&
	    utils_streq ("this is a str test.", "this is a str test") == -1)
	{
		tst_print_success ("Core_Streq");
	}
	else
	{
		tst_print_fail ("Core_Streq");
	}
}

/*
	Function: tst_core_parser (void);
	Description: Tests the parser functions from bask_core.c.
	InitVersion: 0.0.1
*/
static int tst_core_parser (void)
{
	int outint;
	char outstr[50], instr[50];
	char *token, *saveptr;

	strcpy (instr, "test=this is a str test.;\n");

	token = strtok_r (instr, BASKSEP, &saveptr);
	
	if (token == NULL)
	{
		tst_print_fail ("Core_Parser_Str");
		return -1;
	}
	
	parser_get_str (token, "test", outstr, sizeof (outstr), BASKSEP, saveptr);
	
	if (utils_streq (outstr, "this is a str test.") == 0)
	{
		tst_print_success ("Core_Parser_Str");
	}
	else
	{
		tst_print_fail ("Core_Parser_Str");
	}
	
	strcpy (instr, "test=2015;\n");
	
	token = strtok_r (instr, BASKSEP, &saveptr);
	
	if (token == NULL)
	{
		tst_print_fail ("Core_Parser_Int");
		return -1;
	}
	
	parser_get_int (token, "test", &outint, BASKSEP, saveptr);
	
	if (outint == 2015)
	{
		tst_print_success ("Core_Parser_Int");
	}
	else
	{
		tst_print_fail ("Core_Parser_Int");
	}
	
	return 0;
}

/*
	Function: tst_core_time (void);
	Description: Tests the utils_time_get_str function from bask_core.c.
	InitVersion: 0.0.1
*/
static int tst_core_time (void)
{
	char datestr[20];
	
	utils_time_get_str (datestr, sizeof (datestr));
	
	if (datestr == NULL)
	{
		tst_print_fail ("Core_Time_GetStr");
		return -1;
	}
	
	if (strlen(datestr) == 19 &&
	    datestr[2] == '/' &&
	    datestr[5] == '/' &&
	    datestr[8] == '/' &&
	    datestr[11] == '/' &&
	    datestr[14] == '/')
	{
		tst_print_success ("Core_Time_GetStr");
	}
	else
	{
		tst_print_fail ("Core_Time_GetStr");
	}
	
	return 0;
}

/* |--------------------------------------------|
   |		    Tests-UI			|
   |--------------------------------------------| */

/*
	Function: tst_ui_getcolor (char* colorname, char* colorcode);
	Description: Tests the view_theme_get_color function from bask_ui.c.
	InitVersion: 0.0.1
*/
static int tst_ui_getcolor (char* colorname, char* colorcode)
{
	char color[11];
	
	if (strlen (colorname) >= sizeof (color) || strlen (colorcode) >= sizeof (color))
	{
		printf ("UI_GetColor_%s:\t%sFAIL%s\n", colorname, BC_BLD_RED, BC_TXT_RST);
		return -1;
	}
	
	strcpy (color, colorname);
	
	view_theme_get_color (color);
	
	if (utils_streq (color , colorcode) == 0)
	{
		printf ("UI_GetColor_%s:\t%sSUCCESS%s\n", colorname, BC_BLD_GREEN, BC_TXT_RST);
	}
	else
	{
		printf ("UI_GetColor_%s:\t%sFAIL%s\n", colorname, BC_BLD_RED, BC_TXT_RST);
	}
	
	return 0;
}

int main (int argc, char* argv[])
{
	printf ("Bask-Tests (c) 2015 Marc Volker Dickmann\n\n");
	
	tst_core_streq ();
	tst_core_parser ();
	tst_core_time ();
	
	printf ("\n");
	
	tst_ui_getcolor ("txt_black", "\033[0;30m");
	tst_ui_getcolor ("txt_red", "\033[0;31m");
	tst_ui_getcolor ("txt_green", "\033[0;32m");
	tst_ui_getcolor ("txt_yellow", "\033[0;33m");
	tst_ui_getcolor ("txt_blue", "\033[0;34m");
	tst_ui_getcolor ("txt_purple", "\033[0;35m");
	tst_ui_getcolor ("txt_cyan", "\033[0;36m");
	tst_ui_getcolor ("txt_white", "\033[0;37m");
	
	printf ("\n");
	
	tst_ui_getcolor ("bld_black", "\033[1;30m");
	tst_ui_getcolor ("bld_red", "\033[1;31m");
	tst_ui_getcolor ("bld_green", "\033[1;32m");
	tst_ui_getcolor ("bld_yellow", "\033[1;33m");
	tst_ui_getcolor ("bld_blue", "\033[1;34m");
	tst_ui_getcolor ("bld_purple", "\033[1;35m");
	tst_ui_getcolor ("bld_cyan", "\033[1;36m");
	tst_ui_getcolor ("bld_white", "\033[1;37m");
	
	printf ("\n");
	
	tst_ui_getcolor ("und_black", "\033[4;30m");
	tst_ui_getcolor ("und_red", "\033[4;31m");
	tst_ui_getcolor ("und_green", "\033[4;32m");
	tst_ui_getcolor ("und_yellow", "\033[4;33m");
	tst_ui_getcolor ("und_blue", "\033[4;34m");
	tst_ui_getcolor ("und_purple", "\033[4;35m");
	tst_ui_getcolor ("und_cyan", "\033[4;36m");
	tst_ui_getcolor ("und_white", "\033[4;37m");
	
	printf ("\n");
	
	tst_ui_getcolor ("bak_black", "\033[40m");
	tst_ui_getcolor ("bak_red", "\033[41m");
	tst_ui_getcolor ("bak_green", "\033[42m");
	tst_ui_getcolor ("bak_yellow", "\033[43m");
	tst_ui_getcolor ("bak_blue", "\033[44m");
	tst_ui_getcolor ("bak_purple", "\033[45m");
	tst_ui_getcolor ("bak_cyan", "\033[46m");
	tst_ui_getcolor ("bak_white", "\033[47m");
	
	return 0;
}
