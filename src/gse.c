/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "gse.h"



void  GSEInit (GSE* pgse)
{
    pgse->Applications = NULL;
    OSDrawInit();
}


GSEApplication* GSEFindApplication (GSE* pgse, char* appli_name)
{
    List* l_appli = pgse->Applications;

    while (l_appli)
    {
        if (strcmp (((GSEApplication *) l_appli->car)->Name,appli_name) == 0)
            return ((GSEApplication*)l_appli->car);
        l_appli = l_appli->cdr;
    }
    return NULL;
}

GSELinkType* GSEFindLinktype (GSE* pgse, GSEApplication* appli, char* linktype_name)
{
    List* l_linktype = appli->LinkTypes;

    while (l_linktype != NULL)
    {
        if (strcmp (((GSELinkType *)l_linktype->car)->Name, linktype_name) == 0)
            return ((GSELinkType*)l_linktype->car);
        l_linktype = l_linktype->cdr;
    }
    return (NULL);
}



GSESpace* GSEFindSpace (GSE* pgse, GSEApplication* appli, char* space_name)
{
    List* l_space = appli->Spaces;

    while (l_space != NULL)
    {
        if (strcmp (((GSESpace *)l_space->car)->Name, space_name) == 0)
            return ((GSESpace*)l_space->car);
        l_space = l_space->cdr;
    }
    return (NULL);
}


GSEApplication*  GSECreateApplication (GSE* pgse, char* appli_name)
{
    GSEApplication* appli;

    if ((appli = GSEFindApplication (pgse, appli_name)) != NULL)
        return appli;

    appli = ((GSEApplication *) malloc (sizeof (GSEApplication)));
    strcpy (appli->Name , appli_name);
    appli->Objects   = 0;
    appli->Links     = 0;
    appli->LinkTypes = 0;
    appli->Windows   = 0;
    appli->Spaces    = 0;
    ListNewr(&pgse->Applications, appli);
    return appli;
}





GSESpace*  GSECreateSpace(GSE* pgse, char* appli_name, char* space_name)
{
    GSEApplication* appli;
    GSESpace* space;
    appli = GSEFindApplication(pgse, appli_name);
    if (!appli) return NULL;

    space = GSEFindSpace (pgse, appli, space_name);
    if (space) return space;
    space = ((GSESpace *) malloc (sizeof (GSESpace)));
    strcpy (space->Name , space_name);
    space->Code = 0;
    space->Font = OSFONT8;
    space->Pattern = 0;
    space->Resource = 0;
    space->DrawingProcedure = GSEDefaultDrawingProcedure;
    space->ResizeProcedure  = GSEDefaultResizeProcedure;
    space->RemoveProcedure = NULL;
    ListNewr(&appli->Spaces, space);
    return(space);
}



GSELinkType*  GSECreateLinkType (GSE* pgse, char* appli_name, char* linktype_name)
{
    GSEApplication* appli;
    GSELinkType* linkt;
    appli = GSEFindApplication(pgse, appli_name);

    if (!appli)  return NULL;

    linkt = GSEFindLinktype (pgse, appli, linktype_name);
    if (linkt) return linkt;
    linkt = ((GSELinkType *) malloc (sizeof (GSELinkType)));
    strcpy (linkt->Name , linktype_name);
    linkt->FromSpaces = 0;
    linkt->ToSpaces   = 0;
    linkt->Style      = 0;
    ListNewr(&appli->LinkTypes ,linkt);
    return(linkt);
}



void  GSEDomainLinkType (GSE* pgse, char* application_name, char* linktype_name, BYTE type, char* space_name)
{
    GSESpace* space;
    GSEApplication* appli;
    GSELinkType* linktype;
    appli = GSEFindApplication (pgse, application_name);
    if (appli)
    {
        space = GSEFindSpace(pgse, appli, space_name);
        if (!space) return;
        linktype = GSEFindLinktype (pgse, appli, linktype_name);
        if (!linktype) return;

        if (type == GSEFROMSPACE)    ListNewr (&linktype->FromSpaces, space);
        if (type == GSETOSPACE)      ListNewr (&linktype->ToSpaces, space);
    }
}

int GSESetSpaceRemoveProcedure (GSE* pgse, char* application_name, char* space_name, int (*funct) (GSEObject* pobj))
{
    GSEApplication* appli;
    GSESpace* space;
    appli = GSEFindApplication(pgse, application_name);
    if (!appli) return -1;

    space = GSEFindSpace (pgse, appli, space_name);
    if (!space) return -1;
    space->RemoveProcedure = funct;
    return 1;
}

void  GSESetRootWindow (GSE* pgse, GSEObject* root, GSERect* win)
{
    win->Root = root;
}



void  GSESetModeWindow (GSE* pgse, GSERect* win, int mode)
{
    win->Mode = mode;
}



void  GSEPutStyleToLinkType(GSE* pgse, char* application_name, char* linktype_name, int style)
{
    GSELinkType* linktype;
    GSEApplication* appli;

    appli = GSEFindApplication (pgse, application_name);
    if (!appli) return;

    linktype = GSEFindLinktype (pgse, appli, linktype_name);
    if (linktype)    linktype->Style = style;
}


void  GSEPutPatternToSpace (GSE* pgse, char* application_name, char* space_name, DWORD pattern)
{
    GSESpace* space;
    GSEApplication* appli;

    appli = GSEFindApplication (pgse, application_name);
    if (!appli) return;

    space = GSEFindSpace (pgse, appli, space_name);
    if (space)    space->Pattern = pattern;
}


void  GSEPutFontSpace (GSE* pgse, char* application_name, char* space_name, int Font)
{
    GSESpace* space;
    GSEApplication* appli;

    appli = GSEFindApplication (pgse, application_name);
    if (!appli) return;

    space = GSEFindSpace (pgse, appli, space_name);
    if (space)  space->Font = Font;
}


void  GSESetVisibleSpace (GSE* pgse, char* application_name, char* space_name, int visible)
{
    GSESpace* space;
    GSEApplication* appli;
    List* Objects;
    GSEObject* Object;

    appli = GSEFindApplication (pgse, application_name);
    if (!appli) return;

    space = GSEFindSpace (pgse, appli, space_name);
    if (!space)  return;

    Objects = (List *)appli->Objects;
    while (Objects)
    {
        Object = (GSEObject *)Objects->car;
        if (Object->Space == space)
            Object->Visible = visible;
        Objects = Objects->cdr;
    }
}



void  GSEPutCodeSpace (GSE* pgse, char* application_name, char* space_name, int Code)
{
    GSESpace* space;
    GSEApplication* appli;

    appli = GSEFindApplication (pgse, application_name);
    if (!appli) return;

    space = GSEFindSpace (pgse, appli, space_name);
    if (space)  space->Code = Code;
}


