MXEnd
^^^^^^^^

*#include "mx.h"*


.. c:function:: void MXEnd (MX* pmx)

   :param pmx: the MX manager
   :return: no return value
   :description: 

      This function terminates the MX session of an application. Usually it is called at the end of the program before exiting. It frees all resources, closes all the connections. 
   :See Also: :c:func:`MXInit`


.. code-block:: c
  :linenos:
  :caption: Example
  :emphasize-lines: 12


   int main ()
   {
     MX mx;
   
     MXInit (&mx, MXSERVER, NULL, NULL, 0, NULL);
     MXAddPort (&mx, 7080, IPPROTO_TCP);
     MXAddGeneralConnectCallBack (&mx, MXONCONNECT, TCPOnConnect, NULL);
     MXAddGeneralConnectCallBack (&mx, MXONCLOSE, TCPOnClose, NULL);
   
     MXDispatchEvents (&mx, 0);
   
     MXEnd (&mx);
   }