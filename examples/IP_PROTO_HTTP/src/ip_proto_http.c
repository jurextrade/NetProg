#define _CRT_SECURE_NO_WARNINGS
#include "mx.h" 


char FileName[500] = { 0 };

int SendRequest1(MXMessage* pmessage, MXCom* pcom, void* AppField)
{
	char url[500] = { 0 };
	char userid[] = "1";
	char projectname[] = "DemoProject";
	
	char* filename = (char*)AppField;

	sprintf(url, "GET /members/%s/EMV/%s/Files/%s HTTP/1.1", userid, projectname, filename);

	MXSetValue(pmessage, "Request-Line", 1, url);
	MXSetValue(pmessage, "Host", 1, "localhost");

	return 1;
}


int SendRequest (MXMessage* pmessage, MXCom* pcom, void* AppField) 
{
   MXSetValue (pmessage, "Request-Line", 1, "POST /php/solution_dialog.php HTTP/1.1");
   MXSetValue(pmessage, "Host", 1, "localhost");

   MXSetValue(pmessage, "Content-Type", 1, "application/x-www-form-urlencoded");


   char content[400] = { 0 };
   sprintf(content, "%s", "message={\"Name\":\"scandir_r\",\"Values\":[\".\",\"\"]}");

   BUFFERPARM	Buffer;
   Buffer.BufferType = 'T';
   Buffer.BufferSize = strlen(content);
   Buffer.BufferContent = (char*)&content; 
   
   MXSetValue(pmessage, "Content", 1, &Buffer);

   char contentlength[10] = { 0 };
   sprintf(contentlength, "%d", Buffer.BufferSize);
   
   MXSetValue(pmessage, "Content-Length", 1, contentlength);
   

   MXSetValue(pmessage, "Accept", 1, "image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-excel, application/msword, application/vnd.ms-powerpoint, */*");
   MXSetValue(pmessage, "Accept-Language", 1, "fr");
   MXSetValue(pmessage, "Accept-Encoding", 1, "gzip, deflate");
   MXSetValue(pmessage, "User-Agent", 1, "Mozilla/4.0 (compatible; MSIE 5.01; Windows NT)");
   MXSetValue(pmessage, "Proxy-Connection", 1, "Keep-Alive");
   MXSetValue(pmessage, "Pragma", 1, "no-cache");

   return 1;
}

int RecvResponse (MXMessage* pmessage, MXCom* pcom, void* AppField)
{
   BUFFERPARM* buffer;
   STRING Response;
   FILE* f;
   int i = 0;
   int j = 0;
   
   char* filename = (char*)AppField;
   char repertory[] = "C:/";
   char pathfile[500] = { 0 };
   sprintf(pathfile, "%s/%s", repertory, filename);


   Response = (STRING)MXGetValue (pmessage, "Status-Line", 1);
   buffer = MXGetValue (pmessage, "Content", 1);
   
   
   f = fopen (pathfile, "wt");
   fwrite (buffer->BufferContent,   1, buffer->BufferSize, f);
   fclose (f);

 

/* RETURN url is in Buffer->BufferContent */

   free (buffer->BufferContent);
   free (buffer);
   return 1;
}


void tradeprocedure(void* pmessage, BYTE type, char* stream, int size) {
	printf("%.*s", size, stream);
}


void main ()

{

   MX mx;
   MXCom* HTTPCom;


/* Initialize MX manager As a server */

   MXInit (&mx, MXSERVER, NULL, NULL, 0, NULL);

/* Connect to http server */ 

   HTTPCom = MXOpenTCP (&mx, "localhost", 80, IPPROTO_HTTP, NULL, NULL, TRUE);
   if (!HTTPCom) return;

   MXSetTraceProcedure(HTTPCom, tradeprocedure);
   MXAddComCallBack (&mx, HTTPCom, HTTP_SYS, "Response",MXONRECV, RecvResponse, FileName);
   MXAddComCallBack (&mx, HTTPCom, HTTP_SYS, "Request", MXONSEND, SendRequest1,  FileName); 

/* Put message in queue */ 


   MXMessage* psend_message;
   MXMessage* precv_message;


 //  psend_message = MXPutMessage (HTTPCom, HTTP_SYS, "Request");



   psend_message = MXCreateMessage(HTTPCom->MX, HTTP_SYS, "Request");

   strcpy(FileName, "emv_applications.conf");
   MXSend(HTTPCom->MX, HTTPCom, psend_message);
   precv_message = MXRecv(HTTPCom->MX, HTTPCom);
   MXFreeMessage(HTTPCom->MX, precv_message);



   strcpy(FileName, "emv_acceptor.conf");
   MXSend(HTTPCom->MX, HTTPCom, psend_message);
   precv_message = MXRecv(HTTPCom->MX, HTTPCom);
   MXFreeMessage(HTTPCom->MX, precv_message);

   strcpy(FileName, "emv_currencies.conf");
   MXSend(HTTPCom->MX, HTTPCom, psend_message);
   precv_message = MXRecv(HTTPCom->MX, HTTPCom);
   MXFreeMessage(HTTPCom->MX, precv_message);

   MXFreeMessage(HTTPCom->MX, psend_message);
   MXDispatchEvents (&mx, 0);

   MXEnd (&mx);

}


 
 
