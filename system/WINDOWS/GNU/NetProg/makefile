COMP64_REP = C:\Programs\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\lib
COMP32_REP = C:/Programs/MinGW/lib

INC_REP = ../../../../include

OBJ32=  ../x86/obj/cb2a.o ../x86/obj/cgi.o ../x86/obj/database.o ../x86/obj/dg.o ../x86/obj/dns.o ../x86/obj/dnscomp.o ../x86/obj/fiop.o ../x86/obj/ftp.o ../x86/obj/http.o ../x86/obj/mail.o ../x86/obj/mime.o ../x86/obj/mt.o ../x86/obj/mx.o ../x86/obj/mxmanager.o ../x86/obj/mxutil.o ../x86/obj/oscom.o ../x86/obj/osdate.o ../x86/obj/osdb.o ../x86/obj/osio.o ../x86/obj/oslist.o ../x86/obj/osmatch.o  ../x86/obj/osreslex.o ../x86/obj/osresyac.o \
	  ../x86/obj/ostrace.o ../x86/obj/pop.o ../x86/obj/sc.o ../x86/obj/scaction.o ../x86/obj/sclex.o ../x86/obj/scparse.o ../x86/obj/scproc.o ../x86/obj/scprocio.o ../x86/obj/scprocmx.o ../x86/obj/scprocop.o ../x86/obj/scsys.o ../x86/obj/scyacc.o ../x86/obj/smtp.o ../x86/obj/sqlodbc.o ../x86/obj/sqlora.o ../x86/obj/tcp.o ../x86/obj/tp.o ../x86/obj/xf.o 

OBJ64=  ../x64/obj/cb2a.o ../x64/obj/cgi.o ../x64/obj/database.o ../x64/obj/dg.o ../x64/obj/dns.o ../x64/obj/dnscomp.o ../x64/obj/fiop.o ../x64/obj/ftp.o ../x64/obj/http.o ../x64/obj/mail.o ../x64/obj/mime.o ../x64/obj/mt.o ../x64/obj/mx.o ../x64/obj/mxmanager.o ../x64/obj/mxutil.o ../x64/obj/oscom.o ../x64/obj/osdate.o ../x64/obj/osdb.o ../x64/obj/osio.o ../x64/obj/oslist.o ../x64/obj/osmatch.o  ../x64/obj/osreslex.o ../x64/obj/osresyac.o \
	  ../x64/obj/ostrace.o ../x64/obj/pop.o ../x64/obj/sc.o ../x64/obj/scaction.o ../x64/obj/sclex.o ../x64/obj/scparse.o ../x64/obj/scproc.o ../x64/obj/scprocio.o ../x64/obj/scprocmx.o ../x64/obj/scprocop.o ../x64/obj/scsys.o ../x64/obj/scyacc.o ../x64/obj/smtp.o ../x64/obj/sqlodbc.o ../x64/obj/sqlora.o ../x64/obj/tcp.o ../x64/obj/tp.o ../x64/obj/xf.o 


../x86/obj/%.o: ../../../../src/%.c
	gcc -c  $< -DYY_USE_PROTOS -DWINDOWS -I$(INC_REP) -Wno-implicit-function-declaration -Wno-implicit-int -Wno-int-conversion -o  $@

../x64/obj/%.o: ../../../../src/%.c
	gcc -c  $< -DYY_USE_PROTOS -DWINDOWS -I$(INC_REP) -Wno-implicit-function-declaration -Wno-implicit-int -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast -Wno-int-conversion -Wno-	incompatible-pointer-types -o  $@

lib32: $(OBJ32)
	ar rcs ../x86/lib/libNetProg32.a $(OBJ32)

lib64: $(OBJ64)
	ar rcs ../x64/lib/libNetProg64.a $(OBJ64)

io_proto_store.o: ../../examples/IO_PROTO_STORE/io_proto_store.c
	gcc -c  ../../examples/IO_PROTO_STORE/io_proto_store.c -DYY_USE_PROTOS -DWINDOWS -I$(INC_REP)  

store: io_proto_store.o $(OBJ)
	gcc -o store io_proto_store.o -L. -lNetProg -lws2_32  -lodbc32

server.o: ../../examples/server/server.c
	gcc -c  ../../examples/server/server.c -DYY_USE_PROTOS -DWINDOWS -I$(INC_REP)  

server:	server.o $(OBJ)
	gcc -o server server.o -L. -lNetProg -lws2_32  -lodbc32

router.o: ../../examples/router/router.c
	gcc -c  ../../examples/router/router.c -DYY_USE_PROTOS -DWINDOWS -I$(INC_REP)  

router:	router.o $(OBJ)
	gcc -o router router.o -L. -lNetProg -lws2_32  -lodbc32

clean32:
	del ..\x86\obj\*.o 

clean64:
	del ..\x64\obj\*.o 