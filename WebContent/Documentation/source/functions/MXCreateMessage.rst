MXCreateMessage
^^^^^^^^^^^^^^^

*include "mx.h"*


.. c:function:: MXMessage* MXCreateMessage (MX* pmx, char* dialogclassname, char* messageclassname)

   :param pmx: the MX manager
   :param dialogclassname: the dialog class name
   :param messageclassname: the message class name
   :return: if error returns NULL else returns a message instance 
   :Description: 

      This function creates a message instance belonging to (dialogclassname, messageclassname). The message instance created is usually used as a parameter to the synchronous function MXSend


   :See Also: :c:func:`MXSend`, :c:func:`MXFreeMessage`, :c:func:`MXRecv`, :c:func:`MXCopyMessage`



.. code-block:: c
  :linenos:
  :caption: Example
  :emphasize-lines: 18
   


  MX mx;
  MXMessage* pmessage;

  char* Buf;

  BUFFERPARM Buffer;


  MXInit (&mx, MXSERVER, NULL, NULL, 0, NULL);


  /* Establish Connection with a Server and send a TCP message*/


  TCPCom = MXOpenTCP (&mx, "194.204.200.8", 21, IPPROTO_TCP, NULL, NULL, TRUE);
  if (!TCPCom) return 0;

  pmessage =  MXCreateMessage (&mx, TCP_SYS, "Stream");


  Buffer.BufferType = 'T';
  Buffer.BufferSize = strlen ("This is a text message") + 1;
  Buffer.BufferContent = "This is a text message";


  MXSetValue (pmessage, "Stream", 1, &Buffer);

  if (MXSend (&mx, TCPCom, message) <  0) 

    printf ("Error sending buffer\n");


  MXFreeMessage (&mx, pmessage);