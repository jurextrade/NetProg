MXInit
^^^^^^^^

*include "mx.h"*


.. c:function:: int MXInit (MX* pmx, BYTE mode, char* name, char* appliname, int port, char* resourcefile)

   :param pmx: the MX manager
   :param mode: the mode of initialisation (MXSERVER, MXCLIENT)
   :param name: the name of the program if using local connections
   :param appliname: the name of the application
   :param port: the listening port if mode is MXSERVER
   :param resourcefile: the resource file to load
   :return: if error return -1 else returns > 0   
   :description: 
   
   
      This function is used to initialize th ``MX manager``. This is the first function to call in your program, it allocates all resources related to ``MX``. If mode is ``MXCLIENT``, the MX manager does not accept income connections, but the application can connect to other applications. In this mode :c:func:`MXDispatchEvents` called 
      with the parameter miliseconds equal to 0, doesn't return as long as there is still opened connections in the application. When all connections are closed MXDispatchEvents returns 

      If mode is MXSERVER the MX manager accepts all income connections on the ports added with MXAddPort function. In this mode MXDispatchEvents called with the parameter miliseconds to 0, blocks indefinetly
      port corresponds to initial listening port with IPPROTO_DG protocol. Is equivalent to MXAddPort (port, IPPROTO_DG)

      **appliname** is the name of your application, can be NULL

      *name* is tne name of your program, can be NULL

      ``resourcefile`` is the file containing all resources related to your application. See Resource File Section.:

 



   :See Also: :c:func:`MXDispatchEvents`, :c:func:`MXEnd`







  
   

   
.. code-block:: c
  :linenos:
  :caption: Example
  :emphasize-lines: 5
   


   int main(int argc, char* argv[]) 
   {
      MX mx;

      MXInit (&mx, MXSERVER, NULL, NULL, 3010, NULL);
  
      MXAddGeneralConnectCallBack (&mx, MXONCONNECT, OnConnect, &mx);

      MXDispatchEvents (&mx, 0);

      MXEnd (&mx);
   }