void  GSEPutResourceSpace (GSE* pgse, char* application_name, char* space_name, WORD Resource)
{
    GSESpace* space;
    GSEApplication* appli;

    appli = GSEFindApplication (pgse, application_name);
    if (!appli) return;

    space = GSEFindSpace (pgse, appli, space_name);
    if (space)  space->Resource = Resource;
}





GSESpace*  GSEAssociateSpace (GSE* pgse, char* appli_name, char* space_name, GSERect* window)
{
    GSESpace* space;
    GSEApplication* appli;

    appli = GSEFindApplication (pgse, appli_name);
    if (!appli) return NULL;

    space = GSEFindSpace (pgse, appli, space_name);
    if (!space) return NULL;

    ListNewr (&window->Spaces, space);
    return space;
}


GSESpace*  GSEDissociateSpace (GSE* pgse, char* appli_name, char* space_name, GSERect* window)
{
    GSESpace *space;
    GSEApplication* appli;

    appli = GSEFindApplication (pgse, appli_name);
    if (!appli) return NULL;


    space = GSEFindSpace (pgse, appli, space_name);
    if (!space) return NULL;

    ListRemove (&window->Spaces, space);
    return space;
}


GSELinkType*  GSEAssociateLinkType (GSE* pgse, char* appli_name, char* linktype_name, GSERect* window)
{
    GSELinkType* linktype;
    GSEApplication* appli;

    appli = GSEFindApplication (pgse, appli_name);
    if (!appli) return NULL;

    linktype = GSEFindLinktype (pgse, appli, linktype_name);
    if (!linktype) return NULL;

    ListNewr (&window->LinkTypes,linktype);
    return linktype;
}


GSELinkType* GSEDissociateLinkType (GSE* pgse, char* appli_name, char* linktype_name, GSERect* window)
{
    GSELinkType* linktype;
    GSEApplication* appli;

    appli = GSEFindApplication (pgse, appli_name);
    if (!appli) return NULL;


    linktype = GSEFindLinktype (pgse, appli, linktype_name);
    if (!linktype) return NULL;

    ListRemove (&window->LinkTypes,linktype);
    return linktype;
}



void GSEDefaultResizeProcedure (GSEObject* obj, OSRect* rect, LONG dc)
{
    char* label;
	List* list_labels = obj->Name;
	OSRect Rect;
	rect->Right = 0;
	rect->Bottom = 0;
	
	while (list_labels)
	{
		label = ((GSELabel*)(list_labels->car))->Name;
		OSGetTextExtent (label, obj->Space->Font, dc, &Rect.Right , &Rect.Bottom);
		rect->Right = max (rect->Right, Rect.Right);
		rect->Bottom += 20; //Rect.Bottom;
		list_labels = list_labels->cdr;
	}
}


void GSEDefaultDrawingProcedure (GSERect* window, GSEObject* obj, WORD Resource)
{
    double x,y,width,height;
    LONG lx,ty,rx,by;
    OSRect rect;

    x = obj->X ;
    y = obj->Y;
    width = obj->Width;
    height = obj->Height;
    lx = (LONG)view_x(x-width,window);
    ty = (LONG)view_y(y-height,window);
    rx = (LONG)view_x(x+width,window);
    by = (LONG)view_y(y+height,window);

    rect.Left   = lx;
    rect.Top    = ty;
    rect.Right  =  rx;
    rect.Bottom =  by;

    if (obj->Code == GSERECTANGLE)
        OSDrawRectangle (&rect, window->DC);
    else  OSDrawEllipse (&rect, window->DC);

    GSEDisplayLabel (obj->Name, window, obj->Space->Font, &rect, NULL);
}


GSEObject*  GSEMakeNode(GSE* pgse, char* application_name, char* space_name, int code, char* label)
{
    GSEObject* obj;
    GSESpace* space;
    GSEApplication* appli;
    appli = GSEFindApplication(pgse, application_name);
    if (!appli) return NULL;
    space = GSEFindSpace (pgse, appli, space_name);
    if (!space)  return NULL;

    obj = (GSEObject *)malloc (sizeof(GSEObject));
    obj->Type        = 0;
    obj->Visible     = 1;
    obj->Resource    = space->Resource;
    obj->Code        = code;
    obj->Pattern     = space->Pattern;
    obj->Mark        = 0;
    obj->UserField   = 0;
    obj->Association = 0;
    obj->X = obj->Y  = 0;
    obj->Width       = 20;
    obj->Height      = 20;
    obj->Close       = 0;
    obj->Name        = NULL;
    obj->Space       = space;
    obj->Inherit     = NULL;
    obj->Fromherit   = NULL;
    obj->DrawingProcedure = space->DrawingProcedure;
    obj->ResizeProcedure  = space->ResizeProcedure;

    ListNewr (&obj->Name, GSEAllocLabel(label, space->Font));
    ListNewr (&appli->Objects ,obj);
	obj->UserField = NULL;
    return obj;
}


GSELink*  GSEMakeLinkAfter (GSE* pgse, char* application_name, char* linktype_name,
                       GSEObject* from_object, GSEObject* to_object, GSELink* link)
{
    GSELink*        l = NULL;
    GSELinkType*    linktype;
    GSESpace*       from_space;
    GSESpace*       to_space;
    GSEApplication* appli;

    appli = GSEFindApplication(pgse, application_name);
    if (!appli)  return NULL;

    linktype = GSEFindLinktype (pgse, appli, linktype_name);
    if (!linktype) return NULL;

    if ((!from_object) || (!to_object)) return NULL;
    from_space = from_object->Space;
    to_space   = to_object->Space;

    if ((from_object != to_object)&&
            (!GSELinkExist (from_object, to_object, linktype))&&
            (ListMember(from_space, linktype->FromSpaces))&&
            (ListMember (to_space, linktype->ToSpaces)))
    {
        l = GSEAllocLinkAfter(from_object, to_object, link);
        ListNewr(&appli->Links, l);
        l->LinkType = linktype;
        l->Style = linktype->Style;
    }
    return l;
}




