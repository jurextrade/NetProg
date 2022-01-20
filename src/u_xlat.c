/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS

#include "u_xlat.h"
#include "oslist.h"


#ifndef max
#define max(a,b) ((a > b) ? a : b)
#define min(a,b) ((a > b) ? b : a)
#endif


/* tableau de pointeurs sur les structures de tables de conversion */

List* TabXlat = NULL;

static XLAT_TBL* GetXlat(char* from, char* to)
{
    XLAT_TBL* t;
    List* Tables = TabXlat;

    while (Tables)
    {
        t = (XLAT_TBL*)Tables->car;
        /* test sur from et from */
        if (strcmp(from, t->From))
        {
            /* from et from diffårents, test croiså  */
            if (!strcmp(from, t->To) && !strcmp(to, t->From)) return t;
        }
        else /* from et from Ègaux */
            if (!strcmp( to, t->To)) return t;
        Tables = Tables->cdr;
    }
    return NULL; /* pas trouve */
}



/*-------------------------------------------------------------------------*/
/* fonction de chargement d'une table de conversion                        */
/* retourne OK ou NOTFOUND ou NOMEM                                        */
/*-------------------------------------------------------------------------*/


XLAT_TBL* LdXlTbl(char* nomfic)
{
    int rc = 0;
    int Etat = 0;
    char From[MAXCODE+1], To[MAXCODE+1];
    XLAT_TBL *new1, *sav;
    FILE *f;
    char buf[XLAT_MAXBUF+1];
    int inverse = 0; /* = 1 si en modif, tables inversÈes */

    if (( f = fopen( nomfic, "r" )) == NULL)
        return NULL;

    new1 = (XLAT_TBL *)malloc (sizeof(XLAT_TBL));

    if (new1 == NULL)
    {
        fclose(f);
        return NULL;
    }

    /* initialisation table */

    /* lecture du fichier */

    while (ReadSM(f, buf, XLAT_MAXBUF))
    {
        strtok(buf, ";*");
        stripe(buf);
        if (strpbrk(buf,";*") || (*buf == '\0'))
            continue; /* sauter les lignes blanches ou comm */
        /*    strupr(buf);*/

        switch (Etat)
        {
        case 0 :
        case 1 :
            if (strlen(buf) > 6)
            {
                rc = INVALID;
                goto End;
            }
            switch (Etat)
            {
            case 0: /* lecture jeu dÈpart */
                strmcpy(From, buf, MAXCODE);
                Etat = 1;
                break;
            case 1 : /* lecture jeu d'arrivÈe */
                strmcpy(To, buf, MAXCODE);
                /* vÈrification existence table */
                if ((sav = GetXlat(To, From)) != NULL)
                {
                    /* positionnement flag inversion */
                    inverse = strcmp(sav->From, From) != 0 ? 1 : 0;
                    /* passage en mode modif */
                    rc = ALREADY_EXISTS;
                    free (new1);
                    new1 = sav;
                }
                else
                {
                    ListInsert (&TabXlat,new1);
                    strcpy(new1->From, From);
                    strcpy(new1->To, To);
                }
                Etat = 2;
                break;
            }
            break;
        case 2 : /* lecture des codes */
            {
                int n;
                char c[2];
                int i1, i2; /* codes dÈpart et arrivÈe */

                /* interprÈtation ligne */
                n = sscanf(buf, "%2x %[<=>] %2x", &i1, c, &i2);
                if (n != 3) break; /* ignorer ligne invalide */
                switch (*c)
                {
                case '=' : /* conversion dans les deux tables     */
                    new1->Tables[0][i1] = i2;
                    new1->Tables[1][i2] = i1;
                    break;
                case '>' : /* conversion dans la table from -> to */
                    new1->Tables[inverse][i1] = i2;
                    break;
                case '<' : /* conversion dans la table rÈciproque */
                    new1->Tables[not(inverse)][i2] = i1;
                    break;
                }
            }
        } /* fin switch etat */
    } /* fin while lecture */
    /* positionnement flag bijective */
    {
        int i = 0;
        new1->IsBiject = 1;
        for(; i<256; i++)
            if ((unsigned char)new1->Tables[1][(unsigned char)new1->Tables[0][i]] != i)
            {
                /* rÈciproque de l'image non Ègale ‡ ÈlÈment dÈpart */
                new1->IsBiject = 0;
                break;
            }
    }
End :
    ListInsert (&TabXlat, new1);
    fclose( f );
    return new1;
}

/* choix d'une conversion */

