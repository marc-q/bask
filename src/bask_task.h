#ifndef __BASK_TASK_H__
#define __BASK_TASK_H__

struct bask_task
{
	int n;
	int t_active;
	int t_id;
	int t_priority;
	int t_state;
	char t_added[T_S_ADDED];
	char t_finished[T_S_FINISHED];
	char t_project[T_S_PROJECT];
	char t_description[T_S_DESCRIPTION];
	struct bask_task* next;
};

void task_free_ll (struct bask_task**);
int task_insert (struct bask_task**, int, int, int, int, int, char*, char*, char*, char*);
int task_create (bask_core*, struct bask_task**, int, char*, char*);
int task_remove (bask_core*, struct bask_task**, int);
void task_remove_cmd (bask_core*, struct bask_task**, int);
int task_modificate (bask_core*, struct bask_task**, int, int, int, int, char*, char*, char*, char*);
void task_deactivate (bask_core*, struct bask_task**, int);
void task_modify (bask_core*, struct bask_task**, int, int, int, int, char*, char*, char*, char*);
int task_finish (bask_core* tcore, struct bask_task**, int); 
int task_search (bask_core*, struct bask_task**, struct bask_task**, char*);

#endif /* __BASK_TASK_H__ */
