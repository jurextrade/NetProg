Examples
-------------------

.. _a link: https://domain.invalid/


.. csv-table:: 
   :class: myexample
   :header: "IP", "client", "server", "Connection", "Dialog", "Examples"
   :widths: 20, 20, 20, 30, 30, 30

   "TCP",  "yes",  "yes",  "IP_PROTO_TCP", "TCP",  `a link`_
   "FTP",  "yes",  "no",  "IP_PROTO_FTP", "FTP", "example"
   "HTTP",  "yes",  "no",  "IP_PROTO_SMTP", "HTTP", "example"
   "SMTP",  "yes",  "no",  "IP_PROTO_POP", "SMTP", "example"
   "POP",  "yes",  "no",  "IP_PROTO_POP", "POP", "example"
   "DNS",  "yes",  "no",  "IP_PROTO_DNS", "DNS", "example"
   "DG",  "yes",  "yes",  "IP_PROTO_DG", "USER_DEFINED", "example"
  
   "IO",   "",  "",  "", "", ""
   "STORE",  "yes",  "-",  "IO_PROTO_STORE", "ANY", "example"
   "FIXED",  "yes",  "-",  "IO_PROTO_FIOP", "USER_DEFINED", "example"

   "DATABASE", "", "", "", "", ""
   "ODBC", "yes", "-", "DB_PROTO_ODBC", "DB", "example"
   "ORACLE", "yes",  "-", "DB_PROTO_ORACLE", "DB", "example"
   "SQL/DS", "yes",  "", "DB_PROTO_SQLDS", "DB", "example"