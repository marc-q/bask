#ifndef __BASK_UI_H__
#define __BASK_UI_H__

#define T_COLOR_LINEBAK ""

#define DENULL(a) (((a)) == 0 ? (a+1) : (a))
#define GETDIGITS(a) ((int)log10((double)DENULL(a)))

void view_print_nspaces (int);
void view_print_nspaces_str (char*, int);
void view_misc_print_progress (float, char*);
void view_tbl_print_title (char*, int, int);
void view_tbl_print_field_str (char*, int, int);
void view_tbl_print_field_int (int, int, int);

#endif /* __BASK_UI_H__ */
