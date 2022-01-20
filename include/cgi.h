/*-------------------------------------------------------------------------*/
/*                    NETPROG (Network Programming)                        */
/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */  
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*                                                                         */
/* This program is free software; you can redistribute it and/or modify it */
/* under the terms of the GNU General Public License as published by the   */
/* Free Software Foundation; either version 2 of the License, or (at your  */ 
/* option) any later  version.                                             */
/*                                                                         */
/* This program is distributed in the hope that it will be useful, but     */
/* WITHOUT ANY WARRANTY; without even the implied warranty of              */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU General */
/* Public License for more details.                                        */
/*                                                                         */
/* You should have received a copy of the GNU General Public License along */
/* with this program; if not, write to the Free Software Foundation, Inc., */
/* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA                   */
/* Copy of GNU General Public License at: http://www.gnu.org/              */
/*                                                                         */
/* Source code home page: http://sourceforge.net/projects/netprog/         */
/* Contact author at: Gabriel_Jureidini@yahoo.com                          */
/*-------------------------------------------------------------------------*/


#ifndef __CGI_H__
#define __CGI_H__

#include "ostypes.h"
#include "oslist.h"
#include "osio.h"

#define MULTI_PART_BUF_SIZE  16834

#define CGICOLUMN         0
#define CGIBEGINDETAIL    1
#define CGIENDDETAIL      2
#define CGICONSTANT       3 
#define CGITEXT           4
#define CGINEXNOTAG       5
#define CGITABLE          6



#define CGIMaxColumn  1000
#define CGIMaxLevels  20



#define CGTITYPEDATABASE  1
#define CGTITYPEHTTP      2


#define CGIFIND           0
#define CGIPOS            1  
#define CGIINCR           2
#define CGIDECR           3

#define CGIStrEq(a, b) (!strcmp((a), (b)))



typedef enum
{
    cgiParseSuccess,
    cgiParseMemory,
    cgiParseIO
}cgiParseResultType;

typedef enum
{
    cgiFormSuccess,
    cgiFormTruncated,
    cgiFormBadType,
    cgiFormEmpty,
    cgiFormNotFound,
    cgiFormConstrained,
    cgiFormNoSuchChoice,
    cgiFormMemory
}cgiFormResultType;

typedef enum
{
    cgiEnvironmentIO,
    cgiEnvironmentMemory,
    cgiEnvironmentSuccess
}cgiEnvironmentResultType;


typedef enum
{
    cgiEscapeRest,
    cgiEscapeFirst,
    cgiEscapeSecond
}cgiEscapeState;

typedef enum
{
    cgiUnescapeSuccess,
    cgiUnescapeMemory
}cgiUnescapeResultType;



typedef struct _CGIFormEntry
{
    char* Attribute;
    char* Value;
    struct _CGIFormEntry *next;
}CGIFormEntry;



typedef struct _CGINode
{
    char*   Name;
    BYTE    Type;
    DWORD   Pointer;
    List*   Nodes;
}CGINode;


typedef struct _CGIColumns
{
    char* Name;
    int   Size;
    List* Values;
}CGIColumns;


typedef struct _CGI
{
    CGIColumns ColumnsValues[CGIMaxColumn];
    CGINode*   NodeLevels[CGIMaxLevels];
    CGINode*   RootNode;
    int        ColumnsSequence[CGIMaxLevels][CGIMaxColumn];
    int        ColumnsLastRange[CGIMaxLevels];
    int        ColumnsVisited[CGIMaxLevels];
    int        ColumnsNumber;
    char*      Buffer;
    int        Size;
    char       Request[1500];
    char       FileName[250];
    char       Host[500];
    BYTE       Type;
    int        HexValue[256];
    char*      ServerSoftware;
    char*      ServerName;
    char*      GatewayInterface;
    char*      ServerProtocol;
    char*      ServerPort;
    char*      RequestMethod;
    char*      PathInfo;
    char*      PathTranslated;
    char*      ScriptName;
    char*      QueryString;
    char*      RemoteHost;
    char*      RemoteAddr;
    char*      AuthType;
    char*      RemoteUser;
    char*      RemoteIdent;
    char*      ContentType;
    char*      Accept;
    char*      UserAgent;
    char*      Referrer;
    char*      Cookie;
    char*      ContentLengthString;
    int        ContentLength;
    int        CookieLength;
    CGIFormEntry*  FormEntryFirst;
    CGIFormEntry*  CookieEntryFirst;
    char*          FindTarget;
    CGIFormEntry*  FindPos;
    int        Restored;
    List*       Tags;
    FILE*      In;
    FILE*      Out;
}CGI;




