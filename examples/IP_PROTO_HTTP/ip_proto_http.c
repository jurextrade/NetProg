#define _CRT_SECURE_NO_WARNINGS
#include "mx.h" 
#include "../../include/http.h"



int SendRequest (MXMessage* pmessage, MXCom* pcom, void* AppField) 
{
   MXSetValue (pmessage, "Request-Line", 1, 
                      "GET http://sourceforge.net/tracker/?group_id=29057&atid=410216 HTTP/1.0");
   MXSetValue (pmessage, "Host", 1, "www.sourceforge.net");
   MXSetValue (pmessage, "Accept" , 1, 
          "image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-excel,\
           application/msword, application/vnd.ms-powerpoint, */*");
 
   MXSetValue (pmessage, "Accept-Language",1, "fr");
   MXSetValue (pmessage, "Accept-Encoding",1, "gzip, deflate");
   MXSetValue (pmessage, "User-Agent", 1, 
                          "Mozilla/4.0 (compatible; MSIE 5.01; Windows NT)");

   MXSetValue (pmessage, "Proxy-Connection",1, "Keep-Alive");
   MXSetValue (pmessage, "Pragma" , 1, "no-cache");



   return 1;
}

int RecvResponse (MXMessage* pmessage, MXCom* pcom, void* AppField)
{
   BUFFERPARM* buffer;
   STRING Response;
   FILE* f;
   int i = 0;
   int j = 0;
   

   Response = (STRING)MXGetValue (pmessage, "Status-Line", 1);
   buffer = MXGetValue (pmessage, "Content", 1);
   
   
   f = fopen ("C:/response.html", "wt");
   fwrite (buffer->BufferContent,   1, buffer->BufferSize, f);
   fclose (f);

 

/* RETURN url is in Buffer->BufferContent */

   free (buffer->BufferContent);
   free (buffer);
   return 1;
}





void main ()

{

   MX mx;
   MXMessage* pmessage;
   MXCom* HTTPCom;



/* Initialize MX manager */

   MXInit (&mx, MXSERVER, NULL, NULL, 0, NULL);

/* Connect to http server */ 

   HTTPCom = MXOpenTCP (&mx, "www.facebook.com", 80, IPPROTO_HTTP, NULL, NULL, TRUE);
   if (!HTTPCom) return;

   MXAddComCallBack (&mx, HTTPCom, HTTP_SYS, "Response",MXONRECV, RecvResponse, NULL); 
   MXAddComCallBack (&mx, HTTPCom, HTTP_SYS, "Request", MXONSEND, SendRequest,  NULL); 

/* Put message in queue */ 

   pmessage = MXPutMessage (HTTPCom, HTTP_SYS, "Request");



   MXDispatchEvents (&mx, 0);

   MXEnd (&mx);

}


 
 
