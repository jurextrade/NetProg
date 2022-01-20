INC_REP = ../../include


OBJ=  cgi.o database.o dg.o dns.o dnscomp.o fiop.o ftp.o gse.o http.o mail.o mime.o mt.o mx.o mxmanager.o mxutil.o oscom.o osdate.o osdb.o osdraw.o osio.o oslist.o osmatch.o osras.o  osresyac.o ostrace.o pop.o sc.o scaction.o \
	  scparse.o scproc.o scprocio.o scprocmx.o  scsys.o scyacc.o smtp.o  tcp.o tp.o u_xlat.o xf.o scprocop.o osreslex.o sqlodbc.o lex.yy.o
EXEC= NetProg.lib


%.o: ../../src/%.c
	gcc -o  $@ -c -DYY_USE_PROTOS -DWIN32 -I$(INC_REP) $< 

lib: $(OBJ)
	ar rcs libNetProg.a libws2_32.a 
	ar rcs libNetProg.a libgdi32.a libodbc32.a
	ar rcs libNetProg.a $(OBJ)  

sc32: sc32.c
	cc  sc32.c -DYY_USE_PROTOS -DWIN32 -I$(INC_REP)  -L. -lNetProg -lws2_32 -lgdi32 -lodbc32 -o sc32.exe
