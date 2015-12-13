/* Copyright 2015 Marc Volker Dickmann */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bask_core.h"

/* |--------------------------------------------|
   |			Utils			|
   |--------------------------------------------| */

/*
	Function: parser_get_str (char* token, char* key, char* out, size_t outsize, char* septags, char* saveptr);
	Description: Parses a row and return the value if the key is right.
	InitVersion: 0.0.1
*/
int parser_get_str (char* token, char* key, char* out, size_t outsize, char* septags, char* saveptr)
{
	if (utils_streq (token, key) == 0)
	{
		token = strtok_r (NULL, septags, &saveptr);

		if (strlen (token) < outsize)
		{
			strcpy (out, token);
		}

		token = strtok_r (NULL, septags, &saveptr);

		return 0;
	}

	return -1;
}

/*
	Function: parser_get_int (char* token, char* key, int* out, char* septags, char* saveptr);
	Description: Parses a row and return the value if the key is right.
	InitVersion: 0.0.1
*/
int parser_get_int (char* token, char* key, int* out, char* septags, char* saveptr)
{
	if (utils_streq (token, key) == 0)
	{
		token = strtok_r (NULL, septags, &saveptr);

		if (isdigit (token[0]) != 0)
		{
			*out = atoi (token);
		}

		token = strtok_r (NULL, septags, &saveptr);

		return 0;
	}

	return -1;
}

/*
	Function: utils_streq (char* one, char* two);
	Description: Check's if two strings are equal and have the same length.
	InitVersion: 0.0.1
*/
int utils_streq (char* one, char* two)
{
	if (strlen (one) != strlen (two))
	{
		return -1;
	}
	
	return strncmp (one, two, strlen (one));
}
