#ifndef __BASK_CORE_H__
#define __BASK_CORE_H__

#define P_CMD "bask"
#define P_VERSION "0.0.1"
#define BASKSEP "=;"
#define BASKBINFILE "baskbin.txt"
#define BASKCONFFILE "bask.txt"
#define BASKTHEMEFILE "basktheme.txt"

struct _bask_core
{
	int tc_amount;
	int baskbin_uid;
	char path_baskconf[200];
	char path_basktheme[200];
	char path_baskbin[200];
};

typedef struct _bask_core bask_core;

int parser_get_str (char*, char*, char*, size_t, char*);
int parser_get_int (char*, char*, int*, char*);

#endif /* __BASK_CORE_H__ */
