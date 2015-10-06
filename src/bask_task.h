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

int task_check_input (char*, char*, char*, char*, int);

void task_free_ll (struct bask_task**);
int task_insert (struct bask_task**, int, int, int, int, int, char*, char*, char*, char*);
int task_remove (struct bask_task**, int);
int task_modificate (struct bask_task**, int, int, int, int, char*, char*, char*, char*);

int task_create (bask_core*, struct bask_task**, int, char*, char*);
void task_deactivate (struct bask_task**, int);
int task_finish (struct bask_task**, int); 

int task_search (bask_core*, struct bask_task**, struct bask_task**, char*);

void task_create_cmd (bask_core*, struct bask_task**, int, char*, char*);
void task_remove_cmd (bask_core*, struct bask_task**, int);
void task_modificate_cmd (bask_core*, struct bask_task**, int, int, int, int, char*, char*, char*, char*);
void task_deactivate_cmd (bask_core*, struct bask_task**, int);
void task_finish_cmd (bask_core*, struct bask_task**, int); 

#endif /* __BASK_TASK_H__ */