GSELink*  GSEMakeLink (GSE* pgse, char* application_name, char* linktype_name,
                       GSEObject* from_object, GSEObject* to_object)
{
    GSELink*        l = NULL;
    GSELinkType*    linktype;
    GSESpace*       from_space;
    GSESpace*       to_space;
    GSEApplication* appli;

    appli = GSEFindApplication(pgse, application_name);
    if (!appli)  return NULL;

    linktype = GSEFindLinktype (pgse, appli, linktype_name);
    if (!linktype) return NULL;

    if ((!from_object) || (!to_object)) return NULL;
    from_space = from_object->Space;
    to_space   = to_object->Space;

    if ((from_object != to_object)&&
            (!GSELinkExist (from_object, to_object, linktype))&&
            (ListMember(from_space, linktype->FromSpaces))&&
            (ListMember (to_space, linktype->ToSpaces)))
    {
        l = GSEAllocLink(from_object, to_object);
        ListNewr(&appli->Links, l);
        l->LinkType = linktype;
        l->Style = linktype->Style;
    }
    return l;
}

void GSEPutLinkAtBeginning (GSE* pgse, GSELink* l)
{
    GSEObject* FromObj;
    GSEObject* ToObj;
    if (!l) return;

    FromObj = l->FromObject;
    ToObj    =l->ToObject;

    ListRemove (&FromObj->Inherit, l);
    ListRemove(&ToObj->Fromherit,l);
    ListInsert (&FromObj->Inherit, l);
    ListInsert (&ToObj->Fromherit,l);
    return;
}

void GSESetName (GSE* pgse, char* application_name, GSEObject* object, char* label)
{
    GSESpace* space = object->Space;
    GSEApplication* appli = GSEFindApplication (pgse, application_name);
    if (!appli) return;
    if (!ListMember (space, appli->Spaces)) return;


    while (object->Name)
    {
        GSELabel* Label = (GSELabel *)object->Name->car;
        if (Label->Name) free (Label->Name);
        ListRemove (&object->Name, Label);
        free (Label);
    }
    ListNewr (&object->Name, GSEAllocLabel(label, space->Font));
}

void GSERemoveNode (GSE* pgse, char* application_name,GSEObject* object)
{
    GSEApplication* appli;
    GSESpace*       space;
    GSELabel* Label;

    space = object->Space;
    appli = GSEFindApplication (pgse, application_name);
    if (!appli) return;
    if (!ListMember (space, appli->Spaces)) return;


    while (object->Name)
    {
        Label = (GSELabel *)object->Name->car;
        if (Label->Name) free (Label->Name);
        ListRemove (&object->Name, Label);
        free (Label);
    }

    while (object->Inherit)
    {
        GSERemoveLink(pgse, application_name, (GSELink*)object->Inherit->car);
    }

    while (object->Fromherit)
    {
        GSERemoveLink(pgse, application_name,(GSELink*)object->Fromherit->car);
    }
    ListRemove (&appli->Objects, object);

    if (object->Space->RemoveProcedure) 
        object->Space->RemoveProcedure (object);
    free (object);

    return ;
}


void GSERemoveLink(GSE* pgse, char* application_name, GSELink* link)
{
    GSEApplication*  appli;
    GSELinkType*     linktype;
    GSEObject*       from_object;
    GSEObject*       to_object;

    appli = GSEFindApplication (pgse, application_name);
    if (!appli) return;

    linktype = link->LinkType;
    if (!ListMember (linktype, appli->LinkTypes)) return;

    from_object = link->FromObject;
    to_object   = link->ToObject;

    ListRemove(&appli->Links, link);

    ListRemove(&from_object->Inherit,link);
    ListRemove(&to_object->Fromherit,link);

    free (link);
    return;
}


void  GSERemoveAllNodes (GSE* pgse, char* application_name)
{
    GSEApplication* appli;
    appli = GSEFindApplication (pgse, application_name);

    if (!appli) return;

    while (appli->Objects)
    {
        GSERemoveNode (pgse, application_name,(GSEObject *)appli->Objects->car);
    }
    return;
}



void  GSERemoveAllLinks (GSE* pgse, char* application_name)
{
    GSEApplication*  appli;
    appli = GSEFindApplication (pgse, application_name);
    if (!appli) return;

    while (appli->Links)
    {
        GSERemoveLink (pgse, application_name,(GSELink *)appli->Links->car);
    }
    return;
}



void  GSEClearApplication (GSE* pgse, char* application_name)
{
    GSEApplication* appli;
    List*           Objects;
    List*           Links;

    OSDrawEnd ();

    appli = GSEFindApplication (pgse, application_name);
    if (!appli) return;

    Objects =  appli->Objects;
    while (Objects)
    {
        free ((BYTE FAR*)Objects->car);
        Objects = Objects->cdr;
    }
    Links =  appli->Links;
    while (Links)
    {
        free ((BYTE FAR*)Links->car);
        Links = Links->cdr;
    }
    appli->Objects = appli->Links  = NULL;
    return;
}


GSELink* GSEAllocLinkAfter (GSEObject* f_object, GSEObject* t_object, GSELink* a_link)
{
    GSELink* l;

    l = (GSELink *) malloc(sizeof (GSELink));

    l->FromObject = f_object;
    l->ToObject   = t_object;

    ListInsertAfter(&f_object->Inherit, l, a_link);
    ListNewr(&t_object->Fromherit, l);

    return l;
}

GSELink* GSEAllocLink (GSEObject* f_object, GSEObject* t_object)
{
    GSELink* l;

    l = (GSELink *) malloc(sizeof (GSELink));

    l->FromObject = f_object;
    l->ToObject = t_object;

    ListNewr (&f_object->Inherit, l);
    ListNewr(&t_object->Fromherit,l);
    return l;
}





GSELabel* GSEAllocLabel (char* s, int Font)
{
    GSELabel* l;
    l = ((GSELabel *) malloc(sizeof (GSELabel)));
    l->Width = 0;
    l->Height = 0;
    l->Font = Font;
    l->Name = (char *)malloc(strlen (s) + 1);
    strcpy (l->Name, s);
    return l;
}




int GSELinkExist (GSEObject* from_object, GSEObject* to_object, GSELinkType* linktype)
{
    int found = 0;
    List* inherit = from_object->Inherit;

    while ((inherit != NULL) && (!found))
    {
        if ((((GSELink *)inherit->car)->ToObject == to_object) &&
                (((GSELink *)inherit->car)->LinkType == linktype))
            found = 1;
        inherit = inherit->cdr;
    }
    return found;
}



void  GSEDisplayLabel (List* l_label, GSERect* window, int Font, OSRect* Rect, char* string)
{
    List*  l;
    char*  s;
    int    format;
	OSRect rect;
	rect.Left = Rect->Left;
	rect.Top  = Rect->Top;
//	Rect->Bottom = 0;
    l = l_label;
    if (string)  OSDrawText (string, Font, window->DC, Rect, 1);
    else
        while (l)
        {
            s = ((GSELabel*)l->car)->Name;
            format = ((GSELabel*)l->car)->Format;
			OSGetTextExtent (s,Font, window->DC, &rect.Right , &rect.Bottom);
			Rect->Bottom = Rect->Top + 20 ; //rect.Bottom ;
            OSDrawText (s, Font, window->DC, Rect, format);
			Rect->Top = Rect->Top + 20; //rect.Bottom;
			l = l->cdr;
        }
}



