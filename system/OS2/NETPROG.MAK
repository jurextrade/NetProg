PROJ = TEUB
PROJFILE = TEUB.MAK
DEBUG = 0

PWBRMAKE  = pwbrmake
NMAKEBSC1  = set
NMAKEBSC2  = nmake
CC  = cl
CFLAGS_G  = /AL /W3 /G2 /DUBI /BATCH
CFLAGS_D  = /Os /Gi$(PROJ).mdt
CFLAGS_R  = /Ot
ASM  = masm
AFLAGS_G  = /Mx /T
AFLAGS_D  = /Zi
LINKER	= link
ILINK  = ilink
LRF  = echo > NUL
BIND  = bind
RC  = rc
IMPLIB	= implib
LFLAGS_G  = /NOI /ST:8192  /BATCH /SE:256
LFLAGS_D  = /F /PACKC
LFLAGS_R  = /EXE /FAR /PACKC
MAPFILE_D  = NUL
MAPFILE_R  = NUL
LLIBS_G  = LLIBJM2 LLIBSM2 LLIBGR2 ASY_MCL socketl
BRFLAGS  = /o $(PROJ).bsc /Es /Em
BROWSE	= 0

OBJS_EXT  = ..\..\ASYNC\LIB\ASYNCH.OBJ
OBJS  = $(OBJS_EXT) CARELIST.obj DB_M.obj HOOKFUNC.obj JMAIN.obj MES_C.obj\
	PROTOFUN.obj SCRFUNC.obj TRACE.obj UTIL.obj ville.obj MSCXBTRV.obj\
	MAIN.obj GLOBAL.obj FUNCCONT.obj FUNCENTR.obj FONT.obj COMM.obj\
	SER.obj TCP.obj PROTODIV.obj UBI.obj UBETIQ.obj
SBRS  = CARELIST.sbr DB_M.sbr HOOKFUNC.sbr JMAIN.sbr MES_C.sbr PROTOFUN.sbr\
	SCRFUNC.sbr TRACE.sbr UTIL.sbr ville.sbr MSCXBTRV.sbr MAIN.sbr\
	GLOBAL.sbr FUNCCONT.sbr FUNCENTR.sbr FONT.sbr COMM.sbr SER.sbr TCP.sbr\
	PROTODIV.sbr UBI.sbr UBETIQ.sbr

all: $(PROJ).exe

.SUFFIXES: .c .sbr .obj

CARELIST.obj : CARELIST.C ext.h C:\C600\INCLUDE\stdio.h\
	C:\C600\INCLUDE\stdlib.h C:\C600\INCLUDE\dos.h\
	C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

CARELIST.sbr : CARELIST.C ext.h C:\C600\INCLUDE\stdio.h\
	C:\C600\INCLUDE\stdlib.h C:\C600\INCLUDE\dos.h\
	C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

DB_M.obj : DB_M.C C:\C600\INCLUDE\stdlib.h C:\C600\INCLUDE\stdio.h\
	C:\C600\INCLUDE\io.h C:\C600\INCLUDE\memory.h trace.h db_m.h

DB_M.sbr : DB_M.C C:\C600\INCLUDE\stdlib.h C:\C600\INCLUDE\stdio.h\
	C:\C600\INCLUDE\io.h C:\C600\INCLUDE\memory.h trace.h db_m.h

HOOKFUNC.obj : HOOKFUNC.C ext.h C:\C600\INCLUDE\stdio.h\
	C:\C600\INCLUDE\stdlib.h C:\C600\INCLUDE\dos.h\
	C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

HOOKFUNC.sbr : HOOKFUNC.C ext.h C:\C600\INCLUDE\stdio.h\
	C:\C600\INCLUDE\stdlib.h C:\C600\INCLUDE\dos.h\
	C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

JMAIN.obj : JMAIN.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

JMAIN.sbr : JMAIN.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

MES_C.obj : MES_C.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

MES_C.sbr : MES_C.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

PROTOFUN.obj : PROTOFUN.C ext.h C:\C600\INCLUDE\stdio.h\
	C:\C600\INCLUDE\stdlib.h C:\C600\INCLUDE\dos.h\
	C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

