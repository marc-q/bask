#ifndef __BASK_EXPORT_H__
#define __BASK_EXPORT_H__

#define EXPORT_CSV_SPLIT ";"

short export_baskbin (bask_core*, struct bask_task**, char*);
short export_csv (bask_core*, struct bask_task**, char*);
short export_ical_getdatestr (char*, char*);
short export_ical (bask_core*, struct bask_task**, char*);
short export_web (bask_core*, struct bask_task**, char*);

#endif /* __BASK_EXPORT_H__ */