void  GSEDisplayObject (GSEObject* obj, GSERect* window)
{
    if (obj->Visible  && ListMember(obj->Space, window->Spaces))
        obj->Space->DrawingProcedure (window, obj, obj->Space->Resource);
}




GSERect*  GSEMakeWindow (HWND win, LONG dc)
{
    GSERect* gse_window;
    gse_window = ((GSERect *) malloc (sizeof (GSERect)));
    gse_window->Window    = win;
    gse_window->Viewport  = 0;
    gse_window->Visible   = 1;
    gse_window->Spaces    = 0;
    gse_window->LinkTypes = 0;
    gse_window->Root      = 0;
    gse_window->Ratio_x   = 1.;
    gse_window->Ratio_y   = 1.;
    gse_window->Ratio_r   = 1.;
    gse_window->Scroll_x  = 0;
    gse_window->Scroll_y  = 0;
    gse_window->Mode      = 0;
    gse_window->DC        = dc;

    return gse_window;
}


void GSEMarkLine (GSERect* win, double x,double y,double x1,double y1,double range)
{
    OSRect rect, rect1;
    rect.Left = (WORD)(x1 - range);
    rect.Top =  (WORD)(y1 - range);
    rect.Right = (WORD)(x1 + range);
    rect.Bottom = (WORD)(y1 + range);
    rect1.Left = (WORD)(x - range);
    rect1.Top =  (WORD)(y - range);
    rect1.Right = (WORD)(x + range);
    rect1.Bottom = (WORD)(y + range);

    OSInvertRectangle (&rect, win->DC);
    OSInvertRectangle (&rect1, win->DC);
}



void GSEMarkOnOffLink (GSERect* win, GSELink* link)
{
    GSEObject* from_object = link->FromObject;
    GSEObject*	to_object = link->ToObject;

    double x1 = view_x(from_object->X,win), y1 = view_y(from_object->Y,win),
                x2 = view_x(to_object->X,  win),y2 = view_y(to_object->Y,win);
    GSEMarkLine (win,x1,y1,x2,y2,2);
}



void GSEInvertObject(GSEObject* obj, GSERect* win)
{
    double x,y,w,h;
    OSRect rect;
    if (!obj) return;

    x = obj->X;
    y = obj->Y;
    w = obj->Width ;
    h = obj->Height;
    rect.Left  = (LONG)view_x(x-w,win) + 2,
                 rect.Top   = (LONG)view_y(y-h,win) + 2,
                              rect.Right = (LONG)view_x(x+w,win) - 2,
                                           rect.Bottom =(LONG) view_y(y+h,win)- 2;
    OSInvertRectangle (&rect, win->DC);
}


void GSEMarkOnOffObject(GSEObject* obj, GSERect* win)
{
    double x,y,w,h;
    double lx,ty,rx,by;
    if (!obj) return;

    x = obj->X;
    y = obj->Y;
    w = obj->Width ;
    h = obj->Height;
    lx = view_x(x-w,win);
    ty = view_y(y-h,win);
    rx = view_x(x+w,win);
    by = view_y(y+h,win);
    GSEMarkLine (win , lx, ty, rx, ty, 2);
    GSEMarkLine (win , lx, by, rx, by, 2);
}




void GSETreeTotalRoom (GSEObject* obj, GSERect* win, double* room_lx, double* room_rx,
                       double* room_ty, double* room_by)
{
    List* inher;
    double rx = obj->X + obj->Width,
                lx = obj->X - obj->Width,
                     by = obj->Y + obj->Height,
                          ty = obj->Y - obj->Height;
    if (!obj->Visible) return;

    *room_rx = max (rx,  *room_rx);
    *room_by = max (by, *room_by);
    *room_lx = min (lx ,*room_lx);
    *room_ty = min (ty, *room_ty);
    for (inher = obj->Inherit; inher != NULL; inher=inher->cdr)
    {
        GSELink*   l = (GSELink *)inher->car;
        GSEObject* to_object = l->ToObject;
        GSETreeTotalRoom (to_object, win, room_lx, room_rx, room_ty, room_by);
    }
}


void GSERectTotalRoom (GSE* pgse, char* appli_name, GSERect* win)
{

    GSEObject* obj;
    List*      Objects;
    OSRect     rect;

    GSEApplication* appli;
    double     w_w;
    double     w_h;
    double     room_rx = -3000.,room_ty = 3000.,
                                          room_lx = 3000.,room_by = -3000.;
    appli = GSEFindApplication (pgse, appli_name);

    if (!appli) return;

    OSGetWindowSize (win->Window,  &rect);

    Objects = appli->Objects;
    while (Objects != NULL)
    {
        obj = ((GSEObject *)Objects->car);
        if (obj->Visible)
        /* &&(ListMember (obj->space,win->Spaces)))*/
        {
            double rx = obj->X + obj->Width,
                        lx = obj->X - obj->Width,
                             by = obj->Y + obj->Height,
                                  ty = obj->Y - obj->Height;
            room_rx = max (rx, room_rx);
            room_by = max (by, room_by);
            room_lx = min (lx, room_lx);
            room_ty = min (ty, room_ty);
        }
        Objects = Objects->cdr;
    }

    w_w =  (double)(rect.Right - rect.Left);
    w_h =  (double) (rect.Bottom - rect.Top);

    room_rx = room_rx + 10;
    room_lx = room_lx - 10;
    room_ty = room_ty - 10;
    room_by = room_by + 10;
    win->Room_lx = room_lx;
    win->Room_by = room_by;
    win->Room_rx = room_rx;
    win->Room_ty = room_ty;
    if (!win->Viewport) return;
    win->Scroll_x = 0;
    win->Scroll_y = 0;

    win->Ratio_x = w_w / (room_rx -room_lx);
    win->Ratio_y = w_h / (room_by -room_ty);
    win->Ratio_r = min (win->Ratio_x,win->Ratio_y);
}



double* GSERectFrameViewport (GSERect* from_win, GSERect* to_win)
{
    static double v[6];
    v[0] = view_x (world_x (0,from_win),to_win);
    v[1] = view_y (world_y (0,from_win),to_win);
    /*  v[2] = view_x (world_x (window_width (from_win),from_win),to_win);
        v[3] = view_y (world_y (window_height (from_win),from_win),to_win);*/
    return v;
}

