#define DEBUG 1

#include "passwd.c"
//#include <stdlib.h>
//#include <linux/limits.h>

signed int main (signed int argc, char * argv [])
{
	struct passwd * result;
	int * error;
	char buffer [PATH_MAX];
	
	user_get ("id", "0", result, buffer, PATH_MAX, error);
	
	return 0;
}

