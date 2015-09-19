/* Copyright 2015 Marc Volker Dickmann */
#include <stdio.h>
#include <string.h>
#include "bask_core.h"
#include "bask_errors.h"

/*
	Function: errors_print_errormsg (char* errormsg);
	Description: Prints the error message.
	InitVersion: 0.0.1
*/
static void errors_print_errormsg (char* errormsg)
{
	printf ("ERROR: %s\n", errormsg);
}

/*
	Function: errors_print_useinit (void);
	Description: Prints the use bask init message.
	InitVersion: 0.0.1
*/
static void errors_print_useinit (void)
{
	printf ("Use: '$ %s init' to use Bask.\n", P_CMD);
	printf ("Warning: Overides all data.\n");
}

/*
	Function: errors_filenotfound (char* filename);
	Description: Prints the error for when we can't find a file.
	InitVersion: 0.0.1
*/
void errors_filenotfound (char* filename)
{
	printf ("ERROR: File %s not found.\n", filename);
	errors_print_useinit ();
}

/*
	Function: errors_filenotopened (char* filename);
	Description: Prints the error for when we can't open a file.
	InitVersion: 0.0.1
*/
void errors_filenotopened (char* filename)
{
	printf ("ERROR: File %s could'nt be opened.\n", filename);
	errors_print_useinit ();
}

/*
	Function: errors_filenotwritten (char* filename);
	Description: Prints the error for when we can't write a file.
	InitVersion: 0.0.1
*/
void errors_filenotwritten (char* filename)
{
	printf ("ERROR: File %s could'nt be written.\n", filename);
	errors_print_useinit ();
}

/*
	Function: errors_homedirnotgot (void);
	Description: Prints the error for when we can't get the home dir.
	InitVersion: 0.0.1
*/
void errors_homedirnotgot (void)
{
	printf ("ERROR: Cannot get home directory!\n");
}
