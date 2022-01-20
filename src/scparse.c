/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "sclex.h"




SCSection* SCIsSectionDefinedInList(List* listsections, char* name)
{
    SCSection* psection;

    if (!listsections) return(NULL);
    if (!name) return(NULL);

    while(listsections)
    {
        psection = (SCSection*)listsections->car;
        if ((psection->Name) &&
                (OSStrcmp (psection->Name, name) == 0))
            return psection;
        listsections = listsections->cdr;
    }
    return(NULL);
}

SCEntity* SCCopySections(SC* psc, List** listsections)
{
    SCEntity*   pentity      = NULL;
    SCEntity*   pentity_save = NULL;
    SCSection*  psection;
    SCSection*  psection_temp;
    SCVariable* pvariable;
    SCAction* paction;

    if (!(*listsections)) return(NULL);

    /*  SCTraceSections(*listsections);*/

    while(*listsections)
    {
        psection = (SCSection*)(*listsections)->car;

        if ((psection->Name) &&
                (OSStrcmp (psection->Name, "MAIN") == 0))
        {
            /* copie des entities vers SC */
            while(psection->ListEntities)
            {
                pentity = (SCEntity*)psection->ListEntities->car;
                /* on sauvegarde le premier pointeur de la liste */
                if (!pentity_save)       pentity_save = pentity;
                paction = (SCAction*)pentity->Val;
                ListRemove(&psection->ListEntities, pentity);
                ListNewr(&psc->ListEntities, pentity);
            }
            /* copie des variables vers SC */
            while(psection->ListVariables)
            {
                pvariable = (SCVariable*)psection->ListVariables->car;
                ListRemove(&psection->ListVariables, pvariable);
                ListNewr(&psc->ListVariables, pvariable);
            }

            ListRemove(listsections, psection);
            /*SCFreeSection(psection);*/
        }
        else
        {
            /* on supprime la section si elle existe avant d'insérer l'autre de meme nom */
            psection_temp = SCIsSectionDefinedInList(psc->ListSections, psection->Name);
            if (psection_temp)
            {
                ListRemove(&psc->ListSections, psection_temp);
                psc->NbSections--;
                SCFreeSection(psection_temp);
            }
            /* on supprime le pointeur de la liste et on l'insère dans l'autre */
            ListNewr(&psc->ListSections, psection);
            ListRemove(listsections, psection);
            psc->NbSections++;
        }
    }

    return(pentity_save);
}

/*------------------------------------------------------------------------*/
/* parse a string or file (lex-yacc)                                      */
/*------------------------------------------------------------------------*/


List* SCReturnParseSections ()
{
	return SCReturnParseSection;
}


SCEntity* SCParseString(SC* psc, char* Chaine, int* error)
{
    SCEntity*  pentity;
    SCSection* MainSection;
    *error = 0;

    if ((!Chaine) || (Chaine[0] == 0))
    {
        //SCError(psc, 8, NULL);
        *error =  8;
        return(NULL);
    }

    if (SCInitEntryWithString(Chaine) < 0)
    {
        //SCError(psc, 10, NULL);
        *error = 10;
        return(NULL);
    }

    MainSection = (SCSection *)SCReturnParseSection->car;
    if (MainSection) strcpy (MainSection->FileName,    psc->CurrentFileName);

    if (yyparse(psc) != 0)
    {
        //SCError(psc, 12, StrErr);
		*error = 12;
        return(NULL);
    }

    pentity = SCCopySections(psc, &SCReturnParseSection);

    return(pentity);
}

char* SCGetStringError ()
{
	return StrErr;
}


SCEntity* SCParseFile(SC* psc, char* FileName, int* error)
{
    FILE *FileParse = NULL;
    SCEntity* pentity;
    SCSection* MainSection = NULL;

    *error = 0;

    if ((!FileName) ||
            ((FileParse = fopen(FileName, "r")) == NULL))
    {
        sprintf(StrErr, "[File : %s][line : 0] \n", FileName);
        SCError(psc, 9, (char*)StrErr);
        *error = 9;
        return(NULL);
    }

    if (SCInitEntryWithFile(FileParse) < 0)
    {
        SCError(psc, 11, NULL);
        *error = 11;
        return(NULL);
    }

    MainSection = (SCSection *)SCReturnParseSection->car;
    if (MainSection) strcpy (MainSection->FileName, FileName);
    strcpy(psc->CurrentFileName, FileName);


    if (yyparse(psc) != 0)
    {
        fclose(FileParse);
        SCError(psc, 12, StrErr);
        *error = 12;
        return(NULL);
    }

    fclose(FileParse);

    pentity = SCCopySections(psc, &SCReturnParseSection);

    return(pentity);
}

/*------------------------------------------------------------------------*/
/* functions related to chapters (script files)                           */
/*------------------------------------------------------------------------*/

SCSection* SCCreateInteractiveSection (SC* psc)
{
    psc->InteractiveSection = SCAllocSection(psc, "INTERACTIVE");
    return psc->InteractiveSection;
}

SCSection* SCAllocSection(SC* psc,  char* Name)
{
    SCSection *pSection;
    List* listsections;

    listsections = psc->ListSections;
    while (listsections)
    {
        pSection = listsections->car;
        if (strcmp (pSection->Name, Name) == 0)
            return NULL;
        listsections = listsections->cdr;
    }
    pSection = (SCSection *)malloc(sizeof(SCSection));
    if (!pSection) return(NULL);

    pSection->Name = (char *)malloc(strlen(Name) + 1);
    if (!pSection->Name)
    {
        free(pSection);
        return(NULL);
    }

    strcpy(pSection->Name, Name);
    strcpy (pSection->FileName, "");
    psc->CurrentSection     = pSection;
    pSection->ListEntities  = NULL;
    pSection->ListVariables = NULL;
    pSection->NbEntity      = 0;
    pSection->NbVariables   = 0;
    pSection->LineNumber = 0;
    ListNewr(&psc->ListSections, pSection);
    psc->NbSections ++;
    return(pSection);
}

void SCFreeSection(SCSection *pSection)
{
    SCEntity *pEntity;

    if (pSection)
    {
        if (pSection->Name) free(pSection->Name);

        while (pSection->ListEntities)
        {
            pEntity = (SCEntity *)pSection->ListEntities->car;
            ListRemove(&pSection->ListEntities, pEntity);
            SCFreeEntity(pEntity);
        }
        free(pSection);
    }
    return;
}
/*------------------------------------------------------------------------*/

