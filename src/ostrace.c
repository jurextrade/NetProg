/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "ostrace.h"

/*============================================================================*/
/* Les variables globales du module                                           */
/*============================================================================*/

LONG GlbError    = 0L ;
LONG GlbTCPError = 0L ;


char GlbMessg[SZ_BUFFER+1] = "" ; /* Le message                           */
char GlbParam[SZ_BUFFER+1] = "" ; /* Les parametres                       */
char GlbInter[SZ_BUFFER+1] = "" ; /* Le message + les parametres          */
char GlbLigne[SZ_BUFFER+1] = "" ; /* La ligne complete                    */
char GlbReper[SZ_BUFFER+1] = "" ; /* Le repertoire des fichiers codes     */
char GlbPathC[SZ_BUFFER+1] = "" ; /* Le chemin du fichier de code courant */
char String[SZ_BUFFER+1]   = "" ; /* Chaine intermediaire                 */
char GlbNom[2+5+1]         = "" ; /* DGXXXXX ou OSXXXXX ou IVXXXXX        */
void (*GlbPrintProcedure)(char *) = NULL ;

OSPrintProcedure GlbPrintProcedureStruct = {NULL, NULL};
IDENTFICH GlbTrace = { { LOCDISQU, ""} , "netprog", "trace"} ;

FILE* GlbOpenedTraceFile = NULL;

/*============================================================================*/
/* Les fonctions de recuperation des pointeurs sur les variables globales     */
/*============================================================================*/


char* OSGetMessg(void) { return GlbMessg ; }
char* OSGetParam(void) { return GlbParam ; }
char* OSGetInter(void) { return GlbInter ; }
char* OSGetLigne(void) { return GlbLigne ; }
LONG* OSGetError(void) { return &GlbError; }

void OSSetPrintProcedure (int (*funct)(char* , void*), void* app)
{
    GlbPrintProcedureStruct.PrintProcedure = funct;
    GlbPrintProcedureStruct.ApplicationField = app;
}

void OSSetRepertoireCode(char* chRepertoireCode)
{
    strcpy(GlbReper, chRepertoireCode) ;
}


void OSSetFunctionPrintMessage(void (*PrintProcedure)(char *))
{
    GlbPrintProcedure = PrintProcedure ;
}


char* OSReturnMessage(LONG iIdtErr)
{
    return OSConstructMessage(iIdtErr, NULL) ;
}



/*============================================================================*/
/* OSPrintMessage(char *)                                                     */
/* On affiche le message passe en parametre grace a la fonction standard ou   */
/* a la fonction redefinie par l'utilisateur par la                           */
/* fonction OSSetFunctionPrintMessage().                                      */
/*============================================================================*/


void OSPrintMessage(char* chMessage)
{
    char * chStr ;

    chMessage = strcpy(String, (chMessage == NULL ? GlbLigne : chMessage)) ;
    if ( chMessage[0] == '*' )
        while ( chMessage[0] == '*' || chMessage[0] == ' ' ) chMessage ++ ;
    chStr = strchr(chMessage, '$') ;
    if (chStr != NULL)  chStr[0] = 0 ;
    if (GlbPrintProcedure != NULL)   GlbPrintProcedure(chMessage) ;
    else
    {
#ifdef WINDOWS
        /*    MessageBox(NULL, chMessage, "TCP/ERROR", MB_OK);*/
#else
        printf("%s\n", chMessage) ;
#endif
    }

    if (GlbPrintProcedureStruct.PrintProcedure != NULL)
        GlbPrintProcedureStruct.PrintProcedure (chMessage, GlbPrintProcedureStruct.ApplicationField) ;
    else
    {
#ifdef WINDOWS
        /*    MessageBox(NULL, chMessage, "TCP/ERROR", MB_OK);*/
#else
        printf("%s\n", chMessage) ;
#endif
    }

    return ;
}



