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

#ifndef __GSE_H__
#define __GSE_H__

#include "oslist.h"
#include "osdraw.h"


#define GSEDIRECTLINK   0
#define GSENETLINK      1


#define BORDERWIDTH		2
#define MAX_SIZE		   60

#define GSEFROMSPACE 0
#define GSETOSPACE   1


#define view_x(world_x,win)	(((world_x) - (win)->Scroll_x)*(win)->Ratio_x)
#define view_y(world_y,win)	(((world_y) - (win)->Scroll_y)*(win)->Ratio_y)
#define view_r(world_r,win)	((world_r) * (win)->ratio_r)
#define world_x(view_x,win)	(((view_x)/(win)->Ratio_x)+(win)->Scroll_x)
#define world_y(view_y,win)	(((view_y)/(win)->Ratio_y)+(win)->Scroll_y)
#define world_r(view_r,win)	((view_r) / (win)->Ratio_r)       

#define in_seg(x,lx,rx)			((x >= lx) && (x <= rx))
#define in_rectangle(x,y,lx,by,rx,ty)	((in_seg(x,lx,rx))&&(in_seg(y,ty,by))) 
#define inter_seg(lx,rx,lx1,rx1)	((in_seg(lx,lx1,rx1)||(in_seg(lx1,lx,rx))))
#define inter_rect(lx,ty,rx,by,lx1,ty1,rx1,by1)	((inter_seg(lx,rx,lx1,rx1)) \
                                               &&(inter_seg (ty,by,ty1,by1)))  




#define GSERECTANGLE   0
#define GSEELLIPSE     1

#define GSEBLACKBRUSH  BLACK_BRUSH
#define GSENAMESIZE    120


#define GSESetDrawingProcedure(pobject, proc) (pobject)->DrawingProcedure = proc;
#define GSESetResizeProcedure(pobject, proc) (pobject)->ResizeProcedure = proc;

#define GSESetSpaceDrawingProcedure(space, proc) (space)->DrawingProcedure = proc;
#define GSESetSpaceResizeProcedure(space, proc) (space)->ResizeProcedure = proc;





/* La partie semantic des actions graphiques sont definies dans un fichier */

typedef int (*GSEFunction) ();

typedef struct _GSESpaceSemantic
{
    GSEFunction  CreateObject;
    GSEFunction  KillObject;
    GSEFunction  KillTree;
    GSEFunction  MoveObject;
    GSEFunction  InsertObject;
    GSEFunction  OpenObject;
    GSEFunction  CloseObject;
} GSESpaceSemantic;

typedef struct _GSELinktypeSemantic
{
    GSEFunction  CreateLink;
    GSEFunction  MoveLink;
    GSEFunction  KillLink;
    GSEFunction  InsertTree;
}GSELinktypeSemantic;



typedef struct _GSEApplication
{
    char      Name[GSENAMESIZE];
    List*     Objects;
    List*     Links;
    List*     LinkTypes;
    List*     Spaces;
    List*     Windows;
}GSEApplication;



typedef struct _GSELinktype
{
    char      Name[GSENAMESIZE];
    List*     FromSpaces;
    List*     ToSpaces;
    int       Style;
    GSELinktypeSemantic Semantic;
}GSELinkType;


typedef struct _GSELabel
{
    char*     Name;
    int       Width;
    int       Height;
    int       Format;
    int       Font;
}GSELabel;


typedef struct _GSEObject
{
    int       Code ;
    WORD      Resource;
    WORD      Type;
    double    X ;
    double    Y ;
    double    Width ;
    double    Height ;
    DWORD     Pattern ;
    int       Style;
    List*     Name;
    char*     Eval;
    int       Close ;
    int       Mark;
    double    Modif ;
    List*     Fromherit;
    List*     Inherit;
    void*     Association;
    int       Visible;
    void*      UserField;
    void       (*DrawingProcedure) (struct _GSERect* window, struct _GSEObject* pobject, WORD Resource);
    void       (*ResizeProcedure) (struct _GSEObject* pobject, OSRect* rect, LONG dc);
    struct  _GSESpace* Space;
}GSEObject;


