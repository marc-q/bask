/* Copyright 2015 Marc Volker Dickmann */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/dutils.h"
#include "../src/bask_core.h"
#include "../src/bask_time.h"
#include "../src/bask_config.h"
#include "../src/bask_task.h"
#include "../src/bask_ui.h"
#include "../src/bask_export.h"
#include "../src/bask_import.h"

#define TESTS_AMOUNT 42
#define TESTS_FAIL 0
#define TESTS_PASS 1

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
	printf ("%s:", tstname);
	
	if (strlen (tstname) < 15)
	{
		printf ("\t\t");
	}
	else
	{
		printf ("\t");
	}
	
	printf ("%sSUCCESS%s\n", BC_BLD_GREEN, BC_TXT_RST);
}

/*
	Function: tst_print_fail (char* tstname);
	Description: Prints the test failure message.
	InitVersion: 0.0.1
*/
static void tst_print_fail (char* tstname)
{
	printf ("%s:", tstname);
	
	if (strlen (tstname) < 15)
	{
		printf ("\t\t");
	}
	else
	{
		printf ("\t");
	}
	
	printf ("%sFAIL%s\n", BC_BLD_RED, BC_TXT_RST);
}

/*
	Function: tst_print_summary (int points);
	Description: Prints the summary of all tests.
	InitVersion: 0.0.1
*/
static void tst_print_summary (int points)
{
	printf ("\n+=======================+\n");
	printf ("|  Summary of all tests |\n");
	printf ("+=======================+\n");
	printf ("|  Tests: \t%i\t|\n", TESTS_AMOUNT);
	printf ("+-----------------------+\n");
	printf ("|  Passed:\t%i\t|\n", points);
	printf ("|  Failed:\t%i\t|\n", TESTS_AMOUNT-points);
	printf ("+-----------------------+\n");
}

/* |--------------------------------------------|
   |		    Tests-Core			|
   |--------------------------------------------| */

/*
	Function: tst_core_streq (void);
	Description: Tests the utils_streq function from bask_core.c.
	InitVersion: 0.0.1
*/
static int tst_core_streq (void)
{
	if (utils_streq ("this is a str test.", "this is a str test.") == 0 &&
	    utils_streq ("this is a str test", "this is a str test.") == -1 &&
	    utils_streq ("this is a str test.", "this is a str test") == -1)
	{
		tst_print_success ("Core_Streq");
		return TESTS_PASS;
	}
	
	tst_print_fail ("Core_Streq");
	return TESTS_FAIL;
}

/*
	Function: tst_core_parser_str (void);
	Description: Tests the parser_get_str function from bask_core.c.
	InitVersion: 0.0.1
*/
static int tst_core_parser_str (void)
{
	char outstr[50], instr[50];
	char *token, *saveptr;

	strcpy (instr, "test=this is a str test.;\n");

	token = strtok_r (instr, BASKSEP, &saveptr);
	
	if (token == NULL)
	{
		tst_print_fail ("Core_Parser_Str");
		return TESTS_FAIL;
	}
	
	parser_get_str (token, "test", outstr, sizeof (outstr), BASKSEP, saveptr);
	
	if (utils_streq (outstr, "this is a str test.") == 0)
	{
		tst_print_success ("Core_Parser_Str");
		return TESTS_PASS;
	}
	
	tst_print_fail ("Core_Parser_Str");
	return TESTS_FAIL;
}

/*
	Function: tst_core_parser_int (void);
	Description: Tests the parser_get_int function from bask_core.c.
	InitVersion: 0.0.1
*/
static int tst_core_parser_int (void)
{
	int outint;
	char instr[50];
	char *token, *saveptr;
	
	strcpy (instr, "test=2015;\n");
	
	token = strtok_r (instr, BASKSEP, &saveptr);
	
	if (token == NULL)
	{
		tst_print_fail ("Core_Parser_Int");
		return TESTS_FAIL;
	}
	
	parser_get_int (token, "test", &outint, BASKSEP, saveptr);
	
	if (outint == 2015)
	{
		tst_print_success ("Core_Parser_Int");
		return TESTS_PASS;
	}

	tst_print_fail ("Core_Parser_Int");
	return TESTS_FAIL;
}