void TCPERROR(int error)
{
    char mess[32+1] ;

    switch ( error )
    {
#ifdef WINDOWS
    case WSAEMFILE       : strcpy(mess, "TETCP038") ; break ;
#endif
    case EBADF           : strcpy(mess, "TETCP001") ; break ;
    case EINPROGRESS     : strcpy(mess, "TETCP003") ; break ;
    case EDESTADDRREQ    : strcpy(mess, "TETCP004") ; break ;
    case EMSGSIZE        : strcpy(mess, "TETCP005") ; break ;
    case EPROTOTYPE      : strcpy(mess, "TETCP006") ; break ;
    case ENOPROTOOPT     : strcpy(mess, "TETCP007") ; break ;
    case EPROTONOSUPPORT : strcpy(mess, "TETCP008") ; break ;
    case ESOCKTNOSUPPORT : strcpy(mess, "TETCP009") ; break ;
    case EOPNOTSUPP      : strcpy(mess, "TETCP010") ; break ;
    case EAFNOSUPPORT    : strcpy(mess, "TETCP011") ; break ;
    case EADDRINUSE      : strcpy(mess, "TETCP012") ; break ;
    case EADDRNOTAVAIL   : strcpy(mess, "TETCP013") ; break ;
    case ENETDOWN        :
#ifdef WINDOWS
        return;
#else
        strcpy(mess, "TETCP014") ; break ;
#endif
    case ENETUNREACH     : strcpy(mess, "TETCP015") ; break ;
    case ENETRESET       : strcpy(mess, "TETCP016") ; break ;
    case ECONNABORTED    : strcpy(mess, "TETCP017") ; break ;
    case ECONNRESET      : strcpy(mess, "TETCP018") ; break ;
    case ENOBUFS         : strcpy(mess, "TETCP019") ; break ;
    case EISCONN         : strcpy(mess, "TETCP020") ; break ;
    case ENOTCONN        : strcpy(mess, "TETCP021") ; break ;
    case ESHUTDOWN       : strcpy(mess, "TETCP022") ; GlbTCPError = error; break ;
    case ETIMEDOUT       : strcpy(mess, "TETCP023") ; break ;
    case ECONNREFUSED    : strcpy(mess, "TETCP024") ; GlbTCPError = error; break ;
    case EHOSTUNREACH    : strcpy(mess, "TETCP025") ; GlbTCPError = error; break ;
    case EPFNOSUPPORT    : strcpy(mess, "TETCP026") ; break ;
#ifndef __VOS__
    case ELOOP           : strcpy(mess, "TETCP027") ; break ;
    case ENAMETOOLONG    : strcpy(mess, "TETCP028") ; break ;
#endif
    case EHOSTDOWN       : strcpy(mess, "TETCP029") ; GlbTCPError = error; break ;
    case ETOOMANYREFS    : strcpy(mess, "TETCP030") ; break ;
    case EALREADY        : strcpy(mess, "TETCP031") ; break ;
#ifndef __VOS__
    case ENOTEMPTY       : strcpy(mess, "TETCP032") ; break ;
#endif
#ifdef _VM_
    case EFAULT          : strcpy(mess, "TETCP041") ; break ;
#endif
#ifndef OS2
 #if defined (UNIX) || defined(_VM_) || defined(WINDOWS)
#ifndef HP_UX
    case EPROCLIM        : strcpy(mess, "TETCP033") ; break ;
#endif
    case EUSERS          : strcpy(mess, "TETCP034") ; break ;
    case EDQUOT          : strcpy(mess, "TETCP035") ; break ;
    case EREMOTE         : strcpy(mess, "TETCP037") ; break ;
 #endif
#ifndef __VOS__
    case ESTALE          : strcpy(mess, "TETCP036") ; break ;
#endif
#endif
    case EWOULDBLOCK     :
    case 0               : return ;

    default              : sprintf(mess, "TETCP039") ;
        break;
    }

    OSError(mess, NULL) ;
}


/*============================================================================*/
/* OSError (char *, char* param, ...)                                         */
/* Fonction conservee pour la compatibilite descendante.                      */
/* On lui passe directement le code alphanumerique de l'erreur.               */
/* On trace et on affiche l'erreur.                                           */
/*============================================================================*/


