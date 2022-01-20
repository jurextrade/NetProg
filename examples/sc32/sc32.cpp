#include "mx.h"
#include "mxmanager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;
static const char s_szHexAscNo[] = "0123456789ABCDEF";

typedef struct
{
unsigned char    l : 4;
unsigned char    h : 4;
} BCD;

typedef struct
{
unsigned char    h : 4;
unsigned char    l : 4;
} BCD_REVERSE;

#define BCDFlag_Normal 0
#define BCDFlag_Reverse 1

void BCDToAscii(BYTE *SrcString, char DesString[5], int BCDflag)
{

	int i = sizeof (BCD);
		if ( BCDFlag_Normal == BCDflag)
		{
			*DesString++ = (char) s_szHexAscNo[((BCD *)SrcString)->l];
			*DesString++ = (char) s_szHexAscNo[((BCD *)SrcString)->h];
			SrcString = SrcString + sizeof (BCD);
			*DesString++ = (char) s_szHexAscNo[((BCD *)SrcString)->l];
			*DesString++ = (char) s_szHexAscNo[((BCD *)SrcString)->h];

		}

		if (BCDFlag_Reverse == BCDflag)
		{
			*DesString++ = (char) s_szHexAscNo[((BCD_REVERSE *)SrcString)->l];
			*DesString++ = (char) s_szHexAscNo[((BCD_REVERSE *)SrcString)->h];
		}
    *DesString = '\0';
}

/* BCD and BCD_REVERSE  are defined as */



int generateCFile (MXApplication* papplication)
{
	char filename[200]; 
	sprintf (filename, "%s%s.c",  ".\\", papplication->Name); 

	ofstream outFile;

	outFile.open(filename, ios_base::out | ios_base::trunc );
	
	if(outFile.fail()) 
	{ 
		char error[250];
		sprintf (error, "%s %s", "Can't open File for writing" , filename);
		return -1; 
	} 
	std::string s;
	std::string appliname = papplication->Name;
	std::string stype = (papplication->Class->Type == MXSERVER) ? "MXSERVER" : "MXCLIENT";

	s += "#include \"mx.h\"\n\n";
	s += "int main ()\n";
	s += "{\n";
	s += "	MX mx;\n";
	s += "	MXInit (&mx, " + stype + ", NULL, \"" +  appliname + "\", 2000, " + "\"Resource.irc\");\n";

	s += "	MXDispatchEvents (&mx, 0);\n";
	s += "	return 1;\n";
	s += "}\n";

	outFile << s;
    outFile.close ();
	return 0;
}

void generateCFiles (MX* pmx)
{
	if (!pmx->Manager) return;
	List* listapplication = pmx->Manager->Applications;
    while (listapplication)
    {
        MXApplication* papplication = (MXApplication *)listapplication->car;
		generateCFile (papplication);
        listapplication = listapplication->cdr;
    }
}

int generateMakefile (MX* pmx)
{
	if (!pmx->Manager) return -1;
	

	char filename[200]; 
	sprintf (filename, "%s%s",  ".\\", "Makefile"); 

	ofstream outFile;

	outFile.open(filename, ios_base::out | ios_base::trunc );
	
	if(outFile.fail()) 
	{ 
		char error[250];
		sprintf (error, "%s %s", "Can't open File for writing" , filename);
		return -1; 
	} 
	std::string s;
	std::string sall = "all:";

	s += "INC_REP = C:\\PROJECTS\\MT4-Progress\\NetProg\\include\n";
	s += "LIB_REP = C:\\PROJECTS\\MT4-Progress\\NetProg\\system\\GNU\n\n";
	List* listapplication = pmx->Manager->Applications;
    while (listapplication)
    {
        MXApplication* papplication = (MXApplication *)listapplication->car;
		std::string appliname = papplication->Name;
		sall += " " + appliname;
		s += appliname + ": " +  appliname + ".c\n";
		s += "	cc " + appliname + ".c -DYY_USE_PROTOS -DWIN32 -I$(INC_REP)  -L$(LIB_REP) -lNetProg -lws2_32 -lgdi32 -lodbc32 -o " + appliname + ".exe\n\n";
        listapplication = listapplication->cdr;
    }
	s += sall;
	outFile << s;
    outFile.close ();
	return 0;
	
}






