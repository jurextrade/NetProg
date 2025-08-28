#define _CRT_SECURE_NO_WARNINGS
#include "mx.h" 
#include "smtp.h"
#include "mail.h"



int CommandReply (MXMessage* pmessage, MXCom* pcom, void* appfield) 
{
    BUFFERPARM*	   Buffer = (BUFFERPARM *)  MXGetValue (pmessage, "Buffer",     1);
  
    return 1;
}

void main ()

{

   MX mx;
   MXMessage* pmessage;
   MXCom* SMTPCom;
   EMailMessage* message;

   MXInit (&mx, MXSERVER, NULL, NULL, 0, NULL);
   MXAddPort (&mx, 2010, IPPROTO_TCP);
   MXAddGeneralCallBack(&mx,  TCP_SYS, "Stream", MXONRECV, CommandReply, NULL);




  // message = EMailCreateMessage ("Gabriel_Jureidini@yahoo.com",  "Gabriel_Jureidini@yahoo.com", NULL, NULL, "ESSAI", "CEST LE CORPS");
  // SendEmail (mx.TP, "pop.mail.yahoo.com", message); 
/* Establish Connection with SMTP Server */

  // SMTPCom = MXOpenTCP (&mx, "194.214.200.23", 25, IPPROTO_SMTP, NULL, NULL, TRUE);
  // if (!SMTPCom) return ;
 //  MXAddComCallBack (&mx, SMTPCom, SMTP_SYS, "Command_Reply", MXONRECV, CommandReply, NULL);



/* Put message in queue 

   pmessage = MXPutMessage (SMTPCom, "SMTP", "Send");



 

/* Initialize Message 



   MXSetValue (pmessage, "From",    1, "G JUREIDINI<g-jureidini@microsoft.com>"); 
   MXSetValue (pmessage, "To",      1, "ssdsds<g-jureidini@chronopost.fr>"); 
   MXSetValue (pmessage, "Cc",      1, "g-jureidini@chronopost.fr"); 
   MXSetValue (pmessage, "Bcc",     1, "g-jureidini@chronopost.fr"); 
   MXSetValue (pmessage, "Subject", 1, "SUJET"); 
   MXSetValue (pmessage, "Object", 1, "ceci est un test de texte avec retour");
   MXSetValue (pmessage, "Attachment",1, "c:/FTP.txt");

*/

   MXDispatchEvents (&mx, 0);

   MXEnd (&mx);

}

 
 
 