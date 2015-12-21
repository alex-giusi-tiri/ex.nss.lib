//#define DEBUG 1

#include "passwd.c"
//#include <stdlib.h>
//#include <linux/limits.h>

int main (int argc, char * argv [])
{
	//fn_tst ();
	
	struct passwd result;
	//int * error = malloc (sizeof (int));
	int error;
	char buffer [PATH_MAX];
	
	//* error = 0;
	error = 0;
	user_get ("id", "0", & result, buffer, PATH_MAX, & error);
	
	//printf ("Error: %i\n.", * error);
	
	//free (error);
	if (error == 0)
	{
		printf ("ID\t\t: [%s]\n", result.pw_uid);
		printf ("Name\t\t: [%s]\n", result.pw_name);
		printf ("Password\t: [%s]\n", result.pw_passwd);
		printf ("Home\t\t: [%s]\n", result.pw_dir);
		printf ("Shell\t\t: [%s]\n", result.pw_shell);
		printf ("GECOS\t\t: [%s]\n", result.pw_gecos);
	}
	else
	{
		printf ("Error: [%i].\n", error);
		
		return 1;
	}
	
	return 0;
}
