/* Copyright 2015 Marc Volker Dickmann */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/dutils.h"
#include "../src/bask_core.h"

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
	printf ("%s: %sSUCCESS%s\n", tstname, BC_BLD_GREEN, BC_TXT_RST);
}

/*
	Function: tst_print_fail (char* tstname);
	Description: Prints the test failure message.
	InitVersion: 0.0.1
*/
static void tst_print_fail (char* tstname)
{
	printf ("%s: %sFAIL%s\n", tstname, BC_BLD_RED, BC_TXT_RST);
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
	if (utils_streq ("this is a str test." , "this is a str test.") == 0 &&
	    utils_streq ("this is a str test" , "this is a str test.") == -1 &&
	    utils_streq ("this is a str test." , "this is a str test") == -1)
	{
		tst_print_success ("CoreStreq");
	}
	else
	{
		tst_print_fail ("CoreStreq");
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
		return -1;
	}
	
	parser_get_str (token, "test", outstr, sizeof (outstr), BASKSEP, saveptr);
	
	if (utils_streq (outstr, "this is a str test.") == 0)
	{
		tst_print_success ("CoreParserStr");
	}
	else
	{
		tst_print_fail ("CoreParserStr");
	}
	
	strcpy (instr, "test=2015;\n");
	
	token = strtok_r (instr, BASKSEP, &saveptr);
	
	if (token == NULL)
	{
		return -1;
	}
	
	parser_get_int (token, "test", &outint, BASKSEP, saveptr);
	
	if (outint == 2015)
	{
		tst_print_success ("CoreParserInt");
	}
	else
	{
		tst_print_fail ("CoreParserInt");
	}
	
	return 0;
}

int main (int argc, char* argv[])
{
	printf ("Bask-Tests (c) 2015 Marc Volker Dickmann\n\n");
	
	tst_core_streq ();
	tst_core_parser ();
	
	return 0;
}
