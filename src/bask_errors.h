#ifndef __BASK_ERRORS_H__
#define __BASK_ERRORS_H__

void errors_useinit (void);
void errors_filenotfound (char*);
void errors_filenotopened (char*);
void errors_filenotwritten (char*);
void errors_homedirnotgot (void);
void errors_timenotgot (void);
void errors_lengthtobig (char*);
void errors_notunsigned (char*);
void errors_outofrange_int (char*, int, int);
void errors_outofrange_float (char*, float, float);

#endif /* __BASK_ERRORS_H__ */