typedef struct _GSESpace
{
    char      Name[GSENAMESIZE];
    int       Font;
    DWORD     Pattern;
    WORD      Resource;
    int       Code;
    int       (*RemoveProcedure) (GSEObject* pobject);
    void      (*DrawingProcedure) (struct _GSERect* window, GSEObject* pobject, WORD Resource);
    void      (*ResizeProcedure) (GSEObject* pobject, OSRect* rect, LONG dc);
    GSESpaceSemantic Semantic;
}GSESpace;



typedef struct _GSELink
{
    GSELinkType* LinkType;
    GSEObject*   FromObject;
    GSEObject*   ToObject;
    List*        PosXBet;
    List*        PosYBet;
    int          Style;
}GSELink;


typedef struct _GSERect
{
    HWND          Window;
    LONG          DC;
    GSEObject*    Root;
    List*         Spaces;
    List*         LinkTypes;
    List*         LinkSelection;
    List*         ObjSelection;
    double        Ratio_x;
    double        Ratio_y;
    double        Ratio_r;
    double        Scroll_x;
    double        Scroll_y;
    double        Room_lx;
    double        Room_rx;
    double        Room_by;
    double        Room_ty;
    int           Viewport;
    int           Visible;
    int           Mode;
} GSERect;



typedef struct _GSE
{

    List*         Applications;

}GSE;


