/* Copyright 2015 - 2016 Marc Volker Dickmann */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../lib/dutils.h"
#include "../src/bask_core.h"
#include "../src/bask_time.h"
#include "../src/bask_config.h"
#include "../src/bask_task.h"
#include "../src/bask_filter.h"
#include "../src/bask_ui.h"
#include "../src/bask_export.h"
#include "../src/bask_import.h"

#define TESTS_AMOUNT 55
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
	Function: tst_core_atos (void);
	Description: Tests the utils_atos function from bask_core.c.
	InitVersion: 0.0.1
*/
static int tst_core_atos (void)
{
	short tmp;
	
	tmp = 0;
	
	if (utils_atos (&tmp, "2015") == 0 && tmp == 2015 &&
	    utils_atos (&tmp, "+2016") == 0 && tmp == 2016 &&
	    utils_atos (&tmp, "-2017") == 0 && tmp == -2017 &&
	    utils_atos (&tmp, "a2015") == -1 &&
	    utils_atos (&tmp, "32768") == -2 &&
	    utils_atos (&tmp, "-32769") == -2)
	{
		tst_print_success ("Core_Atos");
		return TESTS_PASS;
	}
	
	tst_print_fail ("Core_Atos");
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
	char saveptr[200];

	strcpy (instr, "test=this is a str test with an ;.\n");
	
	parser_get_str (instr, "test=", outstr, sizeof (outstr), BASKSEP, saveptr);
	
	if (utils_streq (outstr, "this is a str test with an ;.") == 0)
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
	char saveptr[200];
	
	strcpy (instr, "test=2015\n");
	
	parser_get_int (instr, "test=", &outint, BASKSEP, saveptr);
	
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
	int passed;
	short out;
	char instr[50];
	char saveptr[200];
	
	passed = FALSE;
	
	strcpy (instr, "test=2015\n");
	
	parser_get_short (instr, "test=", &out, BASKSEP, saveptr);
	
	if (out == 2015)
	{
		passed = TRUE;
	}
	
	strcpy (instr, "test=+2015\n");
	
	parser_get_short (instr, "test=", &out, BASKSEP, saveptr);
	
	if (passed == TRUE &&
	    out != 2015)
	{
		passed = FALSE;
	}
	
	strcpy (instr, "test=-2015\n");
	
	parser_get_short (instr, "test=", &out, BASKSEP, saveptr);
	
	if (passed == TRUE &&
	    out != -2015)
	{
		passed = FALSE;
	}
	
	strcpy (instr, "test=32768\n");
	
	if (passed == TRUE && 
	    parser_get_short (instr, "test=", &out, BASKSEP, saveptr) != -1)
	{
		passed = FALSE;
	}

	strcpy (instr, "test=-32769\n");

	if (passed == TRUE && 
	    parser_get_short (instr, "test=", &out, BASKSEP, saveptr) == -1)
	{
		tst_print_success ("Core_Parser_Short");
		return TESTS_PASS;
	}

	tst_print_fail ("Core_Parser_Short");
	return TESTS_FAIL;
}

