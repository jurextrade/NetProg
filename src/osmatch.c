/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "osmatch.h"


static int matche_after_star (register char *p,register char *t);


static int matche(register char *p, register char *t)
{

    register char range_start, range_end;  /* start and end in range */

    BOOL invert;             /* is this [..] or [!..] */
    BOOL member_match;       /* have I matched the [..] construct? */
    BOOL loop;               /* should I terminate? */

    for ( ; *p;  p++, t++)
    {

        if (!*t)
            return ((*p == '*') && (*++p == '\0'))?  MATCH_VALID : MATCH_ABORT;

        switch (*p)
        {

        case '?':
            break;

        case '*':
            return matche_after_star (p, t);

        case '[':
            {
                p++;
                invert = FALSE;
                if ((*p == '!') || (*p == '^')) {
                    invert = TRUE;
                    p++;
                }
                if (*p == ']')
                    return MATCH_PATTERN;

                member_match = FALSE;
                loop = TRUE;

                while (loop)
                {

                    if (*p == ']')
                    {
                        loop = FALSE;
                        continue;
                    }

                    if (*p == '\\')
                        range_start = range_end = *++p;
                    else
                        range_start = range_end = *p;

                    if (!*p)
                        return MATCH_PATTERN;

                    if (*++p == '-')
                    {

                        range_end = *++p;

                        if ((range_end == '\0') || (range_end == ']'))
                            return MATCH_PATTERN;

                        if (range_end == '\\') {
                            range_end = *++p;

                            if (!range_end)
                                return MATCH_PATTERN;
                        }

                        p++;
                    }
                    if (range_start < range_end)
                    {
                        if ((*t >= range_start) && (*t <= range_end))
                        {
                            member_match = TRUE;
                            loop = FALSE;
                        }
                    }
                    else
                    {
                        if ((*t >= range_end) && (*t <= range_start))
                        {
                            member_match = TRUE;
                            loop = FALSE;
                        }
                    }
                }
                if ((invert && member_match) ||
                        !(invert || member_match))
                    return MATCH_RANGE;
                if (member_match)
                {
                    while (*p != ']')
                    {

                        if (!*p)
                            return MATCH_PATTERN;

                        if (*p == '\\') {
                            p++;

                            if (!*p)
                                return MATCH_PATTERN;
                        }
                        p++;
                    }
                }

                break;
            }  /* switch '[' */
        default:

            if (*p != *t)
                return MATCH_LITERAL;

        }  /* switch */
    }  /* for */

    /* if end of text not reached then the pattern fails */
    if (*t != '\0') return MATCH_END;
    else return MATCH_VALID;
}


/*----------------------------------------------------------------------------
*
* recursively call matche() with final segment of PATTERN and of TEXT.
*
----------------------------------------------------------------------------*/

static int matche_after_star (register char *p,register char *t)
{

    register int match = 0;
    register int nextp;

    while ((*p == '?') || (*p == '*'))
    {

        if ((*p == '?') && (!*t++))
            return MATCH_ABORT;

        /* move to next char in pattern */
        p++;
    }

    if (!*p)
        return MATCH_VALID;

    nextp = *p;

    do {
        if ((nextp == *t) || (nextp == '['))
            match = matche(p, t);

        if (!*t++)
            match = MATCH_ABORT;

    } while ((match != MATCH_VALID) &&
             (match != MATCH_ABORT) &&
             (match != MATCH_PATTERN));

    return match;
}


/*----------------------------------------------------------------------------
*
* match() is a shell to matche() to return only BOOL values.
*
----------------------------------------------------------------------------*/

static BOOL BigMatch(char *string,char *pattern)
{

    int error_type;

    error_type = matche(pattern,string);
    if (error_type == MATCH_VALID ) return TRUE;
    else return FALSE;
}



BOOL is_pattern (char *p)
{
    while (*p)
        switch (*p++)
        {
        case '?':
        case '*':
        case '[':
            return TRUE;
        }
    return FALSE;
}


