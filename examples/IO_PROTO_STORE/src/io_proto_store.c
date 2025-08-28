#define _CRT_SECURE_NO_WARNINGS
#include "mx.h" 
#include "ftp.h"


int GetReply (MXMessage* pmessage, MXCom* pcom, void* appfield)
{ 
   STRING Value = (STRING) MXGetValue (pmessage, "Value", 1);
   LONG Identity= (LONG) MXGetValue (pmessage, "Identity", 1);
   FILEPARM* File = (FILEPARM*)MXGetValue (pmessage, "File", 1);
   LONG Error = (LONG) MXGetValue (pmessage, "Error", 1);

   printf ("%s, %d, (%s,%s, %c, %c, %c, %d, %d), %d\n", Value, Identity, 
            File->FileOri,
            File->FileDest,
            File->FileOriLocal, 
            File->FileDestLocal, 
            File->FileType, 
            File->FileSize, 
            File->FileError, Error); 
   free (File);
   return 1;
}


int DirReply (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
   STRING Value = (STRING)MXGetValue (pmessage, "Value", 1);
   STRING Par = (STRING)MXGetValue (pmessage, "Par", 1);
   LONG Error = (LONG)MXGetValue (pmessage, "Error", 1);
   BUFFERPARM* Buffer = (BUFFERPARM*)MXGetValue (pmessage, "Buffer", 1);
   if (Error != 0) return 1;

   printf ("%s, %s, %d, %.*s\n", Value, Par, Error, Buffer->BufferSize,   Buffer->BufferContent);
   free (Buffer->BufferContent);
   free (Buffer);
   return 1;
} 



int CommandReply (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
   LONG Code = (LONG) MXGetValue (pmessage, "Code", 1);
   STRING Reply = (STRING)MXGetValue (pmessage, "Reply", 1);
   LONG Identity = (LONG) MXGetValue (pmessage, "Identity", 1);
   STRING Value = (STRING)MXGetValue (pmessage, "Value", 1); 
   STRING Par = (STRING)MXGetValue (pmessage, "Par", 1);

   printf ("%d, %s, %d, %s, %s\n", Code, Reply, Identity, Value, Par);

   return 1;
}

int Command (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
   STRING Value = (STRING)MXGetValue (pmessage, "Value", 1);
   STRING Par = (STRING)MXGetValue (pmessage, "Par", 1);

   printf ("%s, %s\n", Value, Par);

   return 1;
}


void main ()
{
   MX mx;
   MXMessage* pmessage; 
   MXCom* FileCom;
   MXCom* FTPCom;
   char FileType = 't';



/* Initialize MX manager */

   MXInit (&mx, MXSERVER, NULL, NULL, 0, NULL);


/* Open file where to store messages */

   FileCom = MXOpenFile (&mx, "c:/FTP.bin", "w", IOPROTO_STORE, 'D', 0);
   if (!FileCom) return; 

/* Put messages in queue */

   pmessage = MXPutMessage (FileCom, FTP_SYS, "Command");
   MXSetValue (pmessage, "Value", 1, "USER");
   MXSetValue (pmessage, "Par", 1, " mt4-progress.com");

   pmessage = MXPutMessage (FileCom, FTP_SYS, "Command");
   MXSetValue (pmessage, "Value", 1, "PASS");
   MXSetValue (pmessage, "Par", 1, "jurex123");

   pmessage = MXPutMessage (FileCom, FTP_SYS, "dir");
   MXSetValue (pmessage, "Value", 1, "LIST");
   MXSetValue (pmessage, "Par", 1, "");

   pmessage = MXPutMessage(FileCom, FTP_SYS, "get") ;
   MXSetValue(pmessage, "FileName", 1, "t.c") ;
   MXSetValue(pmessage, "DestName", 1, "t.c") ;
   MXSetValue(pmessage, "FileType", 1, &FileType) ;
   MXSetValue(pmessage, "Command", 1, "get") ;

   MXFlushCom(&mx, FileCom);  /* send all messages in output queue */

   MXCloseCom (&mx, FileCom);    /* close file */

/* Open ftp connection */

   FTPCom = MXOpenTCP (&mx, "ftp.mt4-progress.com", 21, IPPROTO_FTP, NULL, NULL, TRUE);
   if (!FTPCom) return;
   MXAddComCallBack (&mx, FTPCom, FTP_SYS, "Command",       MXONSEND, Command, NULL);
   MXAddComCallBack (&mx, FTPCom, FTP_SYS, "Command_Reply", MXONRECV, CommandReply, NULL);
   MXAddComCallBack (&mx, FTPCom, FTP_SYS, "dir_reply",     MXONRECV, DirReply, NULL);
   MXAddComCallBack (&mx, FTPCom, FTP_SYS, "get_reply",     MXONRECV, GetReply, NULL);

/* Open file where to read messages */

   FileCom = MXOpenFile (&mx, "c:/FTP.bin", "r", IOPROTO_STORE, 'D', 63);
   if (!FileCom) return;

   while (pmessage = MXRecv (&mx, FileCom))    /* read messages from file */
   {
      MXPutThisMessage (FTPCom, pmessage);       /* put message in ftp output queue */
   }
   MXCloseCom (&mx, FileCom);      /* close file */

    

   MXDispatchEvents (&mx, 0);

   MXEnd (&mx);
} 

 
 