#ifdef __cplusplus
extern "C" {
#endif 

    extern void            GSEInit(GSE* pgse);

    extern GSEApplication* GSECreateApplication (GSE* pgse, char* application_name);
    extern GSESpace*       GSECreateSpace (GSE* pgse, char* application_name, char* space_name);
    extern GSELinkType*    GSECreateLinkType (GSE* pgse, char* application_name, char* linktype_name);
    extern void            GSEDomainLinkType (GSE* pgse, char* application_name, char* linktype_name, BYTE mode, char* space_name);


    extern GSESpace*       GSEAssociateSpace (GSE* pgse, char* appli_name, char* space_name, GSERect* window);
    extern GSESpace*       GSEDissociateSpace (GSE* pgse, char* appli_name, char* linktype_name, GSERect* window);
    extern GSELinkType*    GSEAssociateLinkType (GSE* pgse, char* appli_name, char* linktype_name, GSERect* window);
    extern GSELinkType*    GSEDissociateLinkType (GSE* pgse, char* appli_name, char* linktype_name, GSERect* window);

    extern void            GSEPutpatternSpace (GSE* pgse, char* application_name, char* space_name, DWORD pattern);
    extern void            GSEPutFontSpace (GSE* pgse, char* application_name, char* space_name, int Font);
    extern void            GSEPutCodeSpace (GSE* pgse, char* application_name, char* space_name, int Code);
    extern void            GSEPutResourceSpace (GSE* pgse, char* application_name, char* space_name, WORD Resource);

    extern GSESpace*       GSEFindSpace (GSE* pgse, GSEApplication* appli, char* space_name);
    extern GSEApplication* GSEFindApplication (GSE* pgse, char* appli_name);


    extern GSEObject*      GSEMakeNode (GSE* pgse, char* application_name, char* space_name, int code, char* label);
    extern GSELink*        GSEMakeLink (GSE* pgse, char* application_name, char* lintype_name, GSEObject* node1, GSEObject* node2);
	extern GSELink*        GSEMakeLinkAfter (GSE* pgse, char* application_name, char* lintype_name, GSEObject* node1, GSEObject* node2, GSELink* link);
    extern void            GSERemoveLink(GSE* pgse, char* application_name, GSELink* link);
    extern void            GSERemoveNode (GSE* pgse, char* application_name,GSEObject* object);
    extern void            GSERemoveAllNodes (GSE* pgse, char* application_name);
    extern void            GSERemoveAllLinks (GSE* pgse, char* application_name);
    extern void            GSEClearApplication (GSE* pgse, char* application_name);

    extern void            GTTDHG (GSEObject* obj, double x, double y, double dit_x, double dist_y);
    extern void            GTTDG (GSEObject* object,double root_x,double root_y,double dist_x,double dist_y);

    extern void            GSERectTotalRoom (GSE* pgse, char* aplli_name, GSERect* win);
    extern void            GSEResizeObjects (GSE* pgse, char* appli_name, LONG dc);

    extern GSEObject*      GSEFindObject (GSE* pgse, char* appli_name, GSERect* window, int px, int py);
    extern int             GSELinkExist (GSEObject* from_object, GSEObject* to_object, GSELinkType* linktype);

    extern int             GSEObjectInArea (GSEObject* obj, GSERect* window, int lx, int ty, int rx, int by);
    extern void            GSEMarkOnOffObject(GSEObject* obj, GSERect* win);
    extern void            GSEInvertObject(GSEObject* obj, GSERect* win);

    extern GSEObject*      GSEGetLeftObject (GSE* pgse, GSEObject* Obj, char* appli_name, GSERect* window);
    extern GSEObject*      GSEGetRightObject (GSE* pgse, GSEObject* Obj, char* appli_name,GSERect* window);
    extern GSEObject*      GSEGetUpObject (GSE* pgse, GSEObject* Obj, char* appli_name, GSERect* window);
    extern GSEObject*      GSEGetDownObject (GSE* pgse, GSEObject* Obj,char* appli_name, GSERect* window);
    extern GSEObject*      GSEGetLastChildObject (GSEObject* RootObj);
    extern GSEObject*      GSEGetFirstChildObject (GSEObject* RootObj);

    extern GSELink*			GSEGetLastLink (GSEObject* RootObj);
    extern GSELink*			GSEGetFirstLink (GSEObject* RootObj);


    extern GSERect*        GSEMakeWindow (HWND win, LONG dc);
    extern void            GSESetRootWindow (GSE* pgse, GSEObject* root, GSERect* win);
    extern void            GSESetModeWindow (GSE* pgse, GSERect* win, int mode);

    extern GSELabel*       GSEAllocLabel (char* s, int Font);
    extern GSELink*        GSEAllocLink (GSEObject* f_object, GSEObject* t_object);
	extern GSELink*		   GSEAllocLinkAfter (GSEObject* f_object, GSEObject* t_object, GSELink* a_link);

    extern void            GSEDisplayAllObjects (GSE* pgse, char* appli_name, GSERect* win);
    extern void            GSEDisplayAllLinks (GSE* pgse, char* appli_name, GSERect* win, int mode);
    extern void            GSEDisplayLabel (List* l_label, GSERect* window, int Font, OSRect* rect, char* string);

    extern void            GSERefresh (GSE* pgse, char* appli_name, GSERect* win, double from_x, double to_x, int mode);
    extern void            GSEDisplayObject (GSEObject* obj, GSERect* window);

    extern void            GSEDefaultResizeProcedure  (GSEObject* obj, OSRect* rect, LONG dc);
    extern void            GSEDefaultDrawingProcedure (GSERect* window, GSEObject* obj, WORD Resource);
    extern void            GSESetVisibleSpace (GSE* pgse, char* application_name, char* space_name, int visible);
    extern void            GSEPutLinkAtBeginning (GSE* pgse, GSELink* l);
    extern void            GSEClearWindow (GSERect* win);
    extern void            GSEFreeApplication (GSE* pgse, GSEApplication* Application);
    extern void            GSERemoveAll (GSE* pgse);
    extern void            GSEEnd (GSE* pgse);
    extern int             GSESetSpaceRemoveProcedure (GSE* pgse, char* application_name, char* space_name, int (*funct) (GSEObject* pobj));
    extern void            GSERemoveTreeNode (GSE* pgse, char* application_name, GSEObject* obj);
	extern void			   GSESetName (GSE* pgse, char* application_name, GSEObject* object, char* label);
#ifdef __cplusplus
}
#endif

#endif /* __GSE_H__*/