BOOL is_valid_pattern (char *p, int *error_type)
{

    /* init error_type */
    *error_type = PATTERN_VALID;

    /* loop through pattern to EOS */
    while (*p)
    {

        /* determine pattern type */
        switch (*p)
        {

            /* the [..] construct must be well formed */
        case '[':
            p++;

            /* if the next character is ']' then bad pattern */
            if (*p == ']')
            {
                *error_type = PATTERN_EMPTY;
                return FALSE;
            }

            /* if end of pattern here then bad pattern */
            if (!*p)
            {
                *error_type = PATTERN_CLOSE;
                return FALSE;
            }

            /* loop to end of [..] construct */
            while (*p != ']')
            {

                /* check for literal escape */
                if (*p == '\\') {
                    p++;

                    /* if end of pattern here then bad pattern */
                    if (!*p++) {
                        *error_type = PATTERN_ESC;
                        return FALSE;
                    }
                } else
                    p++;

                /* if end of pattern here then bad pattern */
                if (!*p) {
                    *error_type = PATTERN_CLOSE;
                    return FALSE;
                }

                /* if this a range */
                if (*p == '-') {

                    /* we must have an end of range */
                    if (!*++p || (*p == ']')) {
                        *error_type = PATTERN_RANGE;
                        return FALSE;
                    } else {

                        /* check for literal escape */
                        if (*p == '\\')
                            p++;

                        /* if end of pattern here then bad pattern */
                        if (!*p++) {
                            *error_type = PATTERN_ESC;
                            return FALSE;
                        }
                    }
                }
            }
            break;

            /* all other characters are valid pattern elements */
        case '*':
        case '?':
        default:
            p++;                /* "normal" character */
            break;
        }    /* switch */
    }

    return TRUE;
}

/* ========================================================================*/
/* Routine de type match tres simplifiie (2 jockers seulement : ? et *     */
/* ========================================================================*/

static BOOL fmatch2(register char *fname, register char *pat) ;
static BOOL fmatch(register char *fname, register char *pat)
{
    if(*pat == '*')
    {

        pat++;

        if(*pat == '\0') return TRUE;

        while(*fname != '\0') {
            if(fmatch2(fname, pat) == TRUE) return TRUE;
            fname++;
        }

        return FALSE;
    }
    else return fmatch2(fname, pat);
}

static BOOL fmatch2(register char *fname, register char *pat)
{
    while(*pat != '\0' && *fname != '\0')
    {

        if (*pat == '*') return fmatch(fname, pat);
        if (*pat != '?') if(*pat != *fname) return FALSE;

        pat++;
        fname++;
    }
    if ( *fname != '\0') return FALSE;
    if (*pat == '\0' || (*pat == '*' && *(pat+1) == '\0')) return TRUE;
    return FALSE;
}

BOOL OSSimpleMatch (char* String, char* Pattern)
{
    return fmatch(String,Pattern);
}

BOOL OSMatch(char* String, char* Pattern)
{
    return BigMatch(String,Pattern);
}


