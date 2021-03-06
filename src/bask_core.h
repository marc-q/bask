#ifndef __BASK_CORE_H__
#define __BASK_CORE_H__

#define P_CMD "bask"
#define P_VERSION "0.0.1"
#define P_LICENCE "GNU GPL v.3"
#define BASKSEP '='
#define BASKBINFILE "baskbin.txt"
#define BASKCONFFILE "bask.txt"
#define BASKTHEMEFILE "basktheme.txt"

#define F_BB_S_DATE 20
#define F_ICAL_S_DATE 16

#define T_S_ADDED F_BB_S_DATE
#define T_S_DUE F_BB_S_DATE
#define T_S_FINISHED F_BB_S_DATE
#define T_S_PROJECT 51
#define T_S_DESCRIPTION 201

#define T_O_DESCRIPTIONBREAK 0
#define T_O_AUTODUETODAY 1
#define T_O_COLOR 2

#define BIGGEST(a,b) ((a>b) ? (a) : (b))
#define SMALLEST(a,b) ((a)<(b) ? (a) : (b))
#define ISBOOL(a) ((((a) == (FALSE)) || ((a) == (TRUE))) ? (TRUE) : (FALSE)) 

enum
{
	CORE_ERR_PARSER_NOTBOOL = -2,
	CORE_ERR_PARSER_NOTFOUND
};

struct _bask_core
{
	int tc_amount;
	int baskbin_uid;
	short t_projectmin; /* 0 - 200 */
	short t_descriptionmax; /* 0 - 200 */
	short t_descriptionmin; /* 0 - 200 */
	short t_options;
	short priority_max;
	short priority_min;
	char path_baskpath[200];
	char path_baskconf[200];
	char path_basktheme[200];
	char path_baskbin[200];
};

typedef struct _bask_core bask_core;

short parser_get_str (char*, char*, char*, size_t, char, char*);
short parser_get_int (char*, char*, int*, char, char*);
short parser_get_short (char*, char*, short*, char, char*);
short parser_get_bool (char*, char*, short*, char, char*);
int utils_streq (char*, char*);
short utils_atos (short*, char*);
void utils_mkstr (size_t, char**);
void utils_chstr (char**, char*);
short bask_init_local_file (FILE**, char*);
void bask_get_baskpath (bask_core*, char*, char*);

#endif /* __BASK_CORE_H__ */
