#include "mx.h"

int main ()
{
	MX mx;
	MXInit (&mx, MXCLIENT, NULL, "client007", 2000, "Resource.irc");
	MXDispatchEvents (&mx, 0);
	return 1;
}
