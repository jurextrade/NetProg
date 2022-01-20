MXCreateDialogClass
^^^^^^^^^^^^^^^^^^^^^^^^



#include "mx.h"
MXDialogClass* MXCreateDialogClass (MX* pmx, char* name, WORD code)



Parameters	Description
pmx 	the MX manager
name 	the name of the dialog class to create
code 	the identifier of the dialog class


Description : 

Creates a dialog class. This function is equivalent to the DIALOG section in the MX Resource File. The code and name must be unique in the application. Both sides of the communicating applications should have the same code and name to communicate messages belonging to this dialog

Usually when two applications need to communicate, they define a dialog class that should be loaded on both sides. Connections that use a user defined Dialog have the protocol type IPPROTO_DG. An application connects to the other application with the MXOpenTCP function with IPPROTO_DG parameter, all messages belonging to these dialog classes will be understood by both sides.    

Predefined Dialog classes exist in the system. These classes are used for known protocols. It offers an elegant way to communicate with existing servers lihe FTP, HTTP, POP,...

The following class in your program

  pclass = MXCreateDialogClass(pmx, "MyClass" , 3);

  MXCreateMessageClass(pmx, pclass, "ParseString", 1, 3, "STRING", 1, "String", "LONG", 1, "Identity", "WORD", 1, "Persistency");
  MXCreateMessageClass(pmx, pclass, "ParseReply", 2, 6, "LONG", 1, "Identity", "WORD", 1, "Persistency", "SHORT", 1, "Type",
                                                                                   "WORD", 1, "Defined", "STRING", 1, "Val", "LONG", 1, "Error");

is equivalent in the resource file to 

  DIALOGCLASS 3 *MyClass*
  BEGIN
           MESSAGECLASS ParseStirng (STRING String LONG Identity WORD Persistency)
           MESSAGECLASS parseReply (LONG Identity WORD Persistency SHORT Type WORD Defined STRING Val LONG error)
  END



Return Values : returns the dialog class pointer if succeeded else returns NULL


See Also : MXCreateMessageClass, MXAddObjectToMessageClass 


Example :
  MXDialogClass * pclass;

  pclass = MXCreateDialogClass (pmx, DNS_SYS, DNS_SYS_CODE);

  MXCreateMessageClass (pmx, pclass, "Query", 1, 6, "LONG", 1, "Identity",
                                                                      "WORD", 1, "Operation",
                                                                      "WORD", 1, "QClass",
                                                                      "WORD", 1, "QType",
                                                                      "STRING", 1, "Domain",
                                                                      "BUFFER", 1, "RecordData");


  MXCreateMessageClass (pmx, pclass, "Reply", 1, 13,"LONG", 1, "Identity",
                                                                      "WORD", 1, "Operation",
                                                                      "WORD", 1, "QClass",
                                                                      "WORD", 1, "QType",
                                                                      "STRING", 1, "Domain",
                                                                      "WORD", 1, "Code", 
                                                                      "WORD", 1, "AnswerCount", 
                                                                      "WORD", 1, "AuthorityCount",
                                                                      "WORD", 1, "AdditionalCount",
                                                                      "WORD", 60,"Section",
                                                                      "STRING", 60,"Name",
                                                                      "WORD", 60,"Type",
                                                                      "STRING", 60,"Value");