void  GSEDisplayLink (GSELink* link, GSERect* window, int mode)
{
    GSEObject *from_object,*to_object;
    double f_w,f_h,t_w,t_h;
    int first_x,first_y,last_x,last_y;
    from_object = link->FromObject;
    to_object = link->ToObject;
    f_w = from_object->Width;
    t_w = to_object->Width;
    f_h = from_object->Height;
    t_h = to_object->Height;
    if ((from_object->Visible) && (to_object->Visible) &&
            ListMember (link->LinkType, window->LinkTypes))
        switch (window->Mode)
        {
        case 3 :
            if (mode == GSEDIRECTLINK)
            {
                first_x = (int)view_x(from_object->X + f_w, window);
                first_y = (int)view_y(from_object->Y,       window);
                last_x = (int)view_x(to_object->X - t_w,    window);
                last_y = (int)view_y(to_object->Y,          window);
                OSDrawLine (first_x, first_y, last_x, last_y, window->DC);
            }
            else
            {
                first_x = (int)view_x(from_object->X + f_w, window);
                first_y = (int)view_y(from_object->Y,       window);
                last_x = (int)view_x(to_object->X - t_w,    window);
                last_y = (int)view_y(to_object->Y,          window);
                OSDrawLine (first_x, first_y, first_x + 5, first_y, window->DC);
                OSDrawLine (first_x + 5, last_y, last_x, last_y, window->DC);
                //            OSDrawLine (first_x, first_y, first_x + (last_x - first_x)/2, first_y, window->DC);
                //            OSDrawLine (first_x + (last_x - first_x)/2, last_y, last_x, last_y, window->DC);
            }
            break;
        case 5 :
            if (mode == GSEDIRECTLINK)
            {
                first_x = (int)view_x(from_object->X,       window);
                first_y = (int)view_y(from_object->Y + f_h, window);
                last_x = (int)view_x(to_object->X,          window);
                last_y = (int)view_y(to_object->Y - t_h,    window);
                OSDrawLine (first_x, first_y, last_x, last_y, window->DC);
            }
            else
            {
                first_x = (int)view_x(from_object->X,       window);
                first_y = (int)view_y(from_object->Y + f_h, window);
                last_x = (int)view_x(to_object->X,          window);
                last_y = (int)view_y(to_object->Y - t_h,    window);
                OSDrawLine (first_x, first_y,  first_x, first_y + (last_y - first_y)/2, window->DC);
                OSDrawLine (last_x , first_y + (last_y - first_y)/2, last_x, last_y, window->DC);
            }
            break;
        case 0 :
            first_x = (int)view_x(from_object->X,       window);
            first_y = (int)view_y(from_object->Y,       window);
            last_x = (int)view_x(to_object->X,          window);
            last_y = (int)view_y(to_object->Y,          window);
            OSDrawLine (first_x, first_y, last_x, last_y, window->DC);
            break;
        }
}




void  GSEDisplayTreeLinks (GSEObject* obj, GSERect* win, int mode)
{

    if (!obj->Mark)
    {
        List* inher;
        int i = 0;
        GSEObject* first_object = NULL;
        GSEObject* last_object  = NULL;
        int first_x,first_y,last_x,last_y;

        obj-> Mark = 1;

        for (inher = obj->Inherit; inher != NULL; inher=inher->cdr)
        {
            GSELink* l = (GSELink *)inher->car;
            GSEObject* to_object = l->ToObject;
            if (i == 0)
            {
                first_object = to_object;
            }
            last_object = to_object;
            i++;
			if (obj->Close == 0)
			{
				to_object->Visible = 1;
				GSEDisplayLink (l, win, mode);
				GSEDisplayTreeLinks (to_object, win, mode);
			}
			to_object->Visible = 0;
        }
        if (mode == GSENETLINK)
        {
            if (first_object != last_object && obj->Close == 0)
                switch (win->Mode)
                {
                case 3 :
                    {
                        first_x = (int)view_x(obj->X + obj->Width ,     win);
                        first_y = (int)view_y(first_object->Y, win);
                        last_x = (int)view_x(last_object->X - last_object->Width,  win);
                        last_y = (int)view_y(last_object->Y,win);
                        //                OSDrawLine (first_x + (last_x - first_x)/2 , first_y, first_x + (last_x - first_x)/2, last_y , win->DC);
                        OSDrawLine (first_x + 5 , first_y, first_x + 5, last_y , win->DC);
                    }
                    break;
                case 5 :
                    {
                        first_x = (int)view_x(first_object->X ,     win);
                        first_y = (int)view_y(obj->Y + obj->Height, win);
                        last_x = (int)view_x(last_object->X,        win);
                        last_y = (int)view_y(last_object->Y - last_object->Height,win);
                        OSDrawLine (first_x , first_y + (last_y - first_y)/2, last_x, first_y + (last_y - first_y)/2, win->DC);
                    }
                    break;
                }

        }
    }
}


void  GSEDisplayTreeObjects (GSEObject* obj, GSERect* win)
{
    if (!obj->Mark)
    {
        List* inher;
        obj->Mark = 1;
        GSEDisplayObject(obj, win);

        for (inher = obj->Inherit; inher != NULL; inher=inher->cdr)
        {
			GSELink* l = (GSELink *)inher->car;
			GSEObject* to_object = l->ToObject;
			if (obj->Close == 0 && obj->Visible == 1)
			{
				to_object->Visible = 1;
			}
			else
				to_object->Visible = 0;
			GSEDisplayTreeObjects (to_object, win);
            
		}
    }
}

void  GSERemoveTreeNode (GSE* pgse, char* application_name, GSEObject* obj)
{
	if (!obj) return;
    while (obj->Inherit)
    {
        GSELink* l = (GSELink *)obj->Inherit->car;
        GSEObject* to_object = l->ToObject;
        GSERemoveTreeNode (pgse, application_name, to_object);
    }
    GSERemoveNode(pgse, application_name, obj); 
}


void  GSEDisplayAllLinks (GSE* pgse, char* appli_name, GSERect* win, int mode)
{
    GSEApplication* appli;
    List *Objects,*l1;
    GSEObject *from_object = win->Root;
    appli = GSEFindApplication (pgse, appli_name);
    if (!appli) return;

    Objects = appli->Objects;

    l1 = appli->Links;
    switch (win->Mode) {
    case 3 :
    case 5 :
        if (from_object)
        {
            GSEDisplayTreeLinks (from_object, win, mode);
            while (Objects != NULL)
            {
                ((GSEObject *)Objects->car)->Mark =0;
                Objects = Objects->cdr;
            }
        }
        break;
    case 0 :
        while (l1 != NULL)
        {
            GSELink* link = (GSELink *)l1->car;
            if ((ListMember (link->LinkType, win->LinkTypes)) &&
                    (ListMember (link->FromObject->Space ,win->Spaces)) &&
                    (ListMember (link->ToObject->Space   ,win->Spaces)))
            {
                GSEDisplayLink(link, win, GSEDIRECTLINK);
                l1 = l1->cdr;
            }
        }
        break;
    }
}


