MXCreateMessageClass
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

*#include "mx.h"*


.. c:function:: MXMessageClass* MXCreateMessageClass (MX* pmx, MXDialogClass* pdialogclass, char* name, WORD code, int nb_param, ...)

   :param pmx: the MX manager
   :param pdialogclass: 	the dialog class
   :param name: 	the name of the message class
   :param code: 	the identifier of the message class
   :param nb_param: 	number of objects in the message class
   :return: if error returns NULL else returns the message class 

   :description: 
      Creates a message class. This function is equivalent to the MESSAGECLASS in the DIALOGCLASS section in the MX Resource File. The code and name parameters must be unique in the Dialog class
   
   :See Also: :c:func:`MXCreateDialogClass`, :c:func:`MXAddObjectToMessageClass`

.. code-block:: c
  :linenos:
  :caption: Example
  :emphasize-lines: 12

  MXDialogClass * pclass;

  pclass = MXCreateDialogClass(pmx, POP_SYS, POP_SYS_CODE);

  MXCreateMessageClass(pmx, pclass, "Command", 1 , 3, "LONG", 1, "Identity", "STRING", 1, "Value", "STRING", 1, "Par");

  MXCreateMessageClass(pmx, pclass, "Command_Reply", 2, 5, "LONG", 1, "Identity", "STRING", 1, "Value", "STRING", 1, "Par", "LONG", 1, "Code", "STRING", 1, "Reply"); 
