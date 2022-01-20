INC_REP = C:\PROJECTS\MT4-Progress\NetProg\include
LIB_REP = C:\PROJECTS\MT4-Progress\NetProg\system\GNU

server007: server007.c
	cc server007.c -DYY_USE_PROTOS -DWIN32 -I$(INC_REP)  -L$(LIB_REP) -lNetProg -lws2_32 -lgdi32 -lodbc32 -o server007.exe

bankserver007: bankserver007.c
	cc bankserver007.c -DYY_USE_PROTOS -DWIN32 -I$(INC_REP)  -L$(LIB_REP) -lNetProg -lws2_32 -lgdi32 -lodbc32 -o bankserver007.exe

client007: client007.c
	cc client007.c -DYY_USE_PROTOS -DWIN32 -I$(INC_REP)  -L$(LIB_REP) -lNetProg -lws2_32 -lgdi32 -lodbc32 -o client007.exe

all: server007 bankserver007 client007