void writeinStore (MX* pmx)
{
   MXCom*  FileCom = MXOpenFile (pmx, "Store.txt", "w", IOPROTO_STORE, 'D', 63);
   if (!FileCom) return; 

/* Put messages in queue */
   BYTE tab[10];
   int j = 0;

   MXMessage* pmessage = MXPutMessage (FileCom, "essai", "autorisation");


   MXSetValue (pmessage, "size", 1, &j);
   MXSetValue (pmessage, "twotexts", 1, "First Text*");
   MXSetValue (pmessage, "twotexts", 2, "Second Textzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz*");
   MXSetValue (pmessage, "tab", 1, tab);
     pmessage = MXPutMessage (FileCom, "essai", "autorisation");
	MXSetValue (pmessage, "size", 1, &j);
   MXSetValue (pmessage, "twotexts", 1, "First Text*");
   MXSetValue (pmessage, "twotexts", 2, "Second Textzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz*");
   MXSetValue (pmessage, "tab", 1, tab);
   
   MXFlushCom(pmx, FileCom);  /* send all messages in output queue */

   MXCloseCom (pmx, FileCom);    /* close file */
}

void writeinFiop (MX* pmx)
{
   MXCom* FileCom = MXOpenFile (pmx, "Fiop.txt", "w", IOPROTO_FIOP, 'D', 63);
   if (!FileCom) return; 

/* Put messages in queue */
   BYTE tab[10];
   WORD j = 0;
   memcpy (tab, "1234567890", 10);

   MXMessage* pmessage = MXPutMessage (FileCom, "essai", "autorisation");

   MXSetValue (pmessage, "size", 1, &j);
   MXSetValue (pmessage, "twotexts", 1, "First Text*");
   MXSetValue (pmessage, "twotexts", 2, "Second Textzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz*");
   MXSetValue (pmessage, "tab", 1, tab);
   pmessage = MXPutMessage (FileCom, "essai", "autorisation");
	MXSetValue (pmessage, "size", 1, &j);
   MXSetValue (pmessage, "twotexts", 1, "First Text*");
   MXSetValue (pmessage, "twotexts", 2, "Second Textzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz*");
   MXSetValue (pmessage, "tab", 1, tab);

   MXFlushCom(pmx, FileCom);  /* send all messages in output queue */


   MXCloseCom (pmx, FileCom);    /* close file */

   FileCom = MXOpenFile (pmx, "Fiop.txt", "r", IOPROTO_FIOP, 'D', 63);
	MXSetIOMessageClass(FileCom, MXGetMessageClassFromName (MXGetDialogClassFromName (pmx, "essai"), "autorisation"));
   pmessage = MXRecv (pmx, FileCom);
   char*   rtab     = (char* )  MXGetValue (pmessage, "tab",     1);
   STRING s    = (STRING)MXGetValue (pmessage, "twotexts",    1);
   STRING s1 = (STRING)  MXGetValue (pmessage, "twotexts", 2);

   WORD   size     = (WORD)  MXGetValue (pmessage, "size",     1);

   MXCloseCom (pmx, FileCom);    /* close file */

}



int main ()
{
	MX mx;
	MXMessage* pmessage;
	MXCom* pcom;
	char z[30];
	char bcd[30];
	sprintf (z, "%02x", 4660);
	WORD dec = 4660;
	BYTE* h = (BYTE*)&dec;
	int high = h[0];
	BCDToAscii ((BYTE*)&dec, z, 0);



	if (MXInit (&mx, MXCLIENT, NULL, "server", 2001, "Resource.irc") < 0) return -1;
	generateCFiles (&mx);
	generateMakefile (&mx);
	//system ("make.bat");

	writeinFiop(&mx);
	writeinStore(&mx);
	 
	pcom = MXOpenTCP (&mx, "127.0.0.1", 2000, IPPROTO_DG, NULL, "Server007", true);
	pmessage = MXPutMessage (pcom, "essai", "autorisation");
	   
	BYTE tab[10];
	WORD j = 0;
	MXSetValue (pmessage, "size", 1, &j);
	MXSetValue (pmessage, "twotexts", 1, "First Text*");
	MXSetValue (pmessage, "twotexts", 2, "Second Textzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz*");
	MXSetValue (pmessage, "tab", 1, tab);

	while (true)
		MXDispatchEvents (&mx, 1000000);
	 return 1;
}  

