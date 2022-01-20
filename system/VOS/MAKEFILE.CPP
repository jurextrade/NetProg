# Makefile : export
# AUTEUR   : Jerome JEAN-CHARLES 
# FONCTION : TE (Traitement Export)
# MODIF    : Creation le 06/03/96 
# 
# REMARQUES:
# 
#

.INCLUDE >PRJ>MAK>stdmin.mak
.INCLUDE >PRJ>MAK>acces.mak

# PATCH pour warning OFF  par jerome
CODE_WARN_OFF=  2002 2521 2081 2091 2006
# NE PAS TOUCHER FULL_PROTO : CODE PATCH AUSSI 
FULL_PROTO =  -suppress_diag 2096 2010 $(CODE_WARN_OFF) -system_programming

# Definition des versions de librairie a utiliser
V_FIC     = V3.0
V_INC4    = V2.7
VER_INC_GEN=V1.3

# DEF_FILES=$(DEF_FILES) $(INC_GEN)>vos_call.h osq.pto tpf.pto
DEF_FILES=$(DEF_FILES) 
#TYPE_CHECK = -type_checking maximum

VER_TCP=V1.0 

OBJ_SEARCHES= $(OBJ_SEARCHES) $(OBJ_TCP)
INC_SEARCHES= $(INC_SEARCHES) $(INC_TCP)

BIND_SEARCHES=$(BIND_SEARCHES) $(OBJ_OSQ) $(OBJ_TPF) $(OBJ_TCP) # $(OBJ_SEQ)


BIDON :
     display_line  def_file= $(DEF_FILES)
     display_line  bind= $(BIND_SEARCHES)
     display_line  obj = $(OBJ_SEARCHES) 
     display_line  DIAG = $(SUPPRESS_DIAG)
                          

ALL: scadmin.pm 
   display_line ALL DONE !!!

scadmin.pm : scadmin.obj                                                    \
              db.obj dbstatic.obj mx.obj oscom.obj osdb.obj osdate.obj      \
              osdbsta.obj osio.obj oslist.obj osmatch.obj osras.obj         \
              osreslex.obj osresyac.obj ostrace.obj sc.obj scaction.obj     \
              sclex.obj scparse.obj scproc.obj scprocdb.obj scprocio.obj    \
              scprocmx.obj scprocop.obj scsys.obj scyacc.obj                \
              tp.obj u_xlat.obj xf.obj mime.obj mail.obj ftp.obj
    $(_BIND_BASE_)

# listes des dependances (V=1.52) 

#***************liste des dependences .c.obj
scadmin.obj             : scadmin.c
xf.obj                  : xf.c
u_xlat.obj              : u_xlat.c
tp.obj                  : tp.c
scsys.obj               : scsys.c
scyacc.obj              : scyacc.c
scprocop.obj            : scprocop.c
scprocio.obj            : scprocio.c
scprocmx.obj            : scprocmx.c
scproc.obj              : scproc.c
scprocdb.obj            : scprocdb.c
scparse.obj             : scparse.c
sclex.obj               : sclex.c
sc.obj                  : sc.c
scaction.obj            : scaction.c
ostrace.obj             : ostrace.c
osresyac.obj            : osresyac.c
osreslex.obj            : osreslex.c
osras.obj               : osras.c
osmatch.obj             : osmatch.c
oslist.obj              : oslist.c
osio.obj                : osio.c
osdbsta.obj             : osdbsta.c
osdb.obj                : osdb.c
oscom.obj               : oscom.c
osdate.obj              : osdate.c
mx.obj                  : mx.c
db.obj                  : db.c
dbstatic.obj            : dbstatic.c
ftp.obj			: ftp.c
mail.obj			: mail.c
mime.obj			: mime.c

# eof Makefile 





