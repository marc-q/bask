#ifndef __BASK_IMPORT_H__
#define __BASK_IMPORT_H__

#define ICALSEP ':'

short import_baskbin (bask_core*, struct bask_task**, char*);
void import_baskbin_cmd (bask_core*, struct bask_task**, char*);
void import_csv_parser (bask_core*, struct bask_task**, char*, char*, char*);
short import_csv (bask_core*, struct bask_task**, char*);
void import_csv_cmd (bask_core*, struct bask_task**, char*);
short import_ical_getdatestr (char*, char*);
short import_ical (bask_core*, struct bask_task**, char*);
void import_ical_cmd (bask_core*, struct bask_task**, char*);

#endif /* __BASK_IMPORT_H__ */