void OSError(char * Name, char * chParam, ...)
{
    FILE * File ;
    char * Inter ;
    char * Code ;
    struct tm * t ;
    time_t stTime ;
    va_list argptr ;
    int Lng ;

    GlbLigne[0] = GlbMessg[0] = GlbInter[0] = GlbParam[0] = 0 ;
    GlbError = 0 ;
    time(&stTime) ;
    t = localtime(&stTime) ;

    sprintf(GlbLigne, "%04d%02d%02d %02d:%02d:%02d ", (t->tm_year + 1900), (t->tm_mon + 1), t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec) ;
    Code = "newts.cod" ;

    if ( GlbReper[0] )
    {
        sprintf(GlbPathC, "%s/%s", GlbReper, Code) ;
        Code = GlbPathC ;
    }
#ifdef _VM_
    File = fopen(Code, "r") ;
#else
    File = fopen(Code, "rt") ;
#endif
    if ( ! File ) return ;
    fgetline(GlbMessg, SZ_BUFFER, File) ;

    while (!feof(File))
    {
        if (!strncmp(GlbMessg, Name, 8))
        {
            Inter = GlbMessg ;
            while ( *Inter != ' ' ) Inter ++ ;
            while ( *Inter == ' ' ) Inter ++ ;
            memmove(GlbMessg, Inter, (strlen(Inter) + 1)) ;
            break ;
        }
        fgetline(GlbMessg, SZ_BUFFER, File) ;
    }

    if ( feof(File) )
        sprintf(GlbMessg, "*** Erreur ou message Inconnue %s", "Name") ;
    fclose(File) ;

    if (chParam != NULL)
    {
        String[0] = 0 ;
        va_start(argptr, chParam) ;
        vsprintf(String, chParam, argptr) ;
        va_end(argptr) ;
        Lng = (int)strlen(String) ;
        if ( String[Lng] != '$' )
        {
            if (String[Lng] != ' ')   strcat(String, " ") ;
            strcat(String, SITE1) ;
        }
        strcpy(GlbParam, String) ;
    }
    else strcpy(GlbParam, SITE1) ;

    sprintf(GlbInter, "%s %s", GlbMessg, GlbParam) ;
    if ( GlbInter[0] != '*' )  strcat(GlbLigne, "    ") ;
    strcat(GlbLigne, GlbInter) ;

    OSTraceInFile(NULL, GlbLigne) ;
    OSPrintMessage(GlbLigne) ;
    return ;
}


/*============================================================================*/
/* OSReturnMessage(LONG) -> char *                                            */
/* OSConstructMessage(LONG, char *, ...) -> char *                            */
/* On appele OSLoadMessage() avec l'identifiant de l'erreur 'iIdtErr'.        */
/* Le parametre 'chParam' est une chaine formatrice a laquelle peut           */
/* etre associe un nombre d'elements variables.                               */
/* Le parametre 'iIdtErr' est recopie dans 'GlbError'.                        */
/* Le parametre 'chParam' et ses parametres est ecrit dans 'GlbParam'.        */
/* On renvoie le message construit.                                           */
/*============================================================================*/


char* OSConstructMessage(LONG iIdtErr, char* chParam, ...)
{
    int Lng ;
    struct tm * t ;
    time_t stTime ;
    va_list argptr ;

    GlbLigne[0] = GlbMessg[0] = GlbInter[0] = GlbParam[0] = 0 ;
    GlbError = 0 ;
    time(&stTime) ;
    t = localtime(&stTime) ;
    sprintf(GlbLigne, "%04d%02d%02d %02d:%02d:%02d ", (t->tm_year + 1900), (t->tm_mon + 1), t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec) ;

    if (OSLoadMessage(iIdtErr) == NULL)
        sprintf(GlbMessg, "*** Error Unknown %ld", iIdtErr) ;

    if ( chParam != NULL )
    {
        String[0] = 0 ;
        va_start(argptr, chParam) ;
        vsprintf(String, chParam, argptr) ;
        va_end(argptr) ;
        Lng = (int)strlen(String) ;
        if (String[Lng] != '$')
        {
            if ( String[Lng] != ' ' ) strcat(String, " ") ;
            strcat(String, SITE1) ;
        }
        strcpy(GlbParam, String) ;
    }
    else  strcpy(GlbParam, SITE1) ;
    sprintf(GlbInter, "%s %s", GlbMessg, GlbParam) ;

    if ( GlbInter[0] != '*' )   strcat(GlbLigne, "    ") ;
    strcat(GlbLigne, GlbInter) ;
    GlbError = iIdtErr ;
    return GlbLigne ;
}

