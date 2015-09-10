#ifndef __BASK_VIEWS_H__
#define __BASK_VIEWS_H__

void view_print_legend_tag (char*, char*, char*);
void view_legend (bask_theme*);
void view_print_single (struct bask_task*);
void view_single (bask_core*, struct bask_task**, int);
void view_summary (bask_core*, bask_theme*, struct bask_task**);
void view_tasklist (bask_core*, bask_theme*, struct bask_task**);

#endif /* __BASK_VIEWS_H__ */
