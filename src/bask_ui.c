/* Copyright 2015 Marc Volker Dickmann */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../lib/dutils.h"
#include "bask_core.h"
#include "bask_ui.h"

/* |--------------------------------------------|
   |			Core			|
   |--------------------------------------------| */

/*
	Function: view_print_nspaces (int amount);
	Description: Prints amount spaces.
	InitVersion: 0.0.1
*/
void view_print_nspaces (int amount)
{
	int i;
	
	for (i = 0; i < amount; i++)
	{
		printf (" ");
	}
}

/*
	Function: view_print_nspaces_str (char* tag, int length);
	Description: Prints length spaces minus the length of tag.
	InitVersion: 0.0.1
*/
void view_print_nspaces_str (char* tag, int length)
{
	size_t tl = strlen (tag);
	
	if (length > 0)
	{
		view_print_nspaces (length-tl);
	}
}

/* |--------------------------------------------|
   |			Misc			|
   |--------------------------------------------| */

/*
	Function: view_misc_print_progress (float p, char* bakcolor);
	Description: Prints a progress bar filled p%.
	InitVersion: 0.0.1
*/
void view_misc_print_progress (float p, char* bakcolor)
{
	printf ("%s", bakcolor);
	
	view_print_nspaces ((p/5)+1);
	
	printf ("%s\n", BC_TXT_RST);
}

/* |--------------------------------------------|
   |			Table			|
   |--------------------------------------------| */

/*
	Function: view_tbl_print_title (char* name, int len_underline, int len_normal);
	Description: Prints a table title.
	InitVersion: 0.0.1
*/
void view_tbl_print_title (char* name, int len_underline, int len_normal)
{	
	printf ("%s", name);
	view_print_nspaces_str (name, len_underline);
	printf ("\033[0m");
	
	view_print_nspaces (len_normal);
	
	printf ("\033[4m");
}

/*
	Function: view_tbl_print_field_str (char* value, int len_pre, int len_suf);
	Description: Prints a table field with a str value.
	InitVersion: 0.0.1
*/
void view_tbl_print_field_str (char* value, int len_pre, int len_suf)
{
	view_print_nspaces_str (value, len_pre);
	
	printf ("%s", value);
	
	view_print_nspaces_str (value, len_suf);
}

/*
	Function: view_tbl_print_field_int (int value, int len_pre, int len_suf);
	Description: Prints a table field with a int value.
	InitVersion: 0.0.1
*/
void view_tbl_print_field_int (int value, int len_pre, int len_suf)
{	
	if (len_pre > 0)
	{
		view_print_nspaces (len_pre);
	}
	
	printf ("%i", value);
	
	if (len_suf > 0)
	{
		view_print_nspaces (len_suf);
	}
}