/*
	Function: tst_core_parser_short (void);
	Description: Tests the parser_get_short function from bask_core.c.
	InitVersion: 0.0.1
*/
static int tst_core_parser_short (void)
{
	short out;
	char instr[50];
	char *token, *saveptr;
	
	strcpy (instr, "test=2015;\n");
	
	token = strtok_r (instr, BASKSEP, &saveptr);
	
	if (token == NULL)
	{
		tst_print_fail ("Core_Parser_Short");
		return TESTS_FAIL;
	}
	
	parser_get_short (token, "test", &out, BASKSEP, saveptr);
	
	if (out == 2015)
	{
		tst_print_success ("Core_Parser_Short");
		return TESTS_PASS;
	}

	tst_print_fail ("Core_Parser_Short");
	return TESTS_FAIL;
}

/* |--------------------------------------------|
   |		    Tests-Time			|
   |--------------------------------------------| */

/*
	Function: tst_time_getstr (void);
	Description: Tests the time_get_str function from bask_time.c.
	InitVersion: 0.0.1
*/
static int tst_time_getstr (void)
{
	char datestr[F_BB_S_DATE];
	
	time_get_str (datestr, sizeof (datestr));
	
	if (datestr == NULL)
	{
		tst_print_fail ("Time_Get_Str");
		return TESTS_FAIL;
	}
	
	if (strlen(datestr) == F_BB_S_DATE-1 &&
	    datestr[2] == '/' &&
	    datestr[5] == '/' &&
	    datestr[8] == '/' &&
	    datestr[11] == '/' &&
	    datestr[14] == '/')
	{
		tst_print_success ("Time_Get_Str");
		return TESTS_PASS;
	}
	
	tst_print_fail ("Time_Get_Str");
	return TESTS_FAIL;
}

/*
	Function: tst_time_gettm_str (void);
	Description: Tests the time_get_tm_str function from bask_time.c.
	InitVersion: 0.0.1
*/
static int tst_time_gettm_str (void)
{
	struct tm out;
	
	if (time_get_tm_str (&out, "23/59/59/09/09/2015") != 0)
	{
		tst_print_fail ("Time_Get_Tm_Str");
		return TESTS_FAIL;
	}
	
	if (out.tm_hour == 23 &&
	    out.tm_min == 59 &&
	    out.tm_sec == 59 &&
	    out.tm_mday == 9 &&
	    out.tm_mon == 8 &&
	    out.tm_year == 115 &&
	    time_get_tm_str (&out, "23/59/59/9/9/2015") == -1)
	{
		tst_print_success ("Time_Get_Tm_Str");
		return TESTS_PASS;
	}
	
	tst_print_fail ("Time_Get_Tm_Str");
	return TESTS_FAIL;
}

/* |--------------------------------------------|
   |		   Tests-Config			|
   |--------------------------------------------| */

/*
	Function: tst_config_setstr (void);
	Description: Tests the config_set_str function from bask_config.c.
	InitVersion: 0.0.1
*/
static int tst_config_setstr (void)
{
	int passed;
	bask_core tcore;
	
	passed = 0;
	
	tcore.t_projectmin = 0;
	tcore.t_descriptionmax = 0;
	tcore.t_descriptionmin = 0;
	tcore.t_options = 0;
	
	config_set_str (&tcore, "task_project_min=50;");
	config_set_str (&tcore, "task_description_max=50;");
	config_set_str (&tcore, "task_description_min=50;");
	config_set_str (&tcore, "task_description_break=1;");

	if (tcore.t_projectmin == 50 &&
	    tcore.t_descriptionmax == 50 &&
	    tcore.t_descriptionmin == 50 &&
	    BITGET (tcore.t_options, T_O_DESCRIPTIONBREAK) == 1)
	{
		passed = 1;
	}
	
	if (passed == 1 &&
	    config_set_str (&tcore, "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum.Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum.Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum. Lorem Ipsum Lorem .") == -1 &&
	    config_set_str (&tcore, "task_project_min=201;") == -2 &&
	    config_set_str (&tcore, "task_description_max=201;") == -3 &&
	    config_set_str (&tcore, "task_description_min=201;") == -4 &&
	    config_set_str (&tcore, "task_description_break=2;") == -5)
	{
		tst_print_success ("Config_Set_Str");
		return TESTS_PASS;
	}
	
	tst_print_fail ("Config_Set_Str");
	return TESTS_FAIL;
}

