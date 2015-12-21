//#define DEBUG 1

#include "group.c"
//#include <stdlib.h>
#include <linux/limits.h>

int main (int argc, char * argv [])
{
	//fn_tst ();
	
	struct group group;
	gid_t ** groups;
	
	//int * error = malloc (sizeof (int));
	int error_group, error_groups;
	char buffer [PATH_MAX];
	
	long int index, size;
	
	groups = malloc (sizeof (gid_t *));
	
	//* error = 0;
	error_group = 0;
	group_get ("id", "0", & group, buffer, PATH_MAX, & error_group);
	
	//return EXIT_FAILURE;
	
	error_groups = 0;
	size = 0;
	index = 0;
	_nss_exo_initgroups_dyn ("user_name", 0, & index, & size, groups, 0, & error_groups);
	
	//printf ("USHRT_MAX == %u\n", USHRT_MAX);
	//printf ("SSIZE_MAX == %u\n", SSIZE_MAX);
	
	//printf ("Error: %i\n.", * error);
	
	//if (error_group != 0 && error_groups != 0)
	//{
	//	return 2;
	//}
	
	//free (error);
	if (error_group == 0)
	{
		printf ("ID\t\t: [%i]\n", group.gr_gid);
		printf ("Name\t\t: [%s]\n", group.gr_name);
		printf ("Password\t: [%s]\n", group.gr_passwd);
		// List of user members.
		for (unsigned long int i = 0; i < 2; ++ i)
		{
			printf ("\tgroup.gr_mem [%i] = [%s]\n", i, group.gr_mem [i]);
		}
	}
	else
	{
		printf ("Error: group: [%i].\n", error_group);
		
		//return EXIT_FAILURE;
	}
	
	
	if (error_groups == 0)
	{
		printf ("Groups\n");
		printf ("\tGroup : Size = [%i]\n\n", size);
		
		for (unsigned long int i = 0; i + 1 < size; ++ i)
		{
			printf ("\tGroup [%i] = [%i]\n", i, (* groups) [i]);
			//printf ("\tGroup [%i] = [%i]\n", i, 0);
		}
		
		if (size > 0)
			printf ("\tGroup [%i] = [NULL]\n", size - 1);
	}
	else
	{
		printf ("Error: groups: [%i].\n", error_groups);
		
		//return EXIT_FAILURE + 1;
	}
	
	return EXIT_SUCCESS;
}
