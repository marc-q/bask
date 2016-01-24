#ifndef __BASK_CONFIG_H__
#define __BASK_CONFIG_H__

enum
{
	CONFIG_ERR_SS_COLOR = -7,
	CONFIG_ERR_SS_AUTODUETODAY,
	CONFIG_ERR_SS_DESCBREAK,
	CONFIG_ERR_SS_DESCMIN,
	CONFIG_ERR_SS_DESCMAX,
	CONFIG_ERR_SS_PROJMIN,
	CONFIG_ERR_SS_CONFLINE
};

short config_set_str_raw (bask_core*, char*, short*, char*, size_t, char*);
short config_set_str (bask_core*, char*);
void config_set_str_cmd (bask_core*, char*);
void config_print_set_str_errors (short);
void config_init (bask_core*);
void config_save (bask_core*);
void config_load (bask_core*);

#endif /* __BASK_CONFIG_H__ */
