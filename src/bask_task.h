#ifndef __BASK_TASK_H__
#define __BASK_TASK_H__

#define TASK_FLAG_ACTIVE 0
#define TASK_FLAG_FINISHED 1

enum
{
	TASK_ERR_CHECK_ACTIVE = -8,
	TASK_ERR_CHECK_PRIORITY,
	TASK_ERR_CHECK_ID,
	TASK_ERR_CHECK_DESCRIPTION,
	TASK_ERR_CHECK_PROJECT,
	TASK_ERR_CHECK_FINISHED,
	TASK_ERR_CHECK_DUE,
	TASK_ERR_CHECK_ADDED
};

struct bask_task
{
	unsigned int n; /* Only positive values. */
	unsigned int t_id; /* Only positive values. */
	short t_priority; /* 2 Bytes should be more than enough for this. */
	short t_flags;
	char t_added[T_S_ADDED];
	char t_due[T_S_DUE];
	char t_finished[T_S_FINISHED];
	char* t_project;
	char* t_description;
	struct bask_task* next;
};

short task_check_input_nbrs (bask_core*, int, short, short, short);
short task_check_input (bask_core*, char*, char*, char*, char*, char*, short);

void task_free_ll (struct bask_task**);
short task_insert (struct bask_task**, unsigned int, unsigned int, short, short, int, char*, char*, char*, char*, char*);
short task_remove (struct bask_task**, unsigned int);
short task_modificate (struct bask_task**, unsigned int, short, int, short, char*, char*, char*, char*, char*);

short task_create (bask_core*, struct bask_task**, short, char*, char*);
void task_deactivate (struct bask_task**, unsigned int);
void task_due (struct bask_task**, unsigned int, char*); 
short task_finish (struct bask_task**, unsigned int); 

short task_search (bask_core*, struct bask_task**, struct bask_task**, char*);

void task_create_cmd (bask_core*, struct bask_task**, short, char*, char*);
void task_remove_cmd (bask_core*, struct bask_task**, unsigned int);
void task_modificate_cmd (bask_core*, struct bask_task**, unsigned int, short, int, short, char*, char*, char*, char*, char*);
void task_deactivate_cmd (bask_core*, struct bask_task**, unsigned int);
void task_due_cmd (bask_core*, struct bask_task**, unsigned int, char*);
void task_finish_cmd (bask_core*, struct bask_task**, unsigned int);

#endif /* __BASK_TASK_H__ */