/*============================================================================*/
/* OSLoadMessage(LONG) -> char *                                              */
/* OSLoadMessage() recherche le message associe au code d'erreur passe en     */
/* parametre.                                                                 */
/* Renvoie NULL si non trouve.                                                */
/* On se sert de 'GlbReper' si elle est renseignee.                           */
/*============================================================================*/


char * OSLoadMessage(LONG iIdtErr)
{
    FILE * File ;
    char * Code ;
    char * Message ;

    GlbNom  [0] = 0 ;
    GlbMessg[0] = 0 ;
    Code = "error.cod" ;

    if ( DEBUT_IV <= iIdtErr && iIdtErr <= FIN_IV )
    {
        sprintf(GlbNom, "IV%05ld", iIdtErr) ;
    }
    else
        if ( DEBUT_GM <= iIdtErr && iIdtErr <= FIN_GM )
        {
            sprintf(GlbNom, "GM%05ld", iIdtErr) ;
        }
        else
            if ( DEBUT_DIALOG <= iIdtErr && iIdtErr <= FIN_DIALOG )
            {
                sprintf(GlbNom, "MX%05ld", iIdtErr) ;
            }
            else
                if ( DEBUT_DB <= iIdtErr && iIdtErr <= FIN_DB )
                {
                    sprintf(GlbNom, "DB%05ld", iIdtErr) ;
                }
                else
                    if ( DEBUT_OS <= iIdtErr )
                    {
                        sprintf(GlbNom, "OS%05ld", iIdtErr) ;
                    }
                    else return NULL ;
    if ( GlbReper[0] )
    {
        Code = GlbPathC ;
    }

#ifdef _VM_
    File = fopen(Code, "r") ;
#else
    File = fopen(Code, "rt") ;
#endif
    if (!File)  return NULL ;
    fgetline(GlbMessg, SZ_BUFFER, File) ;
    while (!feof(File))
    {
        if (!strncmp(GlbMessg, GlbNom, 7))
        {
            Message = GlbMessg ;
            while ( *Message != ' ' ) Message ++ ;
            while ( *Message == ' ' ) Message ++ ;
            memmove(GlbMessg, Message, (strlen(Message) + 1)) ;
            fclose(File) ;
            return GlbMessg ;
        }
        fgetline(GlbMessg, SZ_BUFFER, File) ;
    }
    fclose(File) ;
    return NULL ;
}



/*============================================================================*/
/* OSGlbTraceInFile() -> LONG                                                 */
/* OSTraceInFile(IDENTFICH *, char *) -> LONG                                 */
/* On trace le message passe en parametre dans le fichier repere par          */
/* l'identificateur passe en parametre.                                       */
/*============================================================================*/

void OSSetTraceFile (char* fromname)
{
    char Repertory[500];
    char FileName[250];
    char Extension[100];

    GlbTrace.zloc.typloc = LOCDISQU;

    OSGetOnlyName (fromname, FileName, Repertory, LOCDISQU);
    OSGetFileNameExtension (Extension, FileName);

    strtok (FileName, ".");
    strcpy (GlbTrace.ficfname, FileName);
    strcpy (GlbTrace.zloc.localis, Repertory);
    strcpy (GlbTrace.ficftype, Extension);
    return;
}

void OSSetTraceOpenedFile (FILE* file)
{
    GlbOpenedTraceFile = file;
    return;
}



LONG OSGlbTraceInFile()
{
    return OSTraceInFile(&GlbTrace, GlbLigne);
}




