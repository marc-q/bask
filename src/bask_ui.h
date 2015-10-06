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

#define UI_S_THEMECOLOR 11

#define DENULL(a) (((a)) == 0 ? (a+1) : (a))
#define GETDIGITS(a) ((int)log10((double)DENULL(a)))

struct _bask_theme
{
	char color_normal[UI_S_THEMECOLOR];
	char color_important[UI_S_THEMECOLOR];
	char color_today[UI_S_THEMECOLOR];
	char color_critical[UI_S_THEMECOLOR];
	char color_finished[UI_S_THEMECOLOR];
	char color_pbarbak[UI_S_THEMECOLOR];
	char color_seclinesbak[UI_S_THEMECOLOR];
};

typedef struct _bask_theme bask_theme;

void ui_print_nspaces (int);
void ui_print_nspaces_str (char*, int);
void ui_theme_get_color (char*);
void ui_theme_load (bask_core*, bask_theme*);
void ui_misc_print_progress (float, char*);
void ui_tbl_print_title (char*, int, int);
void ui_tbl_print_field_str (char*, int, int);
void ui_tbl_print_field_int (int, int, int);

#endif /* __BASK_UI_H__ */
