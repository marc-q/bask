#ifndef __BASK_CONFIG_H__
#define __BASK_CONFIG_H__

int config_set_str_raw (bask_core*, char*, short*, char*, char*, char*);
int config_set_str (bask_core*, char*);
void config_print_set_str_errors (int);
void config_init_file (bask_core*);
int config_save (bask_core*);
void config_load (bask_core*);

#endif /* __BASK_CONFIG_H__ */