/* |--------------------------------------------|
   |		    Tests-Task			|
   |--------------------------------------------| */

/*
	Function: tst_task_checkinput (void);
	Description: Tests the task_check_input function from bask_task.c.
	InitVersion: 0.0.1
*/
static int tst_task_checkinput (void)
{
	int passed;
	bask_core tcore;
	
	passed = 0;
	
	tcore.t_descriptionmax = 50;
	tcore.t_options = 0;
	
	if (task_check_input (&tcore, "23/59/59/09/09/2015", "23/59/59/09/09/2015", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", 0) == 0 &&
	    task_check_input (&tcore, "23/59/59/09/09/02015", "23/59/59/09/09/2015", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", 0) == -1 &&
	    task_check_input (&tcore, "23/59/59/09/09/2015", "23/59/59/09/09/02015", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", 0) == -2 &&
	    task_check_input (&tcore, "23/59/59/09/09/2015", "23/59/59/09/09/2015", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum.", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", 0) == -3 &&
	    task_check_input (&tcore, "23/59/59/09/09/2015", "23/59/59/09/09/2015", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum.", 0) == -4)
	{
		passed = 1;
	}
	
	tcore.t_options ^= BITCOPY (1, 0, tcore.t_options, T_O_DESCRIPTIONBREAK);
	
	if (passed == 1 && task_check_input (&tcore, "23/59/59/09/09/2015", "23/59/59/09/09/2015", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum.Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum.Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum. Lorem Ipsum Lorem  .", 0) == -4 &&
	task_check_input (&tcore, "23/59/59/09/09/2015", "23/59/59/09/09/2015", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum.Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum.Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum. Lorem Ipsum Lorem .", 0) == 0)
	{
		tst_print_success ("Task_Check_Input");
		return TESTS_PASS;
	}

	tst_print_fail ("Task_Check_Input");
	return TESTS_FAIL;
}

/* |--------------------------------------------|
   |		    Tests-UI			|
   |--------------------------------------------| */

/*
	Function: tst_ui_getcolor (char* colorname, char* colorcode);
	Description: Tests the ui_theme_get_color function from bask_ui.c.
	InitVersion: 0.0.1
*/
static int tst_ui_getcolor (char* colorname, char* colorcode)
{
	char color[UI_S_THEMECOLOR];
	
	if (strlen (colorname) >= sizeof (color) || strlen (colorcode) >= sizeof (color))
	{
		printf ("UI_GetColor_%s:\t%sFAIL%s\n", colorname, BC_BLD_RED, BC_TXT_RST);
		return TESTS_FAIL;
	}
	
	strcpy (color, colorname);
	
	ui_theme_get_color (color);
	
	if (utils_streq (color, colorcode) == 0)
	{
		printf ("UI_GetColor_%s:\t%sSUCCESS%s\n", colorname, BC_BLD_GREEN, BC_TXT_RST);
		return TESTS_PASS;
	}
	
	printf ("UI_GetColor_%s:\t%sFAIL%s\n", colorname, BC_BLD_RED, BC_TXT_RST);
	return TESTS_FAIL;
}

/* |--------------------------------------------|
   |		   Tests-Export			|
   |--------------------------------------------| */
   
/*
	Function: tst_export_icaldate (void);
	Description: Tests the export_ical_getdatestr function from bask_export.c.
	InitVersion: 0.0.1
*/
static int tst_export_icaldate (void)
{
	char datestr[F_ICAL_S_DATE];
	
	export_ical_getdatestr (datestr, "23/59/59/09/09/2015");
	
	if (datestr == NULL)
	{
		tst_print_fail ("Export_ICal_GetDateStr");
		return TESTS_FAIL;
	}
	
	if (utils_streq (datestr, "20150909T235959") == 0)
	{
		tst_print_success ("Export_ICal_GetDateStr");
		return TESTS_PASS;
	}
	
	tst_print_fail ("Export_ICal_GetDateStr");
	return TESTS_FAIL;
}

/* |--------------------------------------------|
   |		   Tests-Import			|
   |--------------------------------------------| */
   
/*
	Function: tst_import_icaldate (void);
	Description: Tests the import_ical_getdatestr function from bask_import.c.
	InitVersion: 0.0.1
*/
static int tst_import_icaldate (void)
{
	char datestr[F_BB_S_DATE];
	
	import_ical_getdatestr (datestr, "20150909T235959");
	
	if (datestr == NULL)
	{
		tst_print_fail ("Import_ICal_GetDateStr");
		return TESTS_FAIL;
	}
	
	if (utils_streq (datestr, "23/59/59/09/09/2015") == 0)
	{
		tst_print_success ("Import_ICal_GetDateStr");
		return TESTS_PASS;
	}
	
	tst_print_fail ("Import_ICal_GetDateStr");
	return TESTS_FAIL;
}

int main (int argc, char* argv[])
{
	int points;
	
	points = 0;
	
	printf ("Bask-Tests (c) 2015 Marc Volker Dickmann\n\n");
	
	points += tst_core_streq ();
	points += tst_core_parser_str ();
	points += tst_core_parser_int ();
	points += tst_core_parser_short ();
	
	printf ("\n");
	
	points += tst_time_getstr ();
	points += tst_time_gettm_str ();
	
	printf ("\n");
	
	points += tst_config_setstr ();
	
	printf ("\n");
	
	points += tst_task_checkinput ();
	
	printf ("\n");
	
	points += tst_ui_getcolor ("txt_black", "\033[0;30m");
	points += tst_ui_getcolor ("txt_red", "\033[0;31m");
	points += tst_ui_getcolor ("txt_green", "\033[0;32m");
	points += tst_ui_getcolor ("txt_yellow", "\033[0;33m");
	points += tst_ui_getcolor ("txt_blue", "\033[0;34m");
	points += tst_ui_getcolor ("txt_purple", "\033[0;35m");
	points += tst_ui_getcolor ("txt_cyan", "\033[0;36m");
	points += tst_ui_getcolor ("txt_white", "\033[0;37m");
	
	printf ("\n");
	
	points += tst_ui_getcolor ("bld_black", "\033[1;30m");
	points += tst_ui_getcolor ("bld_red", "\033[1;31m");
	points += tst_ui_getcolor ("bld_green", "\033[1;32m");
	points += tst_ui_getcolor ("bld_yellow", "\033[1;33m");
	points += tst_ui_getcolor ("bld_blue", "\033[1;34m");
	points += tst_ui_getcolor ("bld_purple", "\033[1;35m");
	points += tst_ui_getcolor ("bld_cyan", "\033[1;36m");
	points += tst_ui_getcolor ("bld_white", "\033[1;37m");
	
	printf ("\n");
	
	points += tst_ui_getcolor ("und_black", "\033[4;30m");
	points += tst_ui_getcolor ("und_red", "\033[4;31m");
	points += tst_ui_getcolor ("und_green", "\033[4;32m");
	points += tst_ui_getcolor ("und_yellow", "\033[4;33m");
	points += tst_ui_getcolor ("und_blue", "\033[4;34m");
	points += tst_ui_getcolor ("und_purple", "\033[4;35m");
	points += tst_ui_getcolor ("und_cyan", "\033[4;36m");
	points += tst_ui_getcolor ("und_white", "\033[4;37m");
	
	printf ("\n");
	
	points += tst_ui_getcolor ("bak_black", "\033[40m");
	points += tst_ui_getcolor ("bak_red", "\033[41m");
	points += tst_ui_getcolor ("bak_green", "\033[42m");
	points += tst_ui_getcolor ("bak_yellow", "\033[43m");
	points += tst_ui_getcolor ("bak_blue", "\033[44m");
	points += tst_ui_getcolor ("bak_purple", "\033[45m");
	points += tst_ui_getcolor ("bak_cyan", "\033[46m");
	points += tst_ui_getcolor ("bak_white", "\033[47m");
	
	printf ("\n");
	
	points += tst_export_icaldate ();
	
	printf ("\n");
	
	points += tst_import_icaldate ();
	
	tst_print_summary (points);
	
	return 0;
}
