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

#define BC_LOW_TXT "3"
#define BC_LOW_BAK "4"

#define BC_HIGH_TXT "9"
#define BC_HIGH_BAK "0;10"

#define UI_S_THEMECOLOR 13

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

void ui_print_nchars (char, int);
void ui_print_nchars_str (char, char*, int);
void ui_theme_get_color (char*);
void ui_theme_load (bask_core*, bask_theme*);
void ui_misc_print_color (bask_core*, char*);
void ui_misc_print_progress (bask_core*, float, char*);
void ui_misc_print_linebreak (bask_core*, char*, short, short);
void ui_tbl_print_title (bask_core*, char*, int, int);
void ui_tbl_print_title_underline (int, int);
void ui_tbl_print_field_str (char*, int, int);
void ui_tbl_print_field_int (int, int, int);

#endif /* __BASK_UI_H__ */
