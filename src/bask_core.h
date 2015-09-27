#ifndef __BASK_CORE_H__
#define __BASK_CORE_H__

#define P_CMD "bask"
#define P_VERSION "0.0.1"
#define P_LICENCE "GNU GPL v.3"
#define BASKSEP "=;"
#define BASKBINFILE "baskbin.txt"
#define BASKCONFFILE "bask.txt"
#define BASKTHEMEFILE "basktheme.txt"
#define T_S_ADDED 51
#define T_S_FINISHED 51
#define T_S_PROJECT 51
#define T_S_DESCRIPTION 201

#define BIGGEST(a,b) ((a>b) ? (a) : (b))

struct _bask_core
{
	int tc_amount;
	int baskbin_uid;
	char path_baskpath[200];
	char path_baskconf[200];
	char path_basktheme[200];
	char path_baskbin[200];
};

typedef struct _bask_core bask_core;

int parser_get_str (char*, char*, char*, size_t, char*);
int parser_get_int (char*, char*, int*, char*);
int utils_time_get_str (char*, size_t);
int utils_streq (char*, char*);

#endif /* __BASK_CORE_H__ */
