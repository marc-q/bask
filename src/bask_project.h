#ifndef __BASK_PROJECT_H__
#define __BASK_PROJECT_H__

struct bask_project
{
	short p_active;
	float p_complete;
	float p_tasks;
	char* p_name;
	struct bask_project* next;
};

void project_free_ll (struct bask_project**);
short project_insert (struct bask_project**, short, short, char*);

#endif /* __BASK_PROJECT_H__ */