typedef struct _CGIElement
{
    BYTE       Type;
    char       Tag[250];
    int        Occurrence;
}CGIElement;


typedef struct _CGITagColumn
{
    int        Range;
    char       Name[250];
}CGITagColumn;



typedef struct _CGITag
{
    char       Name[250];
    BYTE       Type;
    List*      Elements;
    List*      Columns;
}CGITag;



typedef struct _CGIValue
{
    char*      Value;
    void*      UserField;
    List*      ExecuteValue;
}CGIValue;


typedef struct _CGICallBack
{
    void*  ApplicationField;
    int  (*ExecuteProcedure) (CGI* pcgi, CGIValue* pvalue, void* pappfield);
}CGICallBack;







#ifdef __cplusplus
extern "C" {
#endif

    extern char*         CGIGetEnv(char **s, char *var);
    extern int           CGIInitEnv(CGI* pcgi);
    extern void          CGISetupConstants(CGI* pcgi);
    extern void          CGIFreeResources(CGI* pcgi);
    extern void          CGIHeaderLocation(CGI* pcgi, char *redirectUrl);
    extern void          CGIHeaderStatus(CGI* pcgi, int status, char *statusMessage);
    extern void          CGIHeaderContentType(CGI* pcgi, char *mimeType);
    extern CGIFormEntry* CGIFormEntryFindFirst(CGI* pcgi, char *name);
    extern CGIFormEntry* CGIFormEntryFindNext(CGI* pcgi);
    extern int           CGIFirstNonspaceChar(char *s);
    extern void          CGIStringArrayFree(char **stringArray);
    extern int           CGIWriteString(CGI* pcgi, FILE *out, char *s);
    extern int           CGIWriteInt(CGI* pcgi, FILE *out, int i);
    extern int           CGIReadString(CGI* pcgi, FILE *in, char **s);
    extern int           CGIReadInt(CGI* pcgi, FILE *out, int *i);
    extern CGIValue*     CGIAddValue (CGI* pcgi, char* stringcol, char* Value);
    extern void          CGIFreeValue (CGIValue* pvalue);
    extern int           CGIAddCallBack (CGI* pcgi, CGIValue* pvalue, int (*funct)(CGI* pcgi, CGIValue* pvalue, void* appfield), void* appfield);

    extern int           CGICompute (CGI* pcgi, char* Content, int Size);

    extern int           CGIInit (CGI* pcgi, char* host, char* request);
    extern void          CGIFreeColumns (CGI* pcgi);
    extern void          CGIFreeValues (CGI* pcgi);
    extern int           CGIAddColumn (CGI* pcgi, char* name, int namesize, int valuesize, void* values);
    extern char*         CGIGetValue (CGI* pcgi, char* ColumName, int Range);
    extern int           CGISetValue (CGI* pcgi, char* ColumName, int Range, char* Value);
    extern int           CGIGetColumnIndex (CGI* pcgi, char* name);
    extern void          CGISetColumnsSequence (CGI* pcgi, int Level, int Nbr);
    extern int           CGIColumnsInList (CGI* pcgi, List* list);
    extern void          CGIEnd (CGI* pcgi);
    extern char*         CGIGetValueFormEntry (CGI* pcgi, char* Attribute);

    extern int           CGIParseFile (CGI* pcgi, char* FileName);
    extern void          CGIReturn (CGI* pcgi, FILE* fileout);
    extern void          CGIPrintOut (CGI* pcgi, FILE* fileout, CGINode* node, int* Level, int* Range);

    extern CGINode*      CGIMakeNode (char* name, BYTE type, DWORD pointer);
    extern void          CGIFreeNode (CGINode* node);
    extern BYTE          CGILookKeyWord (char* name);
    extern int           CGISelect (char* database, char* name, char* passwd, char* Request, char* PortofolioRequest, char* NewsRequest,  char* FileName, char* RequestIfEmpty, char* FileIfEmptyName, int MaxElt);
    extern int           CGIExecute (char* database, char* name, char* passwd, char* Request, char* FileName);

    extern BOOL          CGIIsDigit (char* value);
    extern int           CGIContentsProc (void* sqlda, void* par);
    extern int           CGIContentsPortofolios (void* sqlda, void* par);
    extern int           CGIContentsNews (void* sqlda, void* par);

    extern int           CGIHeaderProc (void* sqlda, void* par);
    extern int           CGIAck (void* ack, void* par);
    extern void          OSGetSDateHour(long* dat, long* hour);
    extern cgiParseResultType    CGIParseCookieInput(CGI* pcgi, char *data, int length);
    extern cgiParseResultType    CGIParseFormInput(CGI* pcgi, char *data, int length);
    extern cgiParseResultType    CGIParsePostFormInput(CGI* pcgi);
    extern cgiParseResultType    CGIParsePostMultiPartFormInput(CGI* pcgi);
    extern cgiUnescapeResultType CGIUnescapeChars(CGI* pcgi, char **sp, char *cp, int len);
    extern cgiFormResultType     CGIFormString(CGI* pcgi, char *name, char *result, int max);
    extern cgiFormResultType     CGIFormStringNoNewlines(CGI* pcgi, char *name, char *result, int max);
    extern cgiFormResultType     CGIFormEntryString(CGIFormEntry *e, char *result, int max, int newlines);

    extern cgiFormResultType     CGIFormStringMultiple(CGI* pcgi, char *name, char ***result);
    extern cgiFormResultType     CGIFormStringSpaceNeeded(CGI* pcgi, char *name, int *result) ;
    extern cgiFormResultType     CGIFormInteger(CGI* pcgi, char *name, int *result, int defaultV);
    extern cgiFormResultType     CGIFormIntegerBounded(CGI* pcgi, char *name, int *result, int min, int max, int defaultV) ;
    extern cgiFormResultType     CGIFormDouble(CGI* pcgi, char *name, double *result, double defaultV) ;
    extern cgiFormResultType     CGIFormDoubleBounded(CGI* pcgi, char *name, double *result, double min, double max, double defaultV);
    extern cgiFormResultType     CGIFormSelectSingle(CGI* pcgi, char *name, char **choicesText, int choicesTotal, int *result, int defaultV) ;
    extern cgiFormResultType     CGIFormSelectMultiple(CGI* pcgi, char *name, char **choicesText, int choicesTotal,int *result, int *invalid) ;
    extern cgiFormResultType     CGIFormCheckboxSingle(CGI* pcgi, char *name);
    extern cgiFormResultType     CGIFormCheckboxMultiple(CGI* pcgi, char *name, char **valuesText, int valuesTotal,int *result, int *invalid);
    extern cgiFormResultType     CGIFormRadio(CGI* pcgi, char *name,    char **valuesText, int valuesTotal, int *result, int defaultV);
    extern cgiEnvironmentResultType CGIReadEnvironment(CGI* pcgi, char *filename);
    extern cgiEnvironmentResultType CGIReadEnvironment(CGI* pcgi, char *filename);

    extern CGITag*       CGIAddTag (CGI* pcgi, char* Name, BYTE Type, int nb_param, ...);
    extern CGIElement*   CGIAddElement (CGI* pcgi, CGITag* svalue, BYTE Type, char* Name, int Occurrence);
    extern void          CGIFreeTag (CGITag* ptag);
    extern void          CGIFreeTags (CGI* pcgi);


#ifdef __cplusplus
}
#endif




#endif /* __CGIPARSE_H__*/
