#include "mx.h"

int main ()
{
	MX mx;
	MXInit (&mx, MXSERVER, NULL, "server007", 2000, "Resource.irc");
	MXDispatchEvents (&mx, 0);
	return 1;
}