void  GSEDisplayAllObjects (GSE* pgse, char* appli_name, GSERect* win)
{

    GSEApplication* appli;
    List *Objects;
    GSEObject *from_object = win->Root;

    appli = GSEFindApplication (pgse, appli_name);
    if (!appli) return;


    Objects = appli->Objects;
    switch (win->Mode) {
    case 3 :
    case 5 :
        if (from_object)
        {
            GSEDisplayTreeObjects (from_object, win);
            while (Objects)
            {
                ((GSEObject *)Objects->car)->Mark =0;
                Objects = Objects->cdr;
            }
        }
        break;
    case 0 :
        while (Objects)
        {
            GSEDisplayObject((GSEObject*)Objects->car, win);
            Objects = Objects->cdr;
        }
        break;
    }
}






void  GSEResizeObjects (GSE* pgse, char* appli_name, LONG dc)
{

    GSEApplication* appli;
    OSRect Rect;

    int incrx = 0;//2;
    int incry = 0;//4;
    List* Objects;
    GSEObject* obj;

    appli = GSEFindApplication (pgse, appli_name);
    if (!appli) return;


    Objects = appli->Objects;
    while (Objects)
    {
        obj = (GSEObject *)Objects->car;
        if (!obj->Resource)
        {
            incrx = 0; //5;
            incry = 0; //10;
        }
        else
        {
            incrx = 0;
            incry = 0;
        }
        obj = (GSEObject*)Objects->car;
        obj->Space->ResizeProcedure (obj, &Rect, dc);
        obj->Width= (Rect.Right + incrx) / 2;
        obj->Height = (Rect.Bottom+ incry ) /2;
        Objects = Objects->cdr;
    }
}




GSEObject* GSEGetLeftObject (GSE* pgse, GSEObject* Obj, char* appli_name, GSERect* window)
{

    List* Objects;
    List* Links;
    GSEObject* MyObj = NULL;
    GSEObject* obj;
    GSEApplication* appli;


    if (window->Mode == 5)    /* x is fix */
    {
        appli = GSEFindApplication (pgse, appli_name);
        if (!appli) return NULL;
        Objects = appli->Objects;
        while (Objects != NULL)
        {
            obj = (GSEObject*)Objects->car;
            if ((obj->Visible ) && (obj != Obj)  && (obj->Y == Obj->Y))
            {
                if (obj->X < Obj->X)
                    if (MyObj && (obj->X > MyObj->X)) MyObj = obj;
                    else  if (!MyObj) MyObj = obj;
            }
            Objects = Objects->cdr;
        }
    }
    if (window->Mode == 3)
    {
        Links = Obj->Fromherit;
        while (Links)
        {
            obj = ((GSELink*)Links->car)->FromObject;
            if (obj->Visible) return obj;
            Links = Links->cdr;
        }
    }
    return MyObj;

}




GSEObject* GSEGetUpObject (GSE* pgse, GSEObject* Obj, char* appli_name, GSERect* window)
{

    List* Objects;
    List* Links;
    GSEObject* MyObj = NULL;
    GSEObject* obj;
    GSEApplication* appli;

    if (window->Mode == 3)    /* x is fix */
    {
        appli = GSEFindApplication (pgse, appli_name);
        if (!appli) return NULL;
        Objects = appli->Objects;
        while (Objects != NULL)
        {
            obj = (GSEObject*)Objects->car;
            if ((obj->Visible ) && (obj != Obj)  && (obj->X == Obj->X))
            {
                if (obj->Y < Obj->Y)
                    if (MyObj && (obj->Y > MyObj->Y)) MyObj = obj;
                    else  if (!MyObj) MyObj = obj;
            }
            Objects = Objects->cdr;
        }
    }
    if (window->Mode == 5)
    {
        Links = Obj->Fromherit;
        while (Links)
        {
            obj = ((GSELink*)Links->car)->FromObject;
            if (obj->Visible) return obj;
            Links = Links->cdr;
        }

    }
    return MyObj;

}




GSEObject* GSEGetDownObject (GSE* pgse, GSEObject* Obj, char* appli_name, GSERect* window)
{

    List* Objects;
    List* Links;
    GSEObject* MyObj = NULL;
    GSEObject* obj;
    GSEApplication* appli;

    if (window-> Mode == 3)    /* x is fix */
    {
        appli = GSEFindApplication (pgse, appli_name);
        if (!appli) return NULL;
        Objects = appli->Objects;
        while (Objects != NULL)
        {
            obj = (GSEObject*)Objects->car;
            if ((obj->Visible ) && (obj != Obj)  && (obj->X == Obj->X))
            {
                if (obj->Y > Obj->Y)
                    if (MyObj && (obj->Y < MyObj->Y)) MyObj = obj;
                    else if (!MyObj) MyObj = obj;
            }
            Objects = Objects->cdr;
        }
    }
    if (window->Mode == 5)
    {
        Links = Obj->Inherit;
        while (Links)
        {
            obj = ((GSELink*)Links->car)->ToObject;
            if (obj->Visible) return obj;
            Links = Links->cdr;
        }
    }
    return MyObj;

}

GSELink* GSEGetLastLink (GSEObject* Obj)
{
    List* Links;
	GSELink* Link = NULL;
	if (!Obj) return NULL;
	Links = Obj->Inherit;

	while (Links)
    {
        Link = (GSELink*)Links->car;
        Links = Links->cdr;
    }
    return Link;
}

GSELink* GSEGetFirstLink (GSEObject* Obj)
{
	List* Links;
	if (!Obj) return NULL;
	Links = Obj->Inherit;

	if (Links)
        return (GSELink*)Links->car;
    return NULL;

}

GSEObject* GSEGetLastChildObject (GSEObject* Obj)
{
    List* Links;
	GSEObject* obj = NULL;
	if (!Obj) return NULL;
	Links = Obj->Inherit;

	while (Links)
    {
        obj = ((GSELink*)Links->car)->ToObject;
        Links = Links->cdr;
    }
    return obj;
}

GSEObject* GSEGetFirstChildObject (GSEObject* Obj)
{
	List* Links;
	if (!Obj) return NULL;
	Links = Obj->Inherit;

	if (Links)
        return ((GSELink*)Links->car)->ToObject;
    return NULL;
}

