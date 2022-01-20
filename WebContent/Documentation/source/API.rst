
API
=====
 

.. toctree::

   Initialization.rst
   Message_functions.rst
   Connection_functions.rst
   Send_Receive_functions.rst
   Callbacks.rst
   

Initialization 	Sending and Receiving functions
Connection functions	Callbacks and Procedures


Initialization functions

MXInit	Initialize MX manager
MXEnd	Close MX manager and free all resources
MXCreateDialogClass	Create a dialog class
MXFreeDialogClass	Free the dialog class
MXCreateMessageClass	Create a message class in the dialog class
MXFreeMessageClass	Free the message class
MXCreateComClass	Create a connection class
MXFreeComClassFromName	Free a connection class 
MXFreeComClass	Free connection class
MXCreateMessage	Create a message instance 
MXFreeMessage	Free message instance
MXCopyMessage	Copy message instance
MXAddObjectToMessageClass	Add object to a message class
MXFreeObject	Free object in a message class
MXFreeObjectsFromMessageClass	 
MXFreeCom	Free connection instance
MXAddPort	Add a listening port to an application
MXGetPort	Returns the connection port number 
MXLoadResourceFile	Load a resource file in the MX manager
MXDispatchEvents	Dispatch events (MX manager engine)




Message functions



MXSetValue	Set object value  to a  message
MXGetValue	Return object value from its name
MXSetRangeValue	Set object value from its name
MXGetRangeValue	Return object value from its range in the message 
MXGetName	Set object value from its range
MXGetType	Get object type (STRING CHAR WORD ...)
MXGetInternalType	 
MXGetInternalLength	 
MXGetInternalScale	 
MXSetMessageStatut	Set message statut 
MXSetMessageDateHour	Set message output date and time

Connection functions 	


MXOpenTCP	Create a TCP connection
MXOpenBase	Create a database connection
MXOpenFile	create a file connection
MXCloseCom	Close connection and free all input/ouput messages
MXFlushCom	Flush all messages from the output queue of a connection

Sending and Receiving functions



MXPutMessage	Create and put a message in the ouput queue of a connection 
MXPutThisMessage	Put a created message in the ouput queue 
MXJoinCom	Join  two connections
MXGetComFromClient	Get the connection instance from its client
MXAddDGCommands	Add DG Dialog to MX manager
MXAddFTPCommands	Add FTP Dialog to MX manager
MXAddPOPCommands	Add POP Dialog to MX manager
MXAddSMTPCommands	Add SMTP Dialog to MX manager
MXAddTCPCommands	Add TCP Dialog to MX manager
MXAddDBCommands	Add DBG Dialog to MX manager
MXAddFIOPCommands	Add FIOP Dialog to MX manager
MXAddHTTPCommands	Add HTTP Dialog to MX manager
MXAddDNSCommands	Add DNS Dialog to MX manager
MXTranferOutputQueue	 
MXGetComClassFromName	Returns the connection class from the name parameter
MXGetComClassFromCode	Returns the connection class from the code parameter
MXGetDialogClassFromName	Returns the dialog class from the name parameter
MXGetDialogClassFromCode	Returns the dialog class from the code parameter
MXGetMessageClassFromName	Returns the message code from the name parmeter
MXGetMessageClassFromCodes	 
MXGetObjectFromName	Returns the object of a message from its name 
MXGetProtocolFromName	Returns the protocol identifier from the char name parameter
MXSetComProtocol	Set a protocol to a connection
MXReadyToSend	 
MXGetReadyOutputMessage	Returns the next message ready to send from the output queue of a connection
MXGetReadyInputMessage	Returns the next message ready to read from the input queue of a connection 
MXFindIncompleteMessage	 r
MXCheckType2	 
MXFreeContext	 
MXAllocateContext	 
MXExtendValueBuffer	 
MXGetMessageSize	 
MXGetMessageClassSize	 
MXSetIOMessageClass	 Associate a message class to a file fixed format connection
MXSetIOSize	 
MXGetFileSize	 
MXGetBufferSize	 
MXEstablishConnectionOn	 
MXSetCom	 
MXExecuteApplicationProcedures	 
MXReadMessageStatut	 
MXWriteBufferContent	 
MXFindMessage	 
MXReadFileContent	 
MXReadBufferContent	 
MXWriteFileContent	 
MXCreateMessageFromCodes	 
MXAllocMessage	Allocate a message instance
MXInitValues	Reset all message values
MXSetMode	 
MXSend	Send a message in a synchronous mode
MXRecv	Recv message in a synchronous mode
MXInitMessage	Initialize a message instance
MXFreeAndMoveComIn	 
MXSetFileValue	 
MXSetBufferValue	 
MXSetStringValue	 



Callbacks and Procedures	

MXFindApplicationProcedure	Returns an application procedure
MXFindCallBack	Returns a callback associated to a messsage class
MXFindGeneralCallBack	Returns a callback associated to 
MXFindComCallBack	 
MXAddGeneralCallBack	Add a callback on receiving or sending a message belonging to a message class 
MXAddApplicationProcedure	Add an application procedure that will be  executed by MX manager during the events dispatching
MXAddGeneralConnectCallBack	Add a callback that will be triggered on connecting or disconnecting any connection in the MX manager
MXAddConnectCallBack	Add a callback on connecting or disconnecting only connections belonging to a connection class 
MXAddCallBack	Add a callback on sending or receiving a message belonging to a message class
MXAddComCallBack	Add a callback on receiving or sending a message belonging to a message class on a  connection instance
MXAddEndingProcedure	Add a callback on closing a connection instance
MXRemoveGeneralCallBack	Remove a callback
MXRemoveApplicationProcedure	Remove an application procedure from the MX manager
MXRemoveConnectCallBack	Remove callback associated to a connection class
MXRemoveGeneralConnectCallBack	Remove a general callback from the MX manager
MXRemoveCallBack	Remove callback associated to a message class
MXRemoveComCallBack	Remove Callback
MXRemoveEndingProcedure	Remove Ending callback