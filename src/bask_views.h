#ifndef __BASK_VIEWS_H__
#define __BASK_VIEWS_H__

#define BVIEW_TASKLIST 0
#define BVIEW_SUMMARY 1

void view_print_legend_tag (char*, char*);
void view_legend (bask_theme*);
void view_print_single (struct bask_task*);
void view_single (bask_core*, struct bask_task**, int);
void view_summary (bask_core*, bask_theme*, struct bask_task**);
void view_tasklist (bask_core*, bask_theme*, struct bask_task**, short);
void view_history (bask_core*, bask_theme*, struct bask_task**, short);

#endif /* __BASK_VIEWS_H__ */