GSEObject* GSEGetRightObject (GSE* pgse, GSEObject* Obj, char* appli_name, GSERect* window)
{

    List* Objects;
    List* Links;
    GSEObject* MyObj = NULL;
    GSEObject* obj;
    GSEApplication* appli;

    if (window->Mode == 5)    /* x is fix */
    {
        appli = GSEFindApplication (pgse, appli_name);
        if (!appli) return NULL;
        Objects = appli->Objects;
        while (Objects != NULL)
        {
            obj = (GSEObject*)Objects->car;
            if ((obj->Visible ) && (obj != Obj)  && (obj->Y == Obj->Y))
            {
                if (obj->X > Obj->X)
                    if (MyObj && (obj->X < MyObj->X)) MyObj = obj;
                    else if (!MyObj) MyObj = obj;
            }
            Objects = Objects->cdr;
        }
    }
    if (window->Mode == 3)
    {
        Links = Obj->Inherit;
        while (Links)
        {
            obj = ((GSELink*)Links->car)->ToObject;
            if (obj->Visible) return obj;
            Links = Links->cdr;
        }
    }
    return MyObj;

}


int GSEObjectInArea (GSEObject* obj, GSERect* window, int lx, int ty, int rx, int by)
{
    double x,y,width,height;
    int lx1,ty1,rx1,by1;
    if (!obj->Visible)  return 0;
    x = obj->X ;
    y = obj->Y;
    width = obj->Width;
    height = obj->Height;
    lx1 = (int)view_x(x-width,window);
    ty1 = (int)view_y(y-height,window);
    rx1 = (int)view_x(x+width,window);
    by1 = (int)view_y(y+height,window);
    if (in_rectangle (lx1, ty1, lx, by, rx, ty) &&
            in_rectangle (rx1, by1, lx, by, rx, ty) )
        return 1;
    else  return 0;
}


GSEObject* GSEFindObject (GSE* pgse, char* appli_name, GSERect* window, int px, int py)
{
    GSEApplication* appli;
    double x,y,width,height;
    LONG lx,ty,rx,by;
    GSEObject* obj;
    List* Objects;
    GSEObject* FoundObject = NULL;

    appli = GSEFindApplication (pgse, appli_name);
    if (!appli) return NULL;
    Objects = appli->Objects;

    while (Objects)
    {
        obj = (GSEObject*)Objects->car;
        x = obj->X;
        y = obj->Y;
        width = obj->Width; height = obj->Height;
        lx = (LONG)view_x(x-width,window);
        ty = (LONG)view_y(y-height,window);
        rx = (LONG)view_x(x+width,window);
        by = (LONG)view_y(y+height,window);
        if (in_rectangle (px, py, lx, by, rx, ty))
            if (obj->Visible)  FoundObject = obj;
        Objects = Objects->cdr;
    }

    return FoundObject;
}


void  GSEClearWindow (GSERect* win)
{
    OSRect rect;
    OSGetWindowSize (win->Window,  &rect);
  //  OSFillRectangle (&rect, win->DC, 0);
}

void  GSERefresh (GSE* pgse, char* appli_name, GSERect* win, double from_x, double to_x, int mode)
{
    if (!win->Visible )return;

    if (!win->Root) return;
    if (win->Mode == 3)    GTTDHG (win->Root, from_x, to_x, 10., 10.);
    if (win->Mode == 5)    GTTDG (win->Root, from_x, to_x, 10., 10.);

    GSEClearWindow (win);
    GSEDisplayAllLinks (pgse, appli_name, win, mode);
    GSEDisplayAllObjects (pgse, appli_name, win);
}







void find_fields_h (GSEObject* object, double* width_level,  double* modifier,
                    double* next_pos,  double* y_final, double* place,   int* act_width,
                    int* jp, double* y_first, double dist_y, int* son_not_marked)

{
    if (object->Mark == 0)
    {
        object->Mark = 1;
        *son_not_marked=0;
        ++*jp;

        if (width_level[*act_width] == 0.0)
            width_level[*act_width] = object->Width * 2;
        else
            width_level[*act_width] = max (width_level[*act_width], (object->Width * 2));

        if ((object->Inherit != NULL) && (object->Close == 0))
        {
            List *inher;
            double y_first;
            int j = -1 , son_not_marked = 1;
            ++*act_width;

            for(inher = object->Inherit; inher != NULL;  inher = inher->cdr)
                find_fields_h (((GSELink *)(inher->car))->ToObject, width_level,
                               modifier, next_pos, y_final, place, act_width, &j,
                               &y_first, dist_y, &son_not_marked);
            --*act_width;
            if (son_not_marked)
                *place = object->Height + next_pos[*act_width];
            else
            {
                *place = ((y_first + *y_final) / 2);
                modifier[*act_width]= max(modifier[*act_width],
                                          ((object->Height + next_pos[*act_width]) - *place));
                *place = *place + modifier[*act_width];
            }
        }   /* end of if */

        if ((object->Close == 1) || (object->Inherit == NULL))
            *place = object->Height + next_pos[*act_width];
        object->X = 0.0;
        object->Y = *place;
        object->Modif = modifier[*act_width];
        if (*jp==0) *y_first= *place;
        *y_final= *place;
        next_pos[*act_width]= *place + dist_y + object->Height;
    }
}


void modif_node1_h (GSEObject* object, double* modifier_sum, double delta_x,
                    double delta_y, double* total_width,int* act_width, double* vector_width,
                    double dist_x)

{
    if (object->Mark != 0)
    {
        object->Mark= 0;
        object->Y = object->Y + *modifier_sum + delta_y;
        object->X = *total_width + (vector_width [*act_width] / 2) + delta_x;
        if ((object->Inherit != NULL) && (object->Close == 0))
        {
            List *inher;
            *total_width = *total_width + dist_x + vector_width[*act_width];
            ++*act_width;
            if (object->Modif)
                *modifier_sum = *modifier_sum + object->Modif;
            for (inher = object->Inherit; inher != NULL; inher = inher->cdr)
                modif_node1_h (((GSELink *)(inher->car))->ToObject , modifier_sum,
                               delta_x, delta_y, total_width, act_width, vector_width, dist_x);
            --*act_width;
            *total_width= *total_width - (dist_x + vector_width[*act_width]);
            if (object->Modif)
            {
                *modifier_sum = *modifier_sum - object->Modif;
                object->Modif = 0.;
            }
        }
    }
}


void gmnhg (GSEObject* object, double* vector_width, double root_x, double root_y,double dist_x)
{
    double modifier_sum = 0.,
                          total_width = dist_x,
                                        delta_x,delta_y;
    int act_width = 1;

    if (root_x)
        delta_x = root_x - (dist_x + (vector_width [act_width] / 2));
    else  delta_x = 0.;
    if (root_y)    delta_y = root_y - object->Y;
    else  delta_y = 0.;
    modif_node1_h (object,&modifier_sum,delta_x,delta_y,&total_width,
                   &act_width,vector_width,dist_x);
}