/*
	Function: tst_core_get_baskpath (void);
	Description: Tests the bask_get_baskpath function from bask_core.c.
	InitVersion: 0.0.1
*/
static int tst_core_get_baskpath (void)
{
	char out[200];
	bask_core tcore;
		
	strcpy (tcore.path_baskpath, "/test");
	strcat (tcore.path_baskpath, "/.local/share/bask/");
	
	bask_get_baskpath (&tcore, out, "test.txt");
	
	if (utils_streq (out, "/test/.local/share/bask/test.txt") == 0)
	{
		tst_print_success ("Core_Get_Baskpath");
		return TESTS_PASS;
	}

	tst_print_fail ("Core_Get_Baskpath");
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

/*
	Function: tst_time_gethours (void);
	Description: Tests the time_get_hours function from bask_time.c.
	InitVersion: 0.0.1
*/
static int tst_time_gethours (void)
{
	if (time_get_hours ("23/59/59/09/09/2015") == 23 &&
	    time_get_hours ("23/59/59/9/9/2015") == -1)
	{
		tst_print_success ("Time_Get_Hours");
		return TESTS_PASS;
	}
	
	tst_print_fail ("Time_Get_Hours");
	return TESTS_FAIL;
}

/*
	Function: tst_time_getminutes (void);
	Description: Tests the time_get_minutes function from bask_time.c.
	InitVersion: 0.0.1
*/
static int tst_time_getminutes (void)
{
	
	if (time_get_minutes ("23/59/59/09/09/2015") == 59 &&
	    time_get_minutes ("23/59/59/9/9/2015") == -1)
	{
		tst_print_success ("Time_Get_Minutes");
		return TESTS_PASS;
	}
	
	tst_print_fail ("Time_Get_Minutes");
	return TESTS_FAIL;
}

/*
	Function: tst_time_getseconds (void);
	Description: Tests the time_get_seconds function from bask_time.c.
	InitVersion: 0.0.1
*/
static int tst_time_getseconds (void)
{
	
	if (time_get_seconds ("23/59/59/09/09/2015") == 59 &&
	    time_get_seconds ("23/59/59/9/9/2015") == -1)
	{
		tst_print_success ("Time_Get_Seconds");
		return TESTS_PASS;
	}
	
	tst_print_fail ("Time_Get_Seconds");
	return TESTS_FAIL;
}

/*
	Function: tst_time_getday (void);
	Description: Tests the time_get_day function from bask_time.c.
	InitVersion: 0.0.1
*/
static int tst_time_getday (void)
{
	
	if (time_get_day ("23/59/59/09/09/2015") == 9 &&
	    time_get_day ("23/59/59/9/9/2015") == -1)
	{
		tst_print_success ("Time_Get_Day");
		return TESTS_PASS;
	}
	
	tst_print_fail ("Time_Get_Day");
	return TESTS_FAIL;
}

/*
	Function: tst_time_getmonth (void);
	Description: Tests the time_get_month function from bask_time.c.
	InitVersion: 0.0.1
*/
static int tst_time_getmonth (void)
{
	
	if (time_get_month ("23/59/59/09/09/2015") == 9 &&
	    time_get_month ("23/59/59/9/9/2015") == -1)
	{
		tst_print_success ("Time_Get_Month");
		return TESTS_PASS;
	}
	
	tst_print_fail ("Time_Get_Month");
	return TESTS_FAIL;
}

/*
	Function: tst_time_getyear (void);
	Description: Tests the time_get_year function from bask_time.c.
	InitVersion: 0.0.1
*/
static int tst_time_getyear (void)
{
	
	if (time_get_year ("23/59/59/09/09/2015") == 2015 &&
	    time_get_year ("23/59/59/9/9/2015") == -1)
	{
		tst_print_success ("Time_Get_Year");
		return TESTS_PASS;
	}
	
	tst_print_fail ("Time_Get_Year");
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
	
	passed = FALSE;
	
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
	    BITGET (tcore.t_options, T_O_DESCRIPTIONBREAK) == TRUE)
	{
		passed = TRUE;
	}
	
	if (passed == TRUE &&
	    config_set_str (&tcore, "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum.Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum.Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum. Lorem Ipsum Lorem .") == CONFIG_ERR_SS_CONFLINE &&
	    config_set_str (&tcore, "task_project_min=201;") == CONFIG_ERR_SS_PROJMIN &&
	    config_set_str (&tcore, "task_description_max=201;") == CONFIG_ERR_SS_DESCMAX &&
	    config_set_str (&tcore, "task_description_min=201;") == CONFIG_ERR_SS_DESCMIN &&
	    config_set_str (&tcore, "task_description_break=2;") == CONFIG_ERR_SS_DESCBREAK)
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
	Function: tst_task_checkinputnbrs (void);
	Description: Tests the task_check_input_nbrs function from bask_task.c.
	InitVersion: 0.0.1
*/
static int tst_task_checkinputnbrs (void)
{	
	if (task_check_input_nbrs (-1, 0, FALSE, FALSE) == TASK_ERR_CHECK_ID &&
	    task_check_input_nbrs (0, TASK_PRIORITY_MAX+1, FALSE, FALSE) == TASK_ERR_CHECK_PRIORITY &&
	    task_check_input_nbrs (0, TASK_PRIORITY_MIN-1, FALSE, FALSE) == TASK_ERR_CHECK_PRIORITY &&
	    task_check_input_nbrs (0, 0, 2, FALSE) == TASK_ERR_CHECK_ACTIVE &&
	    task_check_input_nbrs (0, 0, -1, FALSE) == TASK_ERR_CHECK_ACTIVE &&
	    task_check_input_nbrs (0, 0, FALSE, FALSE) == 0)
	{
		tst_print_success ("Task_Check_Input_Nbrs");
		return TESTS_PASS;
	}
	
	tst_print_fail ("Task_Check_Input_Nbrs");
	return TESTS_FAIL;
}

/*
	Function: tst_task_checkinput (void);
	Description: Tests the task_check_input function from bask_task.c.
	InitVersion: 0.0.1
*/
static int tst_task_checkinput (void)
{
	int passed;
	bask_core tcore;
	
	passed = FALSE;
	
	tcore.t_descriptionmax = 50;
	tcore.t_options = 0;
	
	if (task_check_input (&tcore, "23/59/59/09/09/2015", "23/59/59/09/09/2015", "23/59/59/09/09/2015", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", FALSE) == 0 &&
	    task_check_input (&tcore, "23/59/59/09/09/02015", "23/59/59/09/09/2015", "23/59/59/09/09/2015", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", FALSE) == TASK_ERR_CHECK_ADDED &&
	    task_check_input (&tcore, "23/59/59/09/09/2015", "23/59/59/09/09/02015", "23/59/59/09/09/2015", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", FALSE) == TASK_ERR_CHECK_DUE &&
	    task_check_input (&tcore, "23/59/59/09/09/2015", "23/59/59/09/09/2015", "23/59/59/09/09/02015", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", FALSE) == TASK_ERR_CHECK_FINISHED &&
	    task_check_input (&tcore, "23/59/59/09/09/2015", "23/59/59/09/09/2015", "23/59/59/09/09/2015", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum.", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", FALSE) == TASK_ERR_CHECK_PROJECT &&
	    task_check_input (&tcore, "23/59/59/09/09/2015", "23/59/59/09/09/2015","23/59/59/09/09/2015", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum.", FALSE) == TASK_ERR_CHECK_DESCRIPTION)
	{
		passed = FALSE;
	}
	
	tcore.t_options ^= BITCOPY (TRUE, 0, tcore.t_options, T_O_DESCRIPTIONBREAK);
	
	if (passed == FALSE && task_check_input (&tcore, "23/59/59/09/09/2015", "23/59/59/09/09/2015", "23/59/59/09/09/2015", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum.Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum.Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum. Lorem Ipsum Lorem  .", 0) == TASK_ERR_CHECK_DESCRIPTION &&
	task_check_input (&tcore, "23/59/59/09/09/2015", "23/59/59/09/09/2015", "23/59/59/09/09/2015", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum  .", "Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum.Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum.Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum. Lorem Ipsum Lorem .", 0) == 0)
	{
		tst_print_success ("Task_Check_Input");
		return TESTS_PASS;
	}

	tst_print_fail ("Task_Check_Input");
	return TESTS_FAIL;
}

/*
	Function: tst_task_getpriority (void);
	Description: Tests the task_get_priority function from bask_task.c.
	InitVersion: 0.0.1
*/
static int tst_task_getpriority (void)
{	
	if (task_get_priority ("0") == 0 &&
	    task_get_priority ("1") == 1 &&
	    task_get_priority ("2") == 2 &&
	    task_get_priority ("3") == 3 &&
	    task_get_priority ("l") == 0 &&
	    task_get_priority ("i") == 1 &&
	    task_get_priority ("t") == 2 &&
	    task_get_priority ("c") == 3 &&
	    task_get_priority ("L") == 0 &&
	    task_get_priority ("I") == 1 &&
	    task_get_priority ("T") == 2 &&
	    task_get_priority ("C") == 3 &&
	    task_get_priority ("b") == -1 &&
	    task_get_priority ("B") == -1)
	{
		tst_print_success ("Task_Get_Priority");
		return TESTS_PASS;
	}
	
	tst_print_fail ("Task_Get_Priority");
	return TESTS_FAIL;
}

/* |--------------------------------------------|
   |		    Tests-Filter		|
   |--------------------------------------------| */

/*
	Function: tst_filter_init (void);
	Description: Tests the filter_init function from bask_filter.c.
	InitVersion: 0.0.1
*/
static int tst_filter_init (void)
{
	bask_filter bfilter;
	
	filter_init (&bfilter, TRUE, -1, 2, 9, 9, 2015);
	
	if (BITGET (bfilter.flags, T_FLTR_ACTIVE) == FLTR_ON &&
	    BITGET (bfilter.task.t_flags, TASK_FLAG_ACTIVE) == TRUE &&
	    BITGET (bfilter.flags, T_FLTR_FINISHED) == FLTR_OFF &&
	    BITGET (bfilter.flags, T_FLTR_PRIORITY) == FLTR_ON &&
	    bfilter.task.t_priority == 2 &&
	    BITGET (bfilter.flags, T_FLTR_DAY) == FLTR_ON &&
	    bfilter.tday == 9 &&
	    BITGET (bfilter.flags, T_FLTR_MONTH) == FLTR_ON &&
	    bfilter.tmonth == 9 &&
	    BITGET (bfilter.flags, T_FLTR_YEAR) == FLTR_ON &&
	    bfilter.tyear == 2015
	    )
	{
		tst_print_success ("Filter_Init");
		return TESTS_PASS;
	}
	
	tst_print_fail ("Filter_Init");
	return TESTS_FAIL;
}

/*
	Function: tst_filter_checktask (void);
	Description: Tests the filter_check_task function from bask_filter.c.
	InitVersion: 0.0.1
*/
static int tst_filter_checktask (void)
{
	int passed;
	bask_filter bfilter;
	struct bask_task btask;
	
	passed = FALSE;
	
	btask.t_id = 0;
	btask.t_flags = 0;
	btask.t_flags ^= BITCOPY (1, 0, btask.t_flags, TASK_FLAG_ACTIVE);
	btask.t_flags ^= BITCOPY (0, 0, btask.t_flags, TASK_FLAG_FINISHED);
	btask.t_priority = 2;
	
	strcpy (btask.t_added, "23/59/59/09/09/2015");
	
	filter_init (&bfilter, TRUE, -1, 2, 9, 9, 2015);
	
	if (filter_check_task (&bfilter, &btask) == TRUE)
	{
		passed = TRUE;
	}
	
	filter_init (&bfilter, -1, -1, -1, -1, -1, -1);
	
	if (passed == TRUE &&
	    filter_check_task (&bfilter, &btask) == TRUE)
	{
		passed = TRUE;
	}
	else
	{
		passed = FALSE;
	}
	
	filter_init (&bfilter, FALSE, -1, -1, 9, 9, 2015);
	
	if (passed == TRUE &&
	    filter_check_task (&bfilter, &btask) == FALSE)
	{
		passed = TRUE;
	}
	else
	{
		passed = FALSE;
	}
	
	filter_init (&bfilter, -1, 1, -1, 9, 9, 2015);
	
	if (passed == TRUE &&
	    filter_check_task (&bfilter, &btask) == FALSE)
	{
		passed = TRUE;
	}
	else
	{
		passed = FALSE;
	}
	
	filter_init (&bfilter, -1, -1, -1, 9, 8, 2015);
	
	if (passed == TRUE &&
	    filter_check_task (&bfilter, &btask) == FALSE)
	{
		tst_print_success ("Filter_Check_Task");
		return TESTS_PASS;
	}
	
	tst_print_fail ("Filter_Check_Task");
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
	Function: tst_import_csvparser (void);
	Description: Tests the import_csv_parser function from bask_import.c.
	InitVersion: 0.0.1
*/
static int tst_import_csvparser (void)
{
	char line[200];
	char *token, *saveptr;
	bask_core tcore;
	struct bask_task* first = NULL;
	
	tcore.baskbin_uid = 1;
	tcore.tc_amount = 0;
	
	strcpy (line, "1;1;2;\"23/59/59/09/09/2015\";\"NONE\";\"NONE\";\"Test\";\"This is a test.\"\n");
	token = strtok_r (line, ";", &saveptr); 
	
	import_csv_parser (&tcore, &first, token, saveptr);
	
	if (first == NULL)
	{
		tst_print_fail ("Import_CSV_Parser");
		return TESTS_FAIL;
	}
	
	if (first->t_id == 1 &&
	    first->t_priority == 2 &&
	    BITGET (first->t_flags, TASK_FLAG_ACTIVE) == TRUE &&
	    BITGET (first->t_flags, TASK_FLAG_FINISHED) == FALSE &&
	    utils_streq (first->t_added, "23/59/59/09/09/2015") == 0 &&
	    utils_streq (first->t_due, "NONE") == 0 &&
	    utils_streq (first->t_finished, "NONE") == 0 &&
	    utils_streq (first->t_project, "Test") == 0 &&
	    utils_streq (first->t_description, "This is a test.") == 0)
	{
		task_free_ll (&first);
		tst_print_success ("Import_CSV_Parser");
		return TESTS_PASS;
	}
	
	task_free_ll (&first);
	tst_print_fail ("Import_CSV_Parser");
	return TESTS_FAIL;
}
   
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
	
	printf ("Bask-Tests (c) 2015 - 2016 Marc Volker Dickmann\n\n");
	
	points += tst_core_streq ();
	points += tst_core_atos ();
	points += tst_core_parser_str ();
	points += tst_core_parser_int ();
	points += tst_core_parser_short ();
	points += tst_core_get_baskpath ();
	
	printf ("\n");
	
	points += tst_time_getstr ();
	points += tst_time_gettm_str ();
	points += tst_time_gethours ();
	points += tst_time_getminutes ();
	points += tst_time_getseconds ();
	points += tst_time_getday ();
	points += tst_time_getmonth ();
	points += tst_time_getyear ();
	
	printf ("\n");
	
	points += tst_config_setstr ();
	
	printf ("\n");
	
	points += tst_task_checkinputnbrs ();
	points += tst_task_checkinput ();
	points += tst_task_getpriority ();
	
	printf ("\n");
	
	points += tst_filter_init ();
	points += tst_filter_checktask ();
	
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
	
	points += tst_import_csvparser ();
	points += tst_import_icaldate ();
	
	tst_print_summary (points);
	
	return 0;
}
