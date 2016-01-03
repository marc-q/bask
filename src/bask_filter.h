#ifndef __BASK_FILTER_H__
#define __BASK_FILTER_H__

#define FLTR_OFF 0
#define FLTR_ON 1

#define T_FLTR_ACTIVE 0
#define T_FLTR_FINISHED 1
#define T_FLTR_PRIORITY 2
#define T_FLTR_MONTH 3

struct _bask_filter
{
	short flags;
	short tmonth;
	struct bask_task task;
};

typedef struct _bask_filter bask_filter;

void filter_init (bask_filter*, short, short, short, short);
int filter_check_task (bask_filter*, struct bask_task*);

#endif /* __BASK_FILTER_H__ */
