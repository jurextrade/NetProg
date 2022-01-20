#include "mx.h"
#include "mxmanager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;


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




void tracemessage (MXMessage* pmessage)
{
}


int main ()
{
	 MX mx;
	 MXInit (&mx, MXCLIENT, NULL, "server", 2000, "Resource.irc");
	 generateCFiles (&mx);
	 generateMakefile (&mx);
	 system ("mingw32-make all");
	 system ("server007.exe");
	 system ("server007.exe");
	 return 1;
}  

