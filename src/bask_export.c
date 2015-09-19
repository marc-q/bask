/* Copyright 2015 Marc Volker Dickmann */
#include <stdio.h>
#include <string.h>
#include "../lib/dutils.h"
#include "bask_core.h"
#include "bask_errors.h"
#include "bask_task.h"

/*
	Function: export_web (bask_core* tcore, struct bask_task** first, char* filename);
	Description: Exports all tasks to an html file named filename.
	InitVersion: 0.0.1
*/
int export_web (bask_core* tcore, struct bask_task** first, char* filename)
{
	int i = 0;
	char prefix[30], pri[4];
	FILE *webfile;
	struct bask_task* ptr = *first;
	
	webfile = fopen (filename, "w");
	
	if (webfile == NULL)
	{
		/*printf ("ERROR: Couldn't write the export file.\n");*/
		errors_filenotwritten (filename);
		return -1;
	}
	
	fprintf (webfile, "<html>\n<head>\n<title>BaskExport</title>\n");
	
	fprintf (webfile, "</head>\n<body>\n<h1>BaskExport</h1>\n<table>\n<tbody>\n");
	fprintf (webfile, "<tr><td>ID</td><td>Project</td><td>Priority</td><td>Description</td></tr>\n");
	
	while (ptr != NULL)
	{
		if (ptr->t_active != 0)
		{
			switch (ptr->t_priority)
			{
				case 0:
					strcpy (prefix, BC_BLD_WHITE);
					strcpy (pri, "L");
					break;
				case 1:
					strcpy (prefix, BC_BLD_GREEN);
					strcpy (pri, "I");
					break;
				case 2:
					strcpy (prefix, BC_BLD_BLUE);
					strcpy (pri, "T");
					break;
				case 3:
					strcpy (prefix, BC_BLD_RED);
					strcpy (pri, "C");
					break;
				default:
					strcpy (prefix, BC_TXT_WHITE);
					strcpy (pri, "N");
					break;
			}

			fprintf (webfile, "<tr><td>%i</td><td>%s</td><td>%s</td><td>%s</td></tr>\n", ptr->t_id, ptr->t_project, pri, ptr->t_description);
			
			i++;
		}
		
		ptr = ptr->next;
	}
	
	fprintf (webfile, "</tbody>\n</table>\n</body>\n</html>\n");
	
	fclose (webfile);
}
