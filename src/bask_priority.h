#ifndef __BASK_PRIORITY_H__
#define __BASK_PRIORITY_H__

#define PRI_S_PCOLOR UI_S_THEMECOLOR
#define PRI_S_PALIAS 4
#define PRI_S_PNAME 20

struct _bask_priority
{
	short p_id;
	char p_color[PRI_S_PCOLOR];
	char p_alias[PRI_S_PALIAS];
	char p_name[PRI_S_PNAME];
	struct _bask_priority* next;
};

typedef struct _bask_priority bask_priority;

void priority_free_ll (bask_priority**);
short priority_insert (bask_priority**, short, char*, char*, char*);
short priority_get_idfromstr (bask_priority**, char*);
short priority_get_viewdata (bask_priority**, short, char*, size_t, char*, size_t);
void priority_create (bask_core*, bask_priority**, short, char*, char*, char*);

#endif /* __BASK_PRIORITY_H__ */
