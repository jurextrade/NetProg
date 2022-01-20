
Functionalities
===============

NetProg rests on MX API functionalities. MX allows the programming of messages.

.. toctree::
   :hidden:
   
   Asynchronous.rst
   Protocols.rst



General  
 

MX Engine 's role is  the management of the events related to connections (opening, closing, cut...),  input/output of the messages (reception, emission). For each event, it starts the associated callbacks. This is done by the management of queues for transmitted or receipts messages; we speak about asynchronous communication

  

MX also offers the synchronous mode, which makes it possible to the application to transmit a message instantaneously.

The 2 modes of synchronous and asynchronous communication can coexist on a same connection.

It offers a means to the programmer of creating transfer journals allowing to know the course of a message transfer, the date of emission or reception.

MX allows also to connect to databases in the same logical manner which is based also on messages. Request and results are also seen as messages.

A language of script allowing the programming of distributed orders, it facilitates the development of communicating applications by the simple execution of files scripts. It is possible to send and recover files à.la.mode ftp, to manage variables, to associate orders certain types of events, to question data bases, to send messages, to carry out remote controls.

Model  
 

The conceptualization of the exchanges between  applications to communicate is done in 2 stages :  

 

Definition of classes of dialogue containing the messages. Messages are represented by attributes (file, entirety, buffer, number,...). A message is only referred by the name of the dialogue class to which it belongs and by its message class name.

Association between the entities-events and callbacks.  


 

Type of messages  

Messages with differed sendings

If the message sent has the property " differed sending ", it will be sent to the dates and hours requested by engine.

Persistent messages

If the message sent has the property " persisting " this one is preserved in the newspaper of the transfers defined by the programmer.

 

Messages with marked reception

If the message sent has the property " marked reception ", engine MX of the transmitting application will start, on arrival of the marked event reception, the callback associated by the programmer.   

 

DL Distributed Language  

Language DL is a language of script, inspired by language LISP, allowing the programming of commands distributed on various machines.

DL can be used in 3 ways:  

 

As a use of language DL in interpreted mode, with three operating modes:

Writing of commands DL in command files (also called scripts of commands).  

Call to functions DL starting from a program C.

Interactive mode, where the user can carry out commands DL in a shell.

 

Strong points  

No code compilation

Modification in real time of the behavior of the distant machine.

Mechanism of waiting.

Support of the mechanism containing messages of MX

Call to functions DL starting from a program C