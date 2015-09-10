#ifndef __BASK_CORE_H__
#define __BASK_CORE_H__

struct _bask_core
{
	int tc_amount;
	int baskbin_uid;
	char baskpath[200];
	FILE* baskfile;
};

typedef struct _bask_core bask_core;

#endif /* __BASK_CORE_H__ */
