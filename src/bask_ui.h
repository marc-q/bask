#ifndef __BASK_UI_H__
#define __BASK_UI_H__

#define BC_BLACK "0"
#define BC_RED "1"
#define BC_GREEN "2"
#define BC_YELLOW "3"
#define BC_BLUE "4"
#define BC_PURPLE "5"
#define BC_CYAN "6"
#define BC_WHITE "7"

#define DENULL(a) (((a)) == 0 ? (a+1) : (a))
#define GETDIGITS(a) ((int)log10((double)DENULL(a)))

struct _bask_theme
{
	char color_normal[11];
	char color_important[11];
	char color_today[11];
	char color_critical[11];
	char color_finished[11];
	char color_pbarbak[11];
	char color_seclinesbak[11];
};

typedef struct _bask_theme bask_theme;

void view_print_nspaces (int);
void view_print_nspaces_str (char*, int);
void view_theme_get_color (char*);
void view_theme_load (bask_core*, bask_theme*);
void view_misc_print_progress (float, char*);
void view_tbl_print_title (char*, int, int);
void view_tbl_print_field_str (char*, int, int);
void view_tbl_print_field_int (int, int, int);

#endif /* __BASK_UI_H__ */