PROTOFUN.sbr : PROTOFUN.C ext.h C:\C600\INCLUDE\stdio.h\
	C:\C600\INCLUDE\stdlib.h C:\C600\INCLUDE\dos.h\
	C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

SCRFUNC.obj : SCRFUNC.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

SCRFUNC.sbr : SCRFUNC.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

TRACE.obj : TRACE.C C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\ctype.h

TRACE.sbr : TRACE.C C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\ctype.h

UTIL.obj : UTIL.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

UTIL.sbr : UTIL.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

ville.obj : ville.c ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

ville.sbr : ville.c ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

MSCXBTRV.obj : MSCXBTRV.C C:\C600\INCLUDE\dos.h

MSCXBTRV.sbr : MSCXBTRV.C C:\C600\INCLUDE\dos.h

MAIN.obj : MAIN.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

MAIN.sbr : MAIN.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

GLOBAL.obj : GLOBAL.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

GLOBAL.sbr : GLOBAL.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

FUNCCONT.obj : FUNCCONT.C ext.h C:\C600\INCLUDE\stdio.h\
	C:\C600\INCLUDE\stdlib.h C:\C600\INCLUDE\dos.h\
	C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

FUNCCONT.sbr : FUNCCONT.C ext.h C:\C600\INCLUDE\stdio.h\
	C:\C600\INCLUDE\stdlib.h C:\C600\INCLUDE\dos.h\
	C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

FUNCENTR.obj : FUNCENTR.C ext.h C:\C600\INCLUDE\stdio.h\
	C:\C600\INCLUDE\stdlib.h C:\C600\INCLUDE\dos.h\
	C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

FUNCENTR.sbr : FUNCENTR.C ext.h C:\C600\INCLUDE\stdio.h\
	C:\C600\INCLUDE\stdlib.h C:\C600\INCLUDE\dos.h\
	C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

FONT.obj : FONT.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

FONT.sbr : FONT.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

COMM.obj : COMM.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

COMM.sbr : COMM.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

SER.obj : SER.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

SER.sbr : SER.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

TCP.obj : TCP.C ext.h c:\tcp\include\sys\errno.h c:\tcp\include\sys\tcptypes.h\
	c:\tcp\include\netinet\in.h c:\tcp\include\sys\socket.h\
	c:\tcp\include\sys\ioctl.h c:\tcp\include\netdb.h\
	c:\tcp\include\arpa\inet.h C:\C600\INCLUDE\stdio.h\
	C:\C600\INCLUDE\stdlib.h C:\C600\INCLUDE\dos.h\
	C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\C600\INCLUDE\errno.h c:\tcp\include\sys\cdefs.h\
	c:\tcp\include\machine\endian.h C:\jam\include\smmach.h\
	C:\jam\include\sminstfn.h C:\jam\include\smattrib.h\
	C:\jam\include\smumisc.h C:\jam\include\smsetup.h\
	C:\jam\include\smvalids.h C:\jam\include\smedits.h\
	C:\jam\include\smproto.h konst.h data_c.h data_cb.h\
	C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\stddef.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h

TCP.sbr : TCP.C ext.h c:\tcp\include\sys\errno.h c:\tcp\include\sys\tcptypes.h\
	c:\tcp\include\netinet\in.h c:\tcp\include\sys\socket.h\
	c:\tcp\include\sys\ioctl.h c:\tcp\include\netdb.h\
	c:\tcp\include\arpa\inet.h C:\C600\INCLUDE\stdio.h\
	C:\C600\INCLUDE\stdlib.h C:\C600\INCLUDE\dos.h\
	C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\C600\INCLUDE\errno.h c:\tcp\include\sys\cdefs.h\
	c:\tcp\include\machine\endian.h C:\jam\include\smmach.h\
	C:\jam\include\sminstfn.h C:\jam\include\smattrib.h\
	C:\jam\include\smumisc.h C:\jam\include\smsetup.h\
	C:\jam\include\smvalids.h C:\jam\include\smedits.h\
	C:\jam\include\smproto.h konst.h data_c.h data_cb.h\
	C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\stddef.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h

PROTODIV.obj : PROTODIV.C ext.h C:\C600\INCLUDE\stdio.h\
	C:\C600\INCLUDE\stdlib.h C:\C600\INCLUDE\dos.h\
	C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

