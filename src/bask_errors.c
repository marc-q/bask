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
	Function: errors_useinit (void);
	Description: Prints the use bask init message.
	InitVersion: 0.0.1
*/
void errors_useinit (void)
{
	printf ("Use: '$ %s init' to use Bask.\n", P_CMD);
	printf ("Warning: Overides all data!\n");
}

/*
	Function: errors_filenotfound (char* filename);
	Description: Prints the error for when we can't find a file.
	InitVersion: 0.0.1
*/
void errors_filenotfound (char* filename)
{
	printf ("ERROR: File %s not found!\n", filename);
}

/*
	Function: errors_filenotopened (char* filename);
	Description: Prints the error for when we can't open a file.
	InitVersion: 0.0.1
*/
void errors_filenotopened (char* filename)
{
	printf ("ERROR: File %s could'nt be opened!\n", filename);
}

/*
	Function: errors_filenotwritten (char* filename);
	Description: Prints the error for when we can't write a file.
	InitVersion: 0.0.1
*/
void errors_filenotwritten (char* filename)
{
	printf ("ERROR: File %s could'nt be written!\n", filename);
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

/*
	Function: errors_timenotgot (void);
	Description: Prints the error for when we can't get the time.
	InitVersion: 0.0.1
*/
void errors_timenotgot (void)
{
	printf ("ERROR: Cannot get the time!\n");
}

/*
	Function: errors_lengthtobig (char* variable);
	Description: Prints the error for when the length of something is to big.
	InitVersion: 0.0.1
*/
void errors_lengthtobig (char* variable)
{
	printf ("ERROR: The length of the %s is to big!\n", variable);
}

/*
	Function: errors_notunsigned (char* variable);
	Description: Prints the error for when the value of something is not unsigned.
	InitVersion: 0.0.1
*/
void errors_notunsigned (char* variable)
{
	printf ("ERROR: The value of %s must be a positive number!\n", variable);
	printf ("ERROR: Converting to a postive number!\n");
}

/*
	Function: errors_outofrange_int (char* variable);
	Description: Prints the error for when the value of something is out of range (int).
	InitVersion: 0.0.1
*/
void errors_outofrange_int (char* variable, int min, int max)
{
	printf ("ERROR: The value of %s must be between %i and %i!\n", variable, min, max);
}

/*
	Function: errors_outofrange_float (char* variable);
	Description: Prints the error for when the value of something is out of range (float).
	InitVersion: 0.0.1
*/
void errors_outofrange_float (char* variable, float min, float max)
{
	printf ("ERROR: The value of %s must be between %f and %f!\n", variable, min, max);
}
