/* Copyright 2015 Marc Volker Dickmann */
#include <stdio.h>
#include <string.h>
#include "bask_core.h"

/* |--------------------------------------------|
   |			Utils			|
   |--------------------------------------------| */
   
/*
	Function: parser_get_str (char* token, char* key, char* out, size_t outsize, char* saveptr);
	Description: Parses a row and return the value if the key is right.
	InitVersion: 0.0.1
*/
int parser_get_str (char* token, char* key, char* out, size_t outsize, char* saveptr)
{
	if (strncmp (token, key, strlen (token)) == 0)
	{
		token = strtok_r (NULL, BASKSEP, &saveptr);

		if (strlen (token) < outsize)
		{
			strcpy (out, token);
		}

		token = strtok_r (NULL, BASKSEP, &saveptr);

		return 0;
	}

	return -1;
}

/*
	Function: parser_get_int (char* token, char* key, int* out, char* saveptr);
	Description: Parses a row and return the value if the key is right.
	InitVersion: 0.0.1
*/
int parser_get_int (char* token, char* key, int* out, char* saveptr)
{
	if (strncmp (token, key, strlen (token)) == 0)
	{
		token = strtok_r (NULL, BASKSEP, &saveptr);

		if (isdigit (token[0]) != 0)
		{
			*out = atoi (token);
		}

		token = strtok_r (NULL, BASKSEP, &saveptr);

		return 0;
	}

	return -1;
}