PROTODIV.sbr : PROTODIV.C ext.h C:\C600\INCLUDE\stdio.h\
	C:\C600\INCLUDE\stdlib.h C:\C600\INCLUDE\dos.h\
	C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

UBI.obj : UBI.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

UBI.sbr : UBI.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

UBETIQ.obj : UBETIQ.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h

UBETIQ.sbr : UBETIQ.C ext.h C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\dos.h C:\C600\INCLUDE\assert.h C:\C600\INCLUDE\bios.h\
	C:\C600\INCLUDE\conio.h C:\C600\INCLUDE\stdarg.h\
	C:\C600\INCLUDE\malloc.h C:\C600\INCLUDE\string.h\
	C:\C600\INCLUDE\graph.h C:\C600\INCLUDE\io.h C:\C600\INCLUDE\fcntl.h\
	C:\C600\INCLUDE\process.h C:\jam\include\smdefs.h\
	C:\jam\include\smerror.h C:\jam\include\smkeys.h\
	C:\jam\include\smbitops.h C:\jam\include\smglobs.h\
	C:\jam\include\smgraph.h lit.h requ.h myerror.h champs.h version.h\
	util.h comm.h ser.h tcp.h mes_c.h carelist.h hookfunc.h protofun.h\
	protodiv.h scrfunc.h ville.h btrieve.h db_m.h trace.h font.h ap100.h\
	ubi.h C:\jam\include\smmach.h C:\jam\include\sminstfn.h\
	C:\jam\include\smattrib.h C:\jam\include\smumisc.h\
	C:\jam\include\smsetup.h C:\jam\include\smvalids.h\
	C:\jam\include\smedits.h C:\jam\include\smproto.h konst.h data_c.h\
	data_cb.h C:\async\include\asynch_1.h C:\async\include\asynch_2.h\
	C:\C600\INCLUDE\errno.h C:\C600\INCLUDE\stddef.h\
	c:\tcp\include\sys\errno.h C:\C600\INCLUDE\ctype.h\
	C:\C600\INCLUDE\time.h C:\C600\INCLUDE\math.h C:\C600\INCLUDE\setjmp.h\
	C:\C600\INCLUDE\signal.h C:\jam\include\smcommon.h\
	C:\jam\include\smldb.h c:\tcp\include\sys\cdefs.h


$(PROJ).bsc : $(SBRS)
	$(PWBRMAKE) @<<
$(BRFLAGS) $(SBRS)
<<

$(PROJ).exe : $(OBJS)
!IF $(DEBUG)
	$(LRF) @<<$(PROJ).lrf
$(RT_OBJS: = +^
) $(OBJS: = +^
)
$@
$(MAPFILE_D)
$(LLIBS_G: = +^
) +
$(LLIBS_D: = +^
) +
$(LIBS: = +^
)
$(DEF_FILE) $(LFLAGS_G) $(LFLAGS_D);
<<
!ELSE
	$(LRF) @<<$(PROJ).lrf
$(RT_OBJS: = +^
) $(OBJS: = +^
)
$@
$(MAPFILE_R)
$(LLIBS_G: = +^
) +
$(LLIBS_R: = +^
) +
$(LIBS: = +^
)
$(DEF_FILE) $(LFLAGS_G) $(LFLAGS_R);
<<
!ENDIF
!IF $(DEBUG)
	$(LINKER) @$(PROJ).lrf
!ELSE
	$(LINKER) @$(PROJ).lrf
!ENDIF


.c.sbr :
!IF $(DEBUG)
	$(CC) /Zs $(CFLAGS_G) $(CFLAGS_D) /FR$@ $<
!ELSE
	$(CC) /Zs $(CFLAGS_G) $(CFLAGS_R) /FR$@ $<
!ENDIF

.c.obj :
!IF $(DEBUG)
	$(CC) /c $(CFLAGS_G) $(CFLAGS_D) /Fo$@ $<
!ELSE
	$(CC) /c $(CFLAGS_G) $(CFLAGS_R) /Fo$@ $<
!ENDIF


run: $(PROJ).exe
	$(PROJ).exe $(RUNFLAGS)

debug: $(PROJ).exe
	CV $(CVFLAGS) $(PROJ).exe $(RUNFLAGS)
