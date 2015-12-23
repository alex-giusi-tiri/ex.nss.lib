//#define DEBUG 1

#include "../Source/nss/passwd.c"
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
		printf ("User:\n");
		
		printf ("\tID\t\t: [%i]\n", result.pw_uid);
		printf ("\tName\t\t: [%s]\n", result.pw_name);
		printf ("\tGroup ID\t: [%i]\n", result.pw_gid);
		printf ("\tPassword\t: [%s]\n", result.pw_passwd);
		printf ("\tHome\t\t: [%s]\n", result.pw_dir);
		printf ("\tShell\t\t: [%s]\n", result.pw_shell);
		printf ("\tGECOS\t\t: [%s]\n", result.pw_gecos);
	}
	else
	{
		printf ("Error: [%i].\n", error);
		
		return 1;
	}
	
	return 0;
}
