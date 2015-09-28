#ifndef __BASK_EXPORT_H__
#define __BASK_EXPORT_H__

#define EXPORT_CSV_SPLIT ";"

int export_baskbin (bask_core*, struct bask_task**, char*);
int export_csv (bask_core*, struct bask_task**, char*);
int export_ical (bask_core*, struct bask_task**, char*);
int export_web (bask_core*, struct bask_task**, char*);

#endif /* __BASK_EXPORT_H__ */
