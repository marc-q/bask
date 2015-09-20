#ifndef __BASK_PROJECT_H__
#define __BASK_PROJECT_H__

struct bask_project
{
	int n;
	int p_active;
	int p_id;
	float p_complete;
	float p_tasks;
	char p_name[T_S_PROJECT];
	struct bask_project* next;
};

void project_free_ll (struct bask_project**);
int project_insert (struct bask_project**, int, int, int, char*);

#endif /* __BASK_PROJECT_H__ */
