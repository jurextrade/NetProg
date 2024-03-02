INC_REP = ../INC
LIB_REP = ../OBJ
CFLAGS =   -c
SRCREPER = ../SRC/


SRC=	  db.c       	  \
	  dbstatic.c 	  \
	  mx.c       	  \
        dg.c              \
	  osdate.c   	  \
	  osdb.c     	  \
	  osio.c     	  \
	  oslist.c   	  \
	  osras.c    	  \
	  osreslex.c 	  \
	  osresyac.c 	  \
	  sclex.c    	  \
	  scprocdb.c 	  \
	  scprocio.c 	  \
	  scprocmx.c 	  \
	  scprocop.c 	  \
	  scsys.c    	  \
	  scyacc.c   	  \
	  xf.c       	  \
	  u_xlat.c   	  \
	  scaction.c 	  \
	  ostrace.c  	  \
	  oscom.c    	  \
	  osmatch.c         \
	  tp.c       	  \
	  osdbsta.c  	  \
	  sc.c       	  \
	  scparse.c  	  \
	  scproc.c 	        \
	  mail.c 	        \
	  ftp.c 	        \
        tcp.c             \
 	  mime.c

OBJ = $(SRC:.c=.o)

$(OBJ) : $(SRC)

.c.o : 
	$(CC) $(CFLAGS)  -DUNIX -DHP_UX -I$(INC_REP) $< 

lib  :	$(OBJ)
	ar -rv libNetProg.a $(OBJ) 
	mv libNetProg.a ../OBJ

clean :
	rm -f ../OBJ/$(OBJ) ;

server : $(OBJ) server.c 
	make lib
	cc  server.c  -I$(INC_REP) -L$(LIB_REP) -lNetProg -o server

scadmin : $(OBJ) scadmin.c 
	make lib
	cc  scadmin.c  -I$(INC_REP) -L$(LIB_REP) -lNetProg -o scadmin