void find_fields_v (GSEObject* object, double* height_level, double* modifier,
                    double* next_pos, double* x_final, double* place, int* act_height, int* jp,
                    double* x_first, double dist_x, int* son_not_marked)

{
    if (object->Mark == 0)
    {
        object->Mark = 1;
        *son_not_marked= 0; ++*jp;

        if (height_level[*act_height] == 0.0)
            height_level[*act_height] = object->Height * 2;
        else
            height_level[*act_height] = max (height_level[*act_height],(object->Height * 2));

        if ((object->Inherit != NULL) && (object->Close == 0))
        {
            List *inher;
            double x_first;
            int j = -1, son_not_marked = 1;
            ++*act_height;

            for(inher = object->Inherit; inher != NULL; inher = inher->cdr)
                find_fields_v (((GSELink *)(inher->car))->ToObject,height_level,
                               modifier,next_pos, x_final,place,act_height,&j,
                               &x_first,dist_x, &son_not_marked);
            --*act_height;
            if (son_not_marked)
                *place = object->Width + next_pos[*act_height];
            else
            {
                *place = ((x_first + *x_final) / 2);
                modifier[*act_height]= max(modifier[*act_height],
                                           ((object->Width + next_pos[*act_height]) - *place));
                *place = *place + modifier[*act_height];
            }
        }   /* end of if */

        if ((object->Close == 1) || (object->Inherit == NULL))
            *place = object->Width + next_pos[*act_height];
        object->X = *place;
        object->Y = 0.;
        object->Modif = modifier[*act_height];
        if (*jp == 0) *x_first= *place;
        *x_final= *place;
        next_pos[*act_height]= *place + dist_x + object->Width;
    }
}

void modif_node1_v (GSEObject* object, double* modifier_sum, double delta_x,
                    double delta_y, double* total_height, int* act_height,
                    double* vector_height, double dist_y)

{
    if (object->Mark != 0)
    {
        object->Mark= 0;
        object->X = object->X + *modifier_sum + delta_x;
        object->Y = *total_height + (vector_height [*act_height] / 2) + delta_y;

        if ((object->Inherit != NULL) && (object->Close == 0))
        {
            List *inher;
            *total_height = *total_height + dist_y + vector_height[*act_height];
            ++*act_height;

            if (object->Modif)
                *modifier_sum = *modifier_sum + object->Modif;

            for (inher = object->Inherit; inher != NULL; inher = inher->cdr)
                modif_node1_v (((GSELink *)(inher->car))->ToObject ,modifier_sum,
                               delta_x,delta_y, total_height,act_height,
                               vector_height,dist_y);
            --*act_height;
            *total_height = *total_height - (dist_y + vector_height[*act_height]);
            if (object->Modif)
            {
                *modifier_sum = *modifier_sum - object->Modif;
                object->Modif = 0.;
            }
        }
    }
}


void gmng (GSEObject* object, double* vector_height, double root_x, double root_y,double dist_y)

{
    double modifier_sum = 0.,
                          total_height = dist_y,
                                         delta_x,delta_y;
    int act_height = 1;

    if (root_y)
        delta_y = root_y - (dist_y + (vector_height [act_height] / 2));
    else
        delta_y = 0.;
    if (root_x)
        delta_x = root_x - object->X;
    else
        delta_x = 0.;
    modif_node1_v (object, &modifier_sum, delta_x, delta_y, &total_height,
                   &act_height,vector_height,dist_y);
}



void  GTTDG (GSEObject* object,double root_x,double root_y,double dist_x,double dist_y)
{
    double x_final =0.,
                    x_first = 0.,
                              place,
                              modifier[MAX_SIZE],
                              next_pos[MAX_SIZE],
                              height_level[MAX_SIZE];
    int    act_height = 1,
                        j = -1,
                            son_not_marked = 1,
                                             i;

    for (i=0; i< MAX_SIZE; ++i)
    {
        next_pos[i] = dist_x;
        modifier[i] = 0.;
        height_level[i] = 0.;
    }
    find_fields_v (object, height_level, modifier, next_pos, &x_final, &place,
                   &act_height, &j, &x_first, dist_x, &son_not_marked);
    gmng (object,height_level,root_x,root_y,dist_y);
}





void  GTTDHG (GSEObject* object,double root_x,double root_y,double dist_x,double dist_y)
{
    double y_final =0.,
                    y_first= 0.,
                             place,
                             modifier[MAX_SIZE],
                             next_pos[MAX_SIZE],
                             width_level[MAX_SIZE];
    int    act_width = 1,
           j = -1,
           son_not_marked = 1,
           i;

    for (i=0; i< MAX_SIZE; ++i)
    {
        next_pos[i] = dist_y;
        modifier[i] = 0.;
        width_level[i] = 0.;
    }
    find_fields_h (object,width_level,modifier,next_pos,&y_final,&place,
                   &act_width,&j,&y_first,dist_y,&son_not_marked);
    gmnhg (object,width_level,root_x,root_y,dist_x);
}


void GSERemoveAll (GSE* pgse)
{
    GSEApplication* Application;
    List* Applications = pgse->Applications;
    while (Applications)
    {
        Application = (GSEApplication *)Applications->car;
        GSERemoveAllNodes (pgse, Application->Name);
        GSERemoveAllLinks (pgse, Application->Name);
        Applications = Applications->cdr;
    }
}


void GSEFreeApplication (GSE* pgse, GSEApplication* Application)
{
    GSESpace* Space;
    GSERect* Window;
    GSELinkType* LinkType;

    GSERemoveAllNodes (pgse, Application->Name);
    GSERemoveAllLinks (pgse, Application->Name);
    while (Application->Spaces)
    {
        Space = (GSESpace *)Application->Spaces->car;
        ListRemove (&Application->Spaces, Space);
        free (Space);
    }

    while (Application->LinkTypes)
    {
        LinkType = (GSELinkType *)Application->LinkTypes->car;
        ListRemove (&Application->LinkTypes, LinkType);
        free (LinkType);
    }

    while (Application->Windows)
    {
        Window = (GSERect *)Application->Windows->car;
        ListRemove (&Application->Windows, Window);
        free (Window);
    }

    ListRemove (&pgse->Applications, Application);
    free (Application);
}




void  GSEEnd (GSE* pgse)
{
    GSEApplication* Application;
    while (pgse->Applications)
    {
        Application = (GSEApplication *)pgse->Applications->car;
        GSEFreeApplication (pgse, Application);
    }
}
