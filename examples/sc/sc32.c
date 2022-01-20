#include "sc.h"


main ()
{
 SC sc;


 SCInit (&sc, NULL,  TRUE, "db_compiere.sc");

 MXDispatchEvents (sc.MX, 0);

}  