#ifndef __BASK_CORE_H__
#define __BASK_CORE_H__

#define P_CMD "bask"
#define P_VERSION "0.0.1"
#define P_LICENCE "GNU GPL v.3"
#define BASKSEP "=;"
#define BASKBINFILE "baskbin.txt"
#define BASKCONFFILE "bask.txt"
#define BASKTHEMEFILE "basktheme.txt"

#define F_BB_S_DATE 20
#define F_ICAL_S_DATE 16

#define T_S_ADDED F_BB_S_DATE
#define T_S_FINISHED F_BB_S_DATE
#define T_S_PROJECT 51
#define T_S_DESCRIPTION 201

#define T_O_DESCRIPTIONBREAK 0

/* TODO: Create a merge function and use this later. */
#define T_FLAG_MERGE 0

#define T_FLTR_ALL -1
#define T_FLTR_STOPPED 0
#define T_FLTR_FINISHED 1
#define T_FLTR_UNFINISHED 2

#define BIGGEST(a,b) ((a>b) ? (a) : (b))

struct _bask_core
{
	int tc_amount;
	int baskbin_uid;
	short t_projectmin; /* 0 - 200 */
	short t_descriptionmax; /* 0 - 200 */
	short t_descriptionmin; /* 0 - 200 */
	short t_options;
	short flags;
	char path_baskpath[200];
	char path_baskconf[200];
	char path_basktheme[200];
	char path_baskbin[200];
};

typedef struct _bask_core bask_core;

int parser_get_str (char*, char*, char*, size_t, char*, char*);
int parser_get_int (char*, char*, int*, char*, char*);
int parser_get_short (char*, char*, short*, char*, char*);
int utils_streq (char*, char*);
int bask_init_local_file (FILE**, char*);

#endif /* __BASK_CORE_H__ */