XLAT* InitXlat(char* from, char* to)
{
    XLAT *ret = (XLAT *)calloc(1, sizeof(XLAT));

    if (ret == NULL)  return NULL;
    if (!strcmp(from, to))
    {
        free(ret);
        return NULL;
    }
    ret->Tab = GetXlat(from, to);
    if (ret->Tab == NULL)
    {
        free(ret);
        return NULL;
    }
    ret->FromTo = strcmp(ret->Tab->From, from) != 0 ? 1 : 0;
    return ret;
}


char* Xlat( XLAT* x, char* dst, char* src, size_t len )
{
    size_t i=0;
    char* p = dst;
    char *Tbl;

    if (x == NULL) return (char *)memcpy( dst, src,len);
    Tbl = x->Tab->Tables[x->FromTo];
    for (;i<len; i++)
    {
        *p = Tbl[(unsigned char)*src];
        p++;src++;
    }
    return dst;
}


char* UnXlat( XLAT *x, char* dst, char* src, size_t len)
{
    size_t i = 0;
    char*  p = dst;
    char*  Tbl;

    if (x == NULL)
        return (char *)memcpy( dst, src,len);
    Tbl = x->Tab->Tables[not(x->FromTo)];
    for (;i<len; i++)
    {
        *p = Tbl[(unsigned char)*src];
        p++;src++;
    }
    return dst;
}



char* Xlats (XLAT* x, char* str)
{
    char* mem = str;
    char* Tbl;

    if (x == NULL) return str;
    Tbl = x->Tab->Tables[x->FromTo];
    while (*str)
    {
        *str = Tbl[(unsigned char)*str];
        str++;
    }
    return mem;
}

char* UnXlats (XLAT* x, char* str)
{
    char* mem = str;
    char* Tbl;

    if (x == NULL) return str;
    Tbl = x->Tab->Tables[not(x->FromTo)];
    while (*str)
    {
        *str = Tbl[(unsigned char)*str];
        str++;
    }
    return mem;
}



char Xlatc (XLAT* x, char c)
{
    return MXlatc(x,c);
}

char UnXlatc (XLAT* x, char c)
{
    return MUnXlatc(x,c);
}

void EndXlat (XLAT* x )
{
    if (x != NULL) free(x);
}

/* libÈration des tables de conversion */

void FreeXlat (void)
{
    XLAT_TBL *Table ;
    while (TabXlat)
    {
        ListRemove (&TabXlat, (Table = TabXlat->car));
        free (Table);
    }
}


/*-------------------------------------------------------------------------*/
/* String Manipulations                                                    */
/*-------------------------------------------------------------------------*/

char _temp_str[256];

/* Delete blanks from beginning and end of string*/

char* stripe (char *src)
{
    char *dest = src;
    char *ptr = dest;
    char *last = dest;

    if ( src == NULL )
        return NULL;

    while( *src == ' ')
        src++;

    while(*src)
    {
        if (*src != ' ')
        {
            last = dest+1 ;
        }
        *dest = *src;
        dest++; src++;
    }
    *last = 0;
    return ptr;
}

/* Copie n chaines dans la premiËre, en les concatÈnant */

char* strcpyv (char *str1, ... )
{
    va_list vp;
    char *s;

    if (str1 == NULL)
        str1 = _temp_str;

    *str1 = 0;
    va_start(vp, str1);
    s = va_arg(vp, char *);

    while (s != NULL)
    {
        strcat(str1, s);
        s = va_arg(vp, char *);
    }

    va_end(vp);
    return str1;
}

/* concat®ne n chaines „ la suite de str1 */
char *strcatv(char *str1, ... )
{
    va_list vp;
    char *s;

    if (str1 == NULL)   str1 = _temp_str;

    va_start(vp, str1);

    s = va_arg(vp, char *);

    while (s != NULL)
    {
        strcat(str1, s);
        s = va_arg(vp, char *);
    }

    va_end(vp);
    return str1;
}

/* copie s2 dans s1, en ajoutant des blancs jusqu'„ une taille de n */

char *strfcpy( char *s1, const char *s2 , int n)
{

    char *p = s1;
    int flag = 0;  /* fin de s2 atteinte */

    for ( ; n ; n-- )
    {
        if (flag)   *s1++ = ' ';
        else
            if (*s2)
            {
                *s1 = *s2;
                s1++;
                s2++;
            }
            else
            {
                flag++;
                *s1 = ' ';
                s1++;
            }
    }
    *s1 = 0;
    return p;
}



/* copie s2 dans s1, jusqu'„ une taille de n, ajoute le nul de fin */
char *strmcpy( char *s1, char *s2 , int n)
{
    /* retourne un pointeur sur s1 */
    char *p = s1;

    for ( ; n ; n-- )
    {
        if (*s2 == '\0') break;
        *s1 = *s2;
        s1++;s2++;
    }
    *s1 = 0;
    return p;
}

