/*-------------------------------------------------------------------------*/
/*                    NETPROG (Network Programming)                        */
/*                    ______________________________                       */
/* FICHIER     : OSDBSTA.H                                                 */
/* DESCRIPTION :                                                           */
/* PROPRIETES  : EAGLENET                                                  */
/*-------------------------------------------------------------------------*/

#ifndef __OSDBSTA_H__
#define __OSDBSTA_H__

#include "osdb.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifdef SQLDS
    extern void sqlds_define_sqlda (OSSqlda* psqlda, int* size,
                                        int* SelectItemNumber,
                                        char* SelectItemName, char* type,
                                        int SelectItemLength);
    extern int sqlds_write_selectitem (OSSqlda* psqlda, char** values,
                                           char* SelectItemName, void* val);
#endif
#ifdef ORACLE
    extern void oracle_define_sqlda (OSSqlda* psqlda, int* size,
                                         int* SelectItemNumber,
                                         char* SelectItemName, char* type,
                                         int SelectItemLength);
    extern int oracle_write_selectitem (OSSqlda* psqlda, char** values,
                                            char* SelectItemName, void* val);
#endif

#ifdef __cplusplus
}
#endif

#endif
