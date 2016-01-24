#ifndef __BASK_VIEWS_H__
#define __BASK_VIEWS_H__

#define BVIEW_TASKLIST 0
#define BVIEW_SUMMARY 1
#define BVIEW_DETAILED 2

void view_print_legend_tag (bask_core*, char*, char*);
void view_legend (bask_core*, bask_theme*, bask_priority**);
void view_print_single (bask_core*, struct bask_task*);
void view_single (bask_core*, struct bask_task**, bask_filter*, unsigned int, short);
void view_summary (bask_core*, bask_theme*, struct bask_task**, bask_filter*);
void view_tasklist (bask_core*, bask_theme*, bask_priority**, struct bask_task**, bask_filter*);
void view_history (bask_core*, bask_theme*, struct bask_task**, bask_filter*);

#endif /* __BASK_VIEWS_H__ */
