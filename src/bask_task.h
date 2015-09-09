#ifndef __BASK_TASK_H__
#define __BASK_TASK_H__

struct bask_task
{
	int n;
	int t_active;
	int t_id;
	int t_priority;
	int t_state;
	char t_finished[51];
	char t_project[51];
	char t_description[201];
	struct bask_task* next;
};

void task_free_ll (struct bask_task**);
int task_insert (struct bask_task**, int, int, int, int, int, char*, char*);
void task_create (bask_core*, struct bask_task**, int, char*, char*);
int task_remove (bask_core*, struct bask_task**, int);
int task_modificate (bask_core*, struct bask_task**, int, int, int, int, char*, char*);
int task_deactivate (bask_core*, struct bask_task**, int);
int task_modify (bask_core*, struct bask_task**, int, int, int, int, char*, char*);
int task_finish (bask_core* tcore, struct bask_task**, int); 
int task_search (bask_core*, struct bask_task**, struct bask_task**, char*);

#endif /* __BASK_TASK_H__ */
