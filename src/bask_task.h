#ifndef __BASK_TASK_H__
#define __BASK_TASK_H__

#define TASK_FLAG_ACTIVE 0
#define TASK_FLAG_FINISHED 1

#define TASK_PRIORITY_MIN 0
#define TASK_PRIORITY_MAX 3

struct bask_task
{
	unsigned int n; /* Only positive values. */
	unsigned int t_id; /* Only positive values. */
	short t_priority; /* 2 Bytes should be more than enough for this. */
	short t_flags;
	char t_added[T_S_ADDED];
	char t_due[T_S_DUE];
	char t_finished[T_S_FINISHED];
	char t_project[T_S_PROJECT];
	char t_description[T_S_DESCRIPTION];
	struct bask_task* next;
};

int task_check_input_nbrs (int, short, short, short);
int task_check_input (bask_core*, char*, char*, char*, char*, char*, short);
int task_get_priority (char*);

void task_free_ll (struct bask_task**);
int task_insert (struct bask_task**, unsigned int, unsigned int, int, short, int, char*, char*, char*, char*, char*);
int task_remove (struct bask_task**, unsigned int);
int task_modificate (struct bask_task**, unsigned int, int, int, short, char*, char*, char*, char*, char*);

int task_create (bask_core*, struct bask_task**, short, char*, char*);
void task_deactivate (struct bask_task**, unsigned int);
int task_due (struct bask_task**, unsigned int, char*); 
int task_finish (struct bask_task**, unsigned int); 

int task_search (bask_core*, struct bask_task**, struct bask_task**, char*);

void task_create_cmd (bask_core*, struct bask_task**, int, char*, char*);
void task_remove_cmd (bask_core*, struct bask_task**, unsigned int);
void task_modificate_cmd (bask_core*, struct bask_task**, unsigned int, int, int, int, char*, char*, char*, char*, char*);
void task_deactivate_cmd (bask_core*, struct bask_task**, unsigned int);
void task_due_cmd (bask_core*, struct bask_task**, unsigned int, char*);
void task_finish_cmd (bask_core*, struct bask_task**, unsigned int);

#endif /* __BASK_TASK_H__ */
