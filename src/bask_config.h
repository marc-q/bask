#ifndef __BASK_CONFIG_H__
#define __BASK_CONFIG_H__

enum
{
	CONFIG_ERR_SS_DESCBREAK = -5,
	CONFIG_ERR_SS_DESCMIN,
	CONFIG_ERR_SS_DESCMAX,
	CONFIG_ERR_SS_PROJMIN,
	CONFIG_ERR_SS_CONFLINE
};

int config_set_str_raw (bask_core*, char*, short*, char*, char*);
int config_set_str (bask_core*, char*);
void config_print_set_str_errors (int);
void config_init (bask_core*);
void config_save (bask_core*);
void config_load (bask_core*);

#endif /* __BASK_CONFIG_H__ */
