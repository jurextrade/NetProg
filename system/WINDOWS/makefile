COMP64_REP = D:/Programs/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/x86_64-w64-mingw32/lib
COMP32_REP = D:\PROJECTS_CROSS\MT4Progress\WebContent\MQL4\Server\Compile\C\MinGW\lib

INC_REP = ../../include


OBJ=  cb2a.o cgi.o database.o dg.o dns.o dnscomp.o fiop.o ftp.o gse.o http.o mail.o mime.o mt.o mx.o mxmanager.o mxutil.o oscom.o osdate.o osdb.o osdraw.o osio.o oslist.o osmatch.o osras.o osreslex.o osresyac.o \
	  ostrace.o pop.o sc.o scaction.o sclex.o scparse.o scproc.o scprocio.o scprocmx.o scprocop.o scsys.o scyacc.o smtp.o sqlodbc.o sqlora.o tcp.o tp.o u_xlat.o xf.o 
EXEC= libNetProg.a


%.o: ../../src/%.c
	gcc -c $< -DYY_USE_PROTOS -DWINDOWS -I$(INC_REP)  -o  $@

lib: $(OBJ)
	ar crf libNetProg.a  $(COMP64_REP)/libodbc32.a
	ar crf libNetProg.a  $(COMP64_REP)/libws2_32.a 
	ar crf libNetProg.a  $(OBJ)  
		
io_proto_store.o: ../../examples/IO_PROTO_STORE/io_proto_store.c
	gcc -c  ../../examples/IO_PROTO_STORE/io_proto_store.c -DYY_USE_PROTOS -DWINDOWS -I$(INC_REP)  

store: io_proto_store.o $(OBJ)
	gcc -o store io_proto_store.o -L. -lNetProg -lws2_32  -lodbc32


clean:
	del $(OBJ) $(EXEC) io_proto_store.o




