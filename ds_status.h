#ifndef DS_STATUS_H
#define DS_STATUS_H
/* Code de retour commun (cours, section 0.3). */
typedef enum { DS_OK = 0, DS_EMPTY, DS_NOMEM, DS_SYNTAX, DS_DIVZERO, DS_BADARG } DsStatus;
typedef int Item;
#endif