int OSGetFilesFromWildCards (char* AllFileName, List** list, char FileLoc, BOOL WithReper)
{
    char FileName[250];
    char Repertory[250];
    char TempLRepertory[250];
    FILEDATA* filetoinsert;
    int Error = 0;
    int nbr = 0;

#ifdef _VM_
    char FileType[100];
    char DestFileName[250];
    char SFSRepertory[250];
    char Token[8];
    char Lct[88];
    char cmsname[30];
    char cmstype[30];
    char cmsloc[30];


    OSGetOnlyName (AllFileName, FileName, Repertory, FileLoc);
    if (strcmp (FileName, "") == 0) return 0;

    OSSetFileName (DestFileName, Repertory, FileName, FileLoc);
    if ((FileLoc == 'R') || (FileLoc == 'r'))
    {
        OSGetSFSFormatFromReper (Repertory, SFSRepertory);
    }
    else
    {
        OSGetCMSFormatFromFileName (AllFileName, cmsname, cmstype, cmsloc);
        sprintf (DestFileName, "%s %s %s", cmsname, cmstype, cmsloc);
    }
    printf ("avant open dir Destfilename = %s\n", DestFileName);

    if ((OpenDirSFS(DestFileName, Token, "FILE", &Error)) < 0)
        return 0;

    while(!GetDirSFS(Token, Lct, 88, 1, &Error))
    {
        strncpy(FileName, Lct + 4, 8);
        FileName[8] = 0;
        strncpy(FileType, Lct + 12, 8);
        FileType[8] = 0;

        if (WithReper)
        {
            strtok(FileName, " ");
            strtok(FileType, " ");
            sprintf (TempLRepertory, "%s%s.%s", Repertory, FileName, FileType);
        }
        else
        {
            strtok(FileName, " ");
            strtok(FileType, " ");
            sprintf(TempLRepertory, "%s.%s", FileName, FileType);
        }

        printf("%s\n", TempLRepertory);
        filetoinsert = (FILEDATA *)malloc (sizeof (FILEDATA));
        strcpy (filetoinsert->Name, TempLRepertory);
        nbr++;
        ListNewr(list , filetoinsert) ;
    }

    CloseDirSFS(Token, &Error);
    return nbr;
#else /*VM*/

#ifdef WINDOWS
 #ifdef _WIN32
    WIN32_FIND_DATA ffblk;
    BOOL done = FALSE;
    HANDLE HandleFile;
 #else
    struct ffblk ffblk;
    int done;
 #endif
#endif
#if defined(UNIX)
    struct dirent *dp;
    DIR* DirectoryPointer;
#endif

#ifdef __VOS__
    char_varying(256)  path_name;
    char_varying (32)  caller = "";
    short int error_code;
    short int maximum_number = 200;
    struct $shortmap expand_star_info_tag
    {
        short int version;
        struct
        {
            char_varying (32) object_name;
            short int object_type;
        }entries[200];
    }expand_star_info;
    short int number_object_names;
    short int switches = 11; /* 1011 */
    int i = 0;
#endif  


    OSGetOnlyName (AllFileName, FileName, Repertory, FileLoc);
    if (strcmp (FileName, "") == 0) return 0;

#ifdef WINDOWS
 #ifdef _WIN32
    HandleFile = FindFirstFile (AllFileName, &ffblk);
    if (HandleFile == INVALID_HANDLE_VALUE) done = TRUE;
    while (!done)
    {
        if (WithReper)
        {
            strcpy (TempLRepertory, Repertory);
            strcat (TempLRepertory, ffblk.cFileName);
        }
        else strcpy (TempLRepertory, ffblk.cFileName);

        filetoinsert = (FILEDATA *)malloc (sizeof (FILEDATA));
        if (ffblk.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            filetoinsert->Type = TYPEDIRECTORY;
        else
            filetoinsert->Type = TYPEFILE;
        filetoinsert->Size = (ffblk.nFileSizeHigh * MAXDWORD) + ffblk.nFileSizeLow;
        strcpy (filetoinsert->Name, TempLRepertory);
        nbr++;
        //      ListNewr(list , strlwr (filetoinsert)) ;
        ListNewr(list , filetoinsert) ;
        done = (!FindNextFile(HandleFile, &ffblk));
    }
    FindClose (HandleFile);
 #else
    done = findfirst (AllFileName, &ffblk , 0);
    while (!done)
    {
        if (WithReper)
        {
            strcpy (TempLRepertory, Repertory);
            strcat (TempLRepertory, ffblk.ff_name);
        }
        else strcpy (TempLRepertory, ffblk.ff_name);

        filetoinsert = (FILEDATA *)malloc (sizeof (FILEDATA));
        strcpy (filetoinsert->Name, TempLRepertory);
        nbr++;
        //      ListNewr(list , strlwr (filetoinsert)) ;
        ListNewr(list , filetoinsert) ;
        done = findnext(&ffblk);
    }
 #endif

#else /*WINDOWS*/
#ifdef UNIX
    if (strcmp (Repertory, "") == 0)
    {
        OSGetDir (Repertory, FileLoc, &Error);
        strcat (Repertory, "/");
    }
    DirectoryPointer = opendir (Repertory);
    if (DirectoryPointer)
    {
        for (dp = readdir (DirectoryPointer); dp!= NULL; dp = readdir (DirectoryPointer))
        {
            if (OSMatch (dp->d_name, FileName))

            {
                if (WithReper)
                {
                    strcpy (TempLRepertory, Repertory);
                    strcat (TempLRepertory, dp->d_name);
                }
                else
                    strcpy (TempLRepertory, dp->d_name);

                filetoinsert = (FILEDATA *)malloc (sizeof (FILEDATA));
                strcpy (filetoinsert->Name, TempLRepertory);
                nbr++;
                ListNewr(list, filetoinsert) ;
            }
        }
        closedir (DirectoryPointer);
    }
#endif /*UNIX*/
#ifdef __VOS__
    if (strcmp (Repertory, "") == 0)
    {
        OSGetDir (Repertory, FileLoc, &Error);
        strcat (Repertory, ">");
        sprintf (FileName, "%s%s", Repertory, AllFileName);
    }
    else strcpy (FileName, AllFileName);

    strcpy_vstr_nstr(&path_name, FileName);
    expand_star_info.version = 1;
    s$expand_star (&path_name, &switches, &maximum_number, &number_object_names, &expand_star_info, &error_code);
    if (error_code != 0)
    {
        char_varying (32) caller = "";
        char_varying (256) message_text = "";
        s$error (&error_code, &caller, &message_text);
        return nbr;
    }
    for (i = 0; i < number_object_names; i++)
    {
        if (WithReper)
        {
            strcpy (TempLRepertory, Repertory);
            strcat_nstr_vstr(TempLRepertory, &expand_star_info.entries[i].object_name);
        }
        else
            strcpy_nstr_vstr(TempLRepertory, &expand_star_info.entries[i].object_name);

        filetoinsert = (FILEDATA *)malloc (sizeof (FILEDATA));
        strcpy (filetoinsert->Name, TempLRepertory);
        if (expand_star_info.entries[i].object_type == 1)
            filetoinsert->Type = TYPEFILE;
        else
            if (expand_star_info.entries[i].object_type == 2)
                filetoinsert->Type = TYPEDIRECTORY;
            else
                if (expand_star_info.entries[i].object_type == 3)
                    filetoinsert->Type = TYPELINK;
        nbr++;
        ListNewr(list, filetoinsert) ;
    }
#endif
#endif /*WINDOWS*/
#endif /*_VM_*/


    return nbr;
}




#define SPACE                  ' '
#define SLASH                  '/'
#define BACK_SLASH             '\\'

#define SENTENCE_BEGINNING     1
#define SENTENCE_ENDING        2
#define GLOBAL                 3


int fggets(FILE* InFile, char *s, int StringLength, int* CRFlag, int* DoneFlag)
{
    char cc;
    int count_down = StringLength;
    char* ss = s;

    ss[0] = 0x0;
    while (1)
    {
        cc = fgetc( InFile );
        switch (cc)
        {
        case 0x0D:
            ss[0] = 0x0;
            *CRFlag = 1;
            return 0;
            break;
        case 0x0A:
            if (!(*CRFlag))
            {
                ss[0] = 0x0;
                return 0;
            }
            break;
        case EOF:
            ss[0] = 0x0;
            *DoneFlag = 1;
            return 0;
            break;
        default:
            if (count_down)
            {
                ss[0] = cc;
                count_down--;
                ss++;
            }
        }
    }
}


int OSReplaceInFile (char* FileName, char* string_orig, char* string_new, char mode, char loc, int* Error)
{
    FILEID*  inf;
    FILEID*  outf;
    char*  p0;
    char*  q;
    char*  r;
    char*  command;
    char*  line;
    char command_buf[108];
    int  done = 0;
    int  crflag = 0;
    long line_count = 0;
    int encounter = 0;
    char  line_buf[4096];
    int   replace_pos;

    line = (char *) (&line_buf);
    line[0] = 0x0;
    replace_pos = GLOBAL;

    if ((mode == 'H') || (mode == 'B'))
        replace_pos = SENTENCE_BEGINNING;
    else
        if ((mode == 'T') || (mode ==  'E'))
            replace_pos = SENTENCE_ENDING;
        else
            if (mode == 'G')
                replace_pos = GLOBAL;
            else return -1;

    /* clean up input line */
    line[0] = 0x0;
    command = (char *) (&command_buf);

    /* check input file existance */
    if ((inf = OSOpenFile(FileName, "r", 't',loc, Error)) == NULL)
    {
        return -1;
    }
    OSCloseFile(inf);

    OSCopyFile (FileName,  "__intermediate__file__", loc, Error);
    if (*Error) return -1;

    if (((inf = OSOpenFile("__intermediate__file__", "r", 't', loc, Error)) == NULL) ||
            ((outf = OSOpenFile(FileName, "w", 't', loc, Error)) == NULL))
    {
        return -1;
    }

    /* main loop */
    while (!done)
    {
        fggets(inf->File, line, 4096, &crflag, &done);

        if ((feof((FILE*)inf->File)) && (strlen(line) == 0))
            break;
        else
            line_count++;

        /* sentense beginning substitution */
        if (replace_pos == SENTENCE_BEGINNING)
        {
            q = (char *) strstr(line, string_orig);
            if (q == line)
            {
                fprintf(outf->File, "%s", string_new);
                r = line + strlen(string_orig);
                if (r != NULL) fprintf(outf->File, "%s\n", r);
                /* increase occurance counter */
                encounter++;
            }
            else
                fprintf(outf->File, "%s\n", line);
            continue;
        }
        /* sentense ending substitution */
        if (replace_pos == SENTENCE_ENDING)
        {
            q = line;
            r = line + strlen(line) - strlen(string_orig);
            if ((r != NULL) && (strcmp(r, string_orig) == 0))
            {
                r[0] = 0x0;
                fprintf(outf->File, "%s", line);
                fprintf(outf->File, "%s\n", string_new);
                /* increase occurance counter */
                encounter++;
            }
            else
                fprintf(outf->File, "%s\n", line);
            continue;
        }
        /* sentense global substitution */
        else
        {
            /* search for string to be substituted */
            q = line;

            /* global substitution */
            while ((strstr(q, string_orig)) && (q[0] != 0x0))
            {
                /* if found, locate it */
                p0 = (char *) strstr(q, string_orig);
                /* put a "stop-sign" at the location */
                p0[0] = 0x0;
                /* only output the line before the location */
                fprintf(outf->File, "%s", q);
                /* then output the string to substitute */
                fprintf(outf->File, "%s", string_new);
                /* prepare to search for any further */
                /* occurance in the rest of the string */
                q = p0;
                q += strlen(string_orig);
                /* increase occurance counter */
                encounter++;
            }
            fprintf(outf->File, "%s\n", q);
        }
        if (feof((FILE*)inf->File)) break;
    }
    OSCloseFile( inf);
    OSCloseFile (outf);

    printf("%d lines in file--> %s scanned;\n", line_count, FileName);
    OSRemoveFile ("__intermediate__file__", loc, Error);
    return encounter;
}



int OSReplace (char* Name, char* string_orig, char* string_new, char FileLoc, char mode, int recurse, int* Error)
{

    char FileName[120];
    char OnlyFileName[120];
    char LocalRepertory[50];
    List* ListFiles = NULL;
    char* Buf = 0;
    DWORD BufSize = 0;
    int NbrFiles = 0;

    OSGetOnlyName (Name, OnlyFileName, LocalRepertory, FileLoc);

    if (strlen (OnlyFileName) == 0)
        strcpy (OnlyFileName, "*");

    if (strcmp (LocalRepertory, "") == 0)
        OSGetDir (LocalRepertory, FileLoc, Error);

    OSSetFileName (FileName, LocalRepertory, OnlyFileName, FileLoc);

    NbrFiles =  OSGetFilesFromWildCards (FileName, &ListFiles, FileLoc, TRUE);

    while (ListFiles)
    {
        FILEDATA* filedata = (FILEDATA *)ListFiles->car;
        if (filedata->Type == TYPEFILE)
        {
            OSReplaceInFile (filedata->Name, string_orig, string_new, mode, 0, Error);
        }
        free (ListFiles->car);
        ListRemove (&ListFiles, ListFiles->car);
    }
    return NbrFiles;


}






int BufferGets(char* Content, int ContentSize, char *s, int StringLength, int* CRFlag, int* DoneFlag, int* Pos)
{
    char cc;
    int count_down = StringLength;
    char* ss = s;

    ss[0] = 0x0;
    while (1)
    {
        if (*Pos == ContentSize)
        {
            ss[0] = 0x0;
            *DoneFlag = 1;
            return 0;
        }
        cc = Content[*Pos];
        (*Pos)++;
        switch (cc)
        {
        case 0x0D:
            ss[0] = 0x0;
            *CRFlag = 1;
            return 0;
            break;

        case 0x0A:
            if (!(*CRFlag))
            {
                ss[0] = 0x0;
                return 0;
            }
            break;
        default:
            if (count_down)
            {
                ss[0] = cc;
                count_down--;
                ss++;
            }
        }
    }
}





int OSFindString (char* Content, int Size, char* string_orig, int occurence)
{
    int  crflag = 0;
    int  done = 0;
    int Pos = 0;
    long line_count = 0   ;
    int encounter = 0;
    char*  q;
    char*  p0;
    int count = 0;

    q = Content;

    /* global substitution */
    while ((strstr(q, string_orig)) && (q[0] != 0x0))
    {
        /* if found, locate it */
        p0 = (char *) strstr(q, string_orig);
        encounter++;
        if ((occurence == 0) ||(encounter == occurence))
        {
            Pos = p0 - Content;
            if (Pos > Size) return -1;
            return (Pos);
        }
        q = p0 + strlen(string_orig);

    }

    return -1;
}






int OSExecuteInFile (char* FileName, char* string_orig, char mode, char loc,
                     int (*funct)(char* file, char* line, int line_pos, int pos, void* appfield), void* appfield, int* Error)
{
    FILEID*  inf;
    char*  p0;
    char*  q;
    char*  r;
    char*  command;
    char*  line;
    char command_buf[108];
    int  done = 0;
    int  crflag = 0;
    long line_count = 0;
    int encounter = 0;
    char  line_buf[4096];
    int   replace_pos;

    line = (char *) (&line_buf);
    line[0] = 0x0;
    replace_pos = GLOBAL;

    if ((mode == 'H') || (mode == 'B'))
        replace_pos = SENTENCE_BEGINNING;
    else
        if ((mode == 'T') || (mode ==  'E'))
            replace_pos = SENTENCE_ENDING;
        else
            if (mode == 'G')
                replace_pos = GLOBAL;
            else return -1;

    /* clean up input line */
    line[0] = 0x0;
    command = (char *) (&command_buf);

    /* check input file existance */
    if ((inf = OSOpenFile(FileName, "r", 't',loc, Error)) == NULL)
    {
        return -1;
    }

    /* main loop */
    while (!done)
    {
        fggets(inf->File, line, 4096, &crflag, &done);

        if ((feof((FILE*)inf->File)) && (strlen(line) == 0))
            break;
        else
            line_count++;

        /* sentense beginning substitution */
        if (replace_pos == SENTENCE_BEGINNING)
        {
            q = (char *) strstr(line, string_orig);
            if (q == line)
            {
                funct (FileName, line, line_count, 0, appfield);
                encounter++;
            }
            continue;
        }
        /* sentense ending substitution */
        if (replace_pos == SENTENCE_ENDING)
        {
            q = line;
            r = line + strlen(line) - strlen(string_orig);
            if ((r != NULL) && (strcmp(r, string_orig) == 0))
            {
                funct (FileName, line, line_count, r - line, appfield);
                encounter++;
            }
            continue;
        }
        /* sentense global substitution */
        else
        {
            /* search for string to be substituted */
            q = line;

            /* global substitution */
            while ((strstr(q, string_orig)) && (q[0] != 0x0))
            {
                /* if found, locate it */
                p0 = (char *) strstr(q, string_orig);
                /* put a "stop-sign" at the location */
                funct (FileName, line, line_count, p0 - line, appfield);
                p0[0] = 0x0;
                q = p0;
                q += strlen(string_orig);
                /* increase occurance counter */
                encounter++;
            }
        }
        if (feof((FILE*)inf->File)) break;
    }
    OSCloseFile( inf);

    printf("%d lines in file--> %s scanned;\n", line_count, FileName);
    return encounter;
}



int OSSearchAndExecute (char* Name, char* string_orig, char FileLoc, char mode,
                        int (*funct)(char* file, char* line, int line_pos, int pos, void* appfield), void* appfield, int* Error)
{
    char FileName[120];
    char OnlyFileName[120];
    char LocalRepertory[50];
    List* ListFiles = NULL;
    char* Buf = 0;
    DWORD BufSize = 0;
    int NbrFiles = 0;

    OSGetOnlyName (Name, OnlyFileName, LocalRepertory, FileLoc);

    if (strlen (OnlyFileName) == 0)
        strcpy (OnlyFileName, "*");

    if (strcmp (LocalRepertory, "") == 0)
        OSGetDir (LocalRepertory, FileLoc, Error);

    OSSetFileName (FileName, LocalRepertory, OnlyFileName, FileLoc);

    NbrFiles =  OSGetFilesFromWildCards (FileName, &ListFiles, FileLoc, TRUE);

    while (ListFiles)
    {
        FILEDATA* filedata = (FILEDATA *)ListFiles->car;
        if (filedata->Type == TYPEFILE)
        {
            OSExecuteInFile (filedata->Name, string_orig, mode, FileLoc, funct, appfield, Error);
        }
        free (ListFiles->car);
        ListRemove (&ListFiles, ListFiles->car);
    }
    return NbrFiles;
}

