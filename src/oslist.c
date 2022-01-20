/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "oslist.h"


int ListMember (void* elt, List* lis)
{
    List* l;
    int member = 0;
    l = lis;
    while (l != NULL)
    {
        ++member;
        if (l->car == elt) return member;
        l= l->cdr;
    }
    return 0;
}


int ListStringMember (char* elt, List* lis)
{
    List* l;
    int member = 0;
    l = lis;
    while (l != NULL)
    {
        ++member;
        if (strcmp (l->car, elt) == 0) return member;
        l= l->cdr;
    }
    return 0;
}

//begining insert
void ListInsert (List** lis, void* elt)
{
    List* l;

    l = (List*) malloc(sizeof(List));
    l->car = elt;
    if (*lis == NULL) l->cdr = NULL;
    else l->cdr = *lis;
    *lis = l;
}

//end insert
void ListNewr (List** lis, void* elt)
{
    List* l;
    List* l1;

    l = ((List*) malloc(sizeof(List)));
    l->cdr = NULL;
    l->car = elt;

    if (*lis == NULL) *lis = l;
    else
    {
        l1 = *lis;
        while (l1->cdr != NULL)
            l1 = l1->cdr;
        l1->cdr = l;
    }
}

void ListRemove (List** lis, void* elt)
{
    List* l;
    List* l1;
    int notfound = 1;

    if (*lis)
    {
        l = (*lis);
        if (l->car == elt)
        {
            *lis = l->cdr;
            free (l);
        }
        else
        {
            l1 = (*lis)->cdr;
            while ((l1 != NULL) && (notfound))
                if (l1->car == elt)
                {
                    l->cdr = l1->cdr;
                    notfound = 0;
                    free (l1);
                }
                else
                {
                    l = l1;
                    l1 = l1->cdr;
                }
        }
    }
}


void ListInsertAfter (List** lis, void* elt, void* a_elt)
{
    List* l;
    List* l1 = *lis;

    if (a_elt)
    {

        while (l1 != NULL)
		{
            if (l1->car == a_elt)
            {
                l = (List*) malloc(sizeof(List));
				l->car = elt; 
				l->cdr = l1->cdr;
                l1->cdr = l;
				break;
            }
            l1 =l1->cdr;
		}
    }
    else
    {
		ListInsert (lis, elt);
    }
}



int ListNbElt (List* l)
{
    List* l1 = l;
    int nbelt = 0;
    while (l1)
    {
        nbelt++;
        l1= l1->cdr;
    }
    return nbelt;
}


void* ListGetElt (List* l, int i)
{
    int j = 0;

    if (i >= ListNbElt (l)) return NULL;

    while (j < i)
    {
        l = l->cdr;
        j++;
    }
    return l->car;
}


void* ListGetLastElt (List* l)
{
    List* l1 = l;
    void* lastelt = NULL;
    while (l1)
    {
        lastelt = l1->car;
        l1 = l1->cdr;
    }
    return lastelt;
}
