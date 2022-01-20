# Microsoft Developer Studio Project File - Name="NetProgLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=NetProgLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NetProgLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NetProgLib.mak" CFG="NetProgLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NetProgLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "NetProgLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NetProgLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "NetProgLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "WINDOWS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "NetProgLib - Win32 Release"
# Name "NetProgLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\cgi.c
# End Source File
# Begin Source File

SOURCE=..\database.c
# End Source File
# Begin Source File

SOURCE=..\Db.c
# End Source File
# Begin Source File

SOURCE=..\dg.c
# End Source File
# Begin Source File

SOURCE=..\dns.c
# End Source File
# Begin Source File

SOURCE=..\dnscomp.c
# End Source File
# Begin Source File

SOURCE=..\fiop.c
# End Source File
# Begin Source File

SOURCE=..\ftp.c
# End Source File
# Begin Source File

SOURCE=..\Gse.c
# End Source File
# Begin Source File

SOURCE=..\http.c
# End Source File
# Begin Source File

SOURCE=..\mail.c
# End Source File
# Begin Source File

SOURCE=..\mime.c
# End Source File
# Begin Source File

SOURCE=..\mx.c
# End Source File
# Begin Source File

SOURCE=..\MXManager.c
# End Source File
# Begin Source File

SOURCE=..\mxutil.c
# End Source File
# Begin Source File

SOURCE=..\Oscom.c
# End Source File
# Begin Source File

SOURCE=..\OSDate.c
# End Source File
# Begin Source File

SOURCE=..\Osdb.c
# End Source File
# Begin Source File

SOURCE=..\Osdraw.c
# End Source File
# Begin Source File

SOURCE=..\Osio.c
# End Source File
# Begin Source File

SOURCE=..\Oslist.c
# End Source File
# Begin Source File

SOURCE=..\osmatch.c
# End Source File
# Begin Source File

SOURCE=..\osras.c
# End Source File
# Begin Source File

SOURCE=..\osreslex.c
# End Source File
# Begin Source File

SOURCE=..\osresyac.c
# End Source File
# Begin Source File

SOURCE=..\Ostrace.c
# End Source File
# Begin Source File

SOURCE=..\pop.c
# End Source File
# Begin Source File

SOURCE=..\sc.c
# End Source File
# Begin Source File

SOURCE=..\scaction.c
# End Source File
# Begin Source File

SOURCE=..\sclex.c
# End Source File
# Begin Source File

SOURCE=..\scparse.c
# End Source File
# Begin Source File

SOURCE=..\scproc.c
# End Source File
# Begin Source File

SOURCE=..\scprocdb.c
# End Source File
# Begin Source File

SOURCE=..\scprocio.c
# End Source File
# Begin Source File

SOURCE=..\scprocmx.c
# End Source File
# Begin Source File

SOURCE=..\scprocop.c
# End Source File
# Begin Source File

SOURCE=..\scsys.c
# End Source File
# Begin Source File

SOURCE=..\scyacc.c
# End Source File
# Begin Source File

SOURCE=..\smtp.c
# End Source File
# Begin Source File

SOURCE=..\sqlodbc.c
# End Source File
# Begin Source File

SOURCE=..\sqlora.c
# End Source File
# Begin Source File

SOURCE=..\tcp.c
# End Source File
# Begin Source File

SOURCE=..\tp.c
# End Source File
# Begin Source File

SOURCE=..\U_xlat.c
# End Source File
# Begin Source File

SOURCE=..\xf.c
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Microsoft Visual Studio\VC98\Lib\RASAPI32.LIB"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Microsoft Visual Studio\VC98\Lib\WSOCK32.LIB"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Microsoft Visual Studio\VC98\Lib\ODBC32.LIB"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Gse.h
# End Source File
# Begin Source File

SOURCE=..\MXManager.h
# End Source File
# End Group
# Begin Group "SC Files"

# PROP Default_Filter "sc"
# Begin Source File

SOURCE=..\scfiles\a.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\b.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\compile_lexyacc.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\compile_NetProg.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\db.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\db1.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\dbaccess.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\dbaccess1.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\dbaddcolumn.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\dblocalaccess.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\dbsybase.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\dns.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\envoi.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\envoi1.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\essai.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\ftp.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\gr.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\io.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\journal.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\mail.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\Make_NetProgLib32.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\msysobjects.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\mx.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\oracle.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\ov.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\ping.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\pm3.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\remote.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\runsc.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\sc.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\SC1.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\smtp.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\system.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\transfer.sc
# End Source File
# Begin Source File

SOURCE=..\scfiles\vol.sc
# End Source File
# End Group
# Begin Group "http"

# PROP Default_Filter "html"
# Begin Source File

SOURCE=..\http\dns_example.html
# End Source File
# Begin Source File

SOURCE=..\http\ftp_example.html
# End Source File
# Begin Source File

SOURCE=..\http\http_example.html
# End Source File
# Begin Source File

SOURCE=..\http\smtp_example.html
# End Source File
# Begin Source File

SOURCE=..\http\store_example.html
# End Source File
# End Group
# Begin Group "LEXYACC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\new_lexyac\osreslex.l
# End Source File
# Begin Source File

SOURCE=..\new_lexyac\osresyac.y
# End Source File
# End Group
# End Target
# End Project