/*------------------------------------------------------------------*/

static void deferrmem(void);

void (*ToGenErrMem)(void) = deferrmem;


void ToIniErrMem(TABOBJ *tab, void (*ToErrMem)(void))
{
    if (tab == NULL)
        ToGenErrMem = ToErrMem;
    else
        tab->errmem = ToErrMem;
}

TABOBJ *ToMk( int MaxObj, size_t Size )
{
    TABOBJ *Tab;
    long nrealloc;

    if ( (Tab = (TABOBJ *)calloc(1, sizeof(TABOBJ))) == NULL )
    {
        ToGenErrMem();
        return NULL;
    }
    /* calcul param®tre råalloc */
    /* calcul taille maximum d'un bloc */
    if (Size > TOMAXALLOC)
        nrealloc = 1; /* råallouer „ chaque fois */
    else
    {
        long MaxSize = MaxObj * Size;  /* taille totale pråvue */
        /* prendre la valeur dans la fourchette */
        MaxSize = max( MaxSize, TOMINALLOC); /* limite infårieure */
        MaxSize = min( MaxSize, TOMAXALLOC); /* limite supårieure */
        nrealloc = MaxSize / Size; /* calcul du nb d'objets */
    }
    Tab->nrealloc = (int)nrealloc;
    Tab->size = Size;
    Tab->errmem = ToGenErrMem;
    return Tab;
}


void *ToInsert( TABOBJ *Tab, int Rang, void *Obj )
{
    long newsize;

    if ( (Rang > Tab->nobj) || ( Rang < 0) )
    {
        Tab->errmem();
        return NULL;
    }
    if ( (Tab->nobj % Tab->nrealloc) == 0 )
    {
        newsize = ((long)Tab->nobj + (long)Tab->nrealloc) * (long)Tab->size;
        if (newsize > MAXALLOC) return NULL;
        if ( (Tab->first = (char *)realloc(Tab->first, (size_t)newsize))
                == NULL )
        {
            Tab->errmem();
            return NULL;
        }
    }

    if (Tab->nobj-Rang > 0) /* dåplacer sauf si en fin de tableau */
        memmove( (char*)ToObj(Tab,Rang)+Tab->size, ToObj(Tab, Rang),
                 (Tab->nobj - Rang) * Tab->size );
    ++Tab->nobj;
    if (Obj != NULL)
        memcpy(ToObj(Tab,Rang), Obj, Tab->size);
    else
        memset(ToObj(Tab,Rang), 0, Tab->size);
    return ToObj(Tab, Rang);
}


/*****
 * Exåcution, pour tous les objets de la table,
 * d'une fonction passåe en argument.
 *****/
int ToExec( TABOBJ *Tab, TOEXEC fct )
{
    int i;

    for (i = 0; i < Tab->nobj; i++)
        if( fct(ToObj(Tab,i)) != 0)
            return 1;
    return 0;
}

/*****
 * Exåcution, pour tous les objets de la table,
 * d'une fonction passåe en argument et prenant un autre argument qcq
 * passå par adresse.
 *****/
int ToExecArg( TABOBJ *Tab, TOEXECARG fct, void * Arg )
{
    int i;
    if ( Tab == NULL) return 1;
    for (i = 0; i < Tab->nobj; i++)
        if( fct( ToObj(Tab, i), Arg) != 0 )
            return 1;
    return 0;
}

/*****
 * Suppression de l'objet de rang n, sans dåsallouer la måmoire
 * Revient „ "dåcrocher" l'objet.
 * Retourne un pointeur sur l'objet ou NULL
 *****/
void *ToTakeObj( TABOBJ *Tab, int Rang )
{
    void *obj;

    if ( (Rang >= Tab->nobj) || (Rang < 0) ) return NULL;
    obj = calloc( 1, Tab->size);
    if (obj == NULL) return NULL;
    memcpy(obj, ToObj(Tab, Rang), Tab->size);
    if (Tab->nobj - Rang -1 > 0)
        memmove( ToObj(Tab,Rang), ToObj(Tab, Rang+1),
                 (size_t) ((Tab->nobj - Rang -1 ) * Tab->size) );
    if ( (--Tab->nobj % Tab->nrealloc) == 0 )
    {
        if (Tab->nobj == 0)
        {
            /* pour corriger le bug de realloc qui ne renvoie pas NULL */
            /* uniquement pour TC ++ 1.0 */
            free(Tab->first);
            Tab->first = NULL;
        }
        else
            Tab->first = (char *)realloc(Tab->first, (size_t)Tab->nobj * (size_t)Tab->size);
    }
    return obj;
}