LONG OSTraceInFile(IDENTFICH * ptIdent, char * chMessage)
{
    FILEID * fileid ;
    char filename[120] ;
    int Error ;
    char loc ;
    char * chStr ;

    filename[0] = 0 ;
    Error = 0 ;
    if ( ptIdent == NULL )  ptIdent = &GlbTrace ;

    OSGetFileName(ptIdent, filename) ;
    loc = ptIdent->zloc.typloc ;

    fileid = OSOpenFile(filename, "a", 't', loc, &Error) ;
    if ( fileid )
    {
        int Lng ;
        strcpy(String, (chMessage == NULL ? GlbLigne : chMessage)) ;
        for ( chStr = String ; chStr[0] != '\0' ; ++ chStr ) if ( chStr[0] == '\n' ) chStr[0] = ' ' ;
        Lng = (int)strlen(String) ;
        String[Lng] = '\n' ;

        OSWriteFile(fileid, String, (Lng+1), 0, &Error) ;

        OSCloseFile(fileid) ;
    }
    return (LONG)Error ;
}

/*============================================================================*/
/* OSReturnFicTrace(IDENTFICH *, char, char *, char *, char *) -> IDENTFICH * */
/* Renseigne la structure d'identification du fichier.                        */
/* On renseigne toujours l'identificateur local.                              */
/*============================================================================*/



IDENTFICH* OSReturnFicTrace(IDENTFICH* ptIdent, char typloc, char* localis, char* ficfname, char * ficftype)
{
    GlbTrace.zloc.typloc = typloc ;
    strcpy(GlbTrace.zloc.localis, localis) ;
    strcpy(GlbTrace.ficfname, ficfname) ;
    strcpy(GlbTrace.ficftype, ficftype) ;

    if ( ptIdent == NULL )  return &GlbTrace ;

    ptIdent->zloc.typloc = GlbTrace.zloc.typloc ;

    strcpy(ptIdent->zloc.localis, GlbTrace.zloc.localis) ;
    strcpy(ptIdent->ficfname, GlbTrace.ficfname) ;
    strcpy(ptIdent->ficftype, GlbTrace.ficftype) ;

    return ptIdent ;
}


/*============================================================================*/
/* fgetline(LPSTR, FILE *) -> LPSTR                                           */
/* Fonction de lecture d'une ligne dans un fichier ouvert, gràce à la         */
/* structure ptFic. Le caractère retour chariot est eliminé si il est         */
/* présent.                                                                   */
/*============================================================================*/

char* fgetline(char * chChaine, int iNbr, FILE * ptFic)
{
    int iInd ;

    if (!ptFic) ptFic = stdin;

    if (fgets(chChaine, iNbr, ptFic) == NULL) return NULL;

    iInd = (int)strlen(chChaine) ;

    if (chChaine[iInd-1] == '\n')  chChaine[iInd-1] = 0 ;
    return chChaine ;
}




LONG OSTraceMessage (char* message)
{
    struct tm * t ;
    time_t stTime ;
    char SError[1000];

    time(&stTime) ;
    t = localtime(&stTime) ;

    sprintf(SError, "%04d%02d%02d %02d:%02d:%02d     ",
            (t->tm_year + 1900), (t->tm_mon + 1), t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec) ;

    strcat(SError, message);

    return OSTraceInFile(NULL, SError) ;

}


LONG OSTraceOpenedMessage (char* message)
{
    struct tm * t ;
    time_t stTime ;
    char chMessage[1000];
    LONG Error = 0;
    int Lng ;
    char * chStr ;




    time(&stTime) ;
    t = localtime(&stTime) ;

    sprintf(chMessage, "%04d%02d%02d %02d:%02d:%02d     ",
            (t->tm_year + 1900), (t->tm_mon + 1), t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec) ;

    strcat(chMessage, message);

    strcpy(String, (chMessage == NULL ? GlbLigne : chMessage)) ;

    for ( chStr = String ; chStr[0] != '\0' ; ++ chStr )
        if ( chStr[0] == '\n' ) chStr[0] = ' ' ;

    Lng = (int)strlen(String) ;
    String[Lng] = '\n' ;

    if (GlbOpenedTraceFile)
        fwrite (String, 1, (Lng+1), GlbOpenedTraceFile);

    return Error;
}


