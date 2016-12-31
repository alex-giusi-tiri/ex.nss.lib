//#define DEBUG 1

#include "../Source/nss/group.c"
//#include <stdlib.h>
#include <linux/limits.h>

int main (int argc, char * argv [])
{
	//fn_tst ();
	
	printf ("SSIZE_MAX == %u\n", SSIZE_MAX);	// 4294967295
	printf ("USHRT_MAX == %u\n", USHRT_MAX);	// 65535
	printf ("PATH_MAX == %u\n", PATH_MAX);		// 4096
	
	return 0;
	
	struct group group;
	gid_t ** groups;
	
	//int * error = malloc (sizeof (int));
	int error_group, error_groups;
	char buffer [PATH_MAX];
	
	long int index, size;
	
	groups = malloc (sizeof (gid_t *));
	
	//* error = 0;
	error_group = 0;
	group_get ("name", "college", & group, buffer, PATH_MAX, & error_group);
	
	//printf ("strlen (NULL) == %u\n", strlen ("\0"));
	
	//return EXIT_FAILURE;
	
	error_groups = 0;
	size = 0;
	index = 0;
	_nss_exo_initgroups_dyn ("college", 0, & index, & size, groups, 0, & error_groups);
	
	
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
		unsigned long int i = 0;
		//for (unsigned long int i = 0; i < 1; ++ i)
		while (group.gr_mem [i] != NULL)
		{
			printf ("\tgroup.gr_mem [%i] == [%s]\n", i, group.gr_mem [i]);
			
			++ i;
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