/*****
 * Suppression de l'objet de rang n.
 * Retourne le nouveau nombre d'objets de la table.
 * retourne -1 en cas d'erreur dans les param®tres
 * Si la table est type "fast", la måmoire allouåe pour l'objet
 * n'est pas libåråe.
 *****/
int ToDelObj( TABOBJ *Tab, int Rang )
{
    void *obj;

    if ((obj = ToTakeObj( Tab, Rang)) == NULL)
    {
        Tab->errmem();
        return -1;
    }
    free(obj);
    return Tab->nobj;
}




/*****
 * Destruction de la table et de tous les objets qu'elle contient.
 *****/
void ToDel( TABOBJ *Tab )
{
    if ( Tab == NULL) return;
    /* libåration du tableau */
    if (Tab->nobj) free(Tab->first);
    free(Tab);
}

/*****
 * Destruction de la table et de tous les objets qu'elle contient.
 * Mise „ NULL de *PtrTab
 *****/
void ToDelAdr( TABOBJ ** PtrTab)
{
    ToDel(*PtrTab);
    *PtrTab = NULL;
}

/*****
  Ajout de plusieurs objets
  Retourne l'adresse du tableau cråå, NULL si plus de måmoire
*****/
void *ToAddSvl( TABOBJ *Tab, int n)
{
    long newsize = ((long)Tab->nobj + (long)n) * (long)Tab->size;

    if (newsize > MAXALLOC) return NULL;
    if ( (Tab->first = (char *)realloc(Tab->first, (size_t)newsize))
            == NULL )
    {
        Tab->errmem();
        return NULL;
    }
    Tab->nobj += n;
    return Tab->first;
}

/*****
  Recopie d'un tableau source
  Le tableau destination doit §tre cråå au pråalable
  retourne l'adresse du premier objet copiå dans dest si OK
  NULL si pb måmoire
*****/
void *ToCopy(TABOBJ *dest, TABOBJ *source)
{
    /* råcup®re l'adresse du N+1 ®me ålåment pour memcpy */
    char *p = (char*)ToLastObj(dest)+ToSizeObj(dest);

    if (ToAddSeveral(dest, ToSize(source)) == NULL)
        return NULL;
    /* recopie */
    memcpy(p, ToTab(source), ToSize(source) * ToSizeObj(source));
    return p;
}

/***********************************************************************/

/*********************/
/* Fonctions locales */
/*********************/

/*
 * Fonction par dåfaut pour dåbordement måmoire.
 */
static void deferrmem(void)
{
    exit(1);
}




/*-------------------------------------------------------------------------*/
/* lecture d'une ligne d'un fichier texte, fonction gånårale               */
/* option peut prendre les valeurs :                                       */
/*   S_COLOR   : pour ne pas compter les car. de contrØle couleur          */
/*   S_TRUNC   : pour tronquer „ max, sans aller jusqu'au cr suivant       */
/*   S_FILL    : pour remplir jusqu'„ max avec des blancs                  */
/*-------------------------------------------------------------------------*/



size_t RdSGnrl(FILE *f, char *s, int max, int option)
{
    int lus = 0;  /* nb d'octets lus */
    int c;
    int nocoul = !(option & S_COLOR);

    while ( (c = getc(f)) != EOF)
    {
        if (nocoul || ((char)c != COLORCAR)) lus++;
        if (c == '\n')  /* fin de ligne cr   */
        {
            if (option & S_FILL)
                for (; lus <= max ; lus++)
                {
                    *s = ' ';
                    s++;
                }
            break;
        }
        if (lus > max)
        {
            if (option & (S_TRUNC | S_FILL))
            {
                if (c != '\n') ungetc(c,f);
                break;
            }
        }
        else
        {
            *s = c;
            s++;
        }
    }
    if (option & S_FILL)
        for (; lus <= max ; lus++)
        {
            *s = ' ';
            s++;
        }
    *s = 0;
    return lus;
}

/* Ecriture d'une chaine dans un fichier texte, + saut de ligne  */
/* ouvert en wt ou rt+                                           */
/* ne pas oublier, dans le cas rt+, d'appeler
** fseek(f,0,SEEK_CUR) avant åcriture, si l'opåration pråcådente
** åtait une lecture, et vice-versa.
** retourne le nb de car. åcrits si OK, 0 sinon
*/
size_t WriteS(FILE *f, char *s)
{
    int ecrits=0;

    for (;*s;ecrits++)
    {
        putc(*s,f);
        s++;
    }
    if (putc('\n',f) == EOF) return 0;
    return ++ecrits;
}
