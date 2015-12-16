#include "default.h"
//#include "tool.h"

//#include <stdio.h>
//#include <stdlib.h>
//#include <linux/limits.h>
//#include <nss.h>
//#include <errno.h>
#include <grp.h>
#include <malloc.h>
#include <pthread.h>
#include <string.h>

/*
*/
enum nss_status group_get (const char * get_type, const char * get, struct group * result, char * buffer, size_t buffer_size, int * error)
{
	signed int result_error;
	gid_t id;
	const char * id_text;
	//const char * name;
	//const char * password;
	const char * members_count_text;
	unsigned long long int members_count;
	unsigned long long int member_index;
	void * buffer_memory;
	
	FILE * file;
	char line [PATH_MAX];
	char exe [PATH_MAX] = "";
	
	
	// executable . " group get " . get_type . " " . get
	// 5
	strcat (exe, executable);
	strcat (exe, " password get ");
	strcat (exe, get_type);
	strcat (exe, " ");
	strcat (exe, get);
	
	// Open the command for reading.
	file = popen (exe, "r");
	
	if (file == NULL)
	{
		pclose (file);
		
		//NSS_DEBUG ("Failed to run the command.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	
	
	// Get the group's ID.
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		//NSS_DEBUG ("Failed to parse the executable's output : ID of Group\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	id_text = strdup (line);
	errno = 0;
	id = strtoul (id_text, NULL, 10);
	if (errno != 0)
	{
		pclose (file);
		
		free (id_text);
		
		NSS_DEBUG ("getspnam_r : Failed to parse number.n");
	
		return NSS_STATUS_UNAVAIL;
	}
	
	// Get the group's name.
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		free (id_text);
		
		//NSS_DEBUG ("Failed to parse the executable's output : Name of Group\n");

		return NSS_STATUS_UNAVAIL;
	}
	result -> gr_name = strdup (line);
	
	// Get the group's password.
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		free (id_text);
		free (result -> gr_name);
		
		//NSS_DEBUG ("Failed to parse the executable's output : Password of Group\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	result -> gr_passwd = strdup (line);
	
	// Get the count of members.
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		free (id_text);
		free (result -> gr_name);
		free (result -> gr_passwd);
		
		//NSS_DEBUG ("Failed to parse the executable's output : Password of Group\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	members_count_text = strdup (line);
	errno = 0;
	members_count = strtoul (members_count_text, NULL, 10);
	if (errno != 0)
	{
		pclose (file);
		
		free (id_text);
		free (result -> gr_name);
		free (result -> gr_passwd);
		
		NSS_DEBUG ("getspnam_r : Failed to parse number.n");
	
		return NSS_STATUS_UNAVAIL;
	}
	
	
	// Get the members of the group.
	buffer_memory = malloc (sizeof (char *) * members_count);
	if (buffer_memory == NULL)
	{
		pclose (file);
		
		free (id_text);
		free (result -> gr_name);
		free (result -> gr_passwd);
		free (members_count_text);
		
		return NSS_STATUS_UNAVAIL;
	}
	
	result -> gr_mem = (char **) buffer_memory;
	// Read the output one line at a time.
	member_index = 0;
	while (fgets (line, PATH_MAX, file) != NULL)
	{
		//printf ("%s", line);
		result -> gr_mem [member_index] = strdup (line);
		
		++ member_index;
	}
	
	result_error = WEXITSTATUS (pclose (file));
	
	free (members_count_text);
	free (id_text);
	
	if (result_error != 0)
	{
		for (unsigned long long int i = 0; i < member_index; ++ i)
		{
			free (result -> gr_mem [i]);
		}
		free (result -> gr_mem);
		
		free (result -> gr_name);
		free (result -> gr_passwd);
		
		//NSS_DEBUG ("Retrieving group #%d : Failure\n", id);
		
		return NSS_STATUS_UNAVAIL;
	}
	
	//free (id_text);
	//free (result -> gr_name);
	//free (result -> gr_name);
	//free (members_count_text);
	
	//NSS_DEBUG ("Retrieved group #%d: %s\n", id, name);
	
	return NSS_STATUS_SUCCESS;
}

/*
	Get group by name.
	
	name			:	Name of the group.
	buffer			:	Buffer which will contain all string pointed to by gbuf entries.
	buffer_size		:	Buffer size.
	error			:	Pointer to "error_number", which is filled if an error occurs.
*/

enum nss_status _nss_sqlite_getgrnam_r (const char * name, struct group * result, char * buffer, size_t buffer_size, int * error)
{
	return group_get ("name", name, result, buffer, buffer_size, error);
}

/*
	Get group by ID.
	
	@param gid GID.
	@param buf Buffer which will contain all string pointed to by gbuf entries.
	@param buflen buf length.
	@param errnop Pointer to errno, will be filled if an error occurs.
*/

enum nss_status _nss_exo_getgrgid_r (gid_t id, struct group * result, char * buffer, size_t buffer_size, int * error)
{
	return group_get ("id", itos (id), result, buffer, buffer_size, error);
}

/*
 * Haven't seen any detailed documentation about this function.
 * Anyway it has to fill in all groups for the specified user without adding their main group (group parameter).
 * @param user Username whose groups are wanted.
 * @param group Main group of user (should not be included in groupsp).
 * @param start Index from which groups filling must begin (initgroups_dyn is called for every backend). May be updated
 * @param size Size of groups vector. Can be modified if function needs more space (should not exceed limit).
 * @param groupsp Pointer to the group vector. May be realloc'ed if more space is needed.
 * @param limit Max size of groupsp (<= 0 if no limit).
 * @param errnop Pointer to errno (filled if an error occurs).
 */

enum nss_status _nss_exo_initgroups_dyn (const char * name, gid_t group_id, long int * index, long int * size, gid_t ** groups, long int limit, int * error)
{
	signed int result_error;
	//gid_t id;
	//const char * id_text;
	//const char * name;
	//const char * password;
	const char * count_text;
	unsigned long long int count;
	//unsigned long long int member_index;
	//void * buffer_memory;
	
	FILE * file;
	char line [PATH_MAX];
	char exe [PATH_MAX] = "";
	char group_id_text [PATH_MAX];
	
	// executable . " groups get name " . user/* . " " . itos (gid)*/
	// Argument count: 4
	strcat (exe, executable);
	strcat (exe, " groups get name ");
	strcat (exe, name);
	
	// Open the command for reading.
	file = popen (exe, "r");
	
	if (file == NULL)
	{
		pclose (file);
		
		//NSS_DEBUG ("Failed to run the command.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	
	
	// Get the count of groups (it excludes the main group).
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		//NSS_DEBUG ("Failed to parse the executable's output : ID of Group\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	count_text = strdup (line);
	errno = 0;
	count = strtoul (count_text, NULL, 10);
	if (errno != 0)
	{
		pclose (file);
		
		free (count_text);
		
		NSS_DEBUG ("getspnam_r : Failed to parse number.n");
		
		return NSS_STATUS_UNAVAIL;
	}
	
	/*
	// Get the main group (ID).
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		free (count_text);
		
		//NSS_DEBUG ("Failed to parse the executable's output : ID of Group\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	id_text = strdup (line);
	//result -> gr_gid = stoi (id_txt);
	*/
	
	if (limit > 0 && (* index) + 1 + count > limit)
	{
		free (count_text);
		
		// limit exceeded; tell caller to try with a larger one
		//NSS_ERROR ("initgroups_dyn: limit was too low\n");
		
		(* error) = ERANGE;
		
		return NSS_STATUS_TRYAGAIN;
	}
	
	//* size = count;
	
	if ((* index) + 1 + count > (* size))
	{
		(* size) = (* index) + 1 + count;
		
		(* groups) = realloc (* groups, sizeof (** groups) * (* size));
	}
	
	/*
	// Get the groups.
	//buffer_memory = malloc (sizeof (gid_t) * count);
	if (buffer_memory == NULL)
	{
		pclose (file);
		
		free (count_text);
		
		return NSS_STATUS_UNAVAIL;
	}
	
	* groups = (gid_t *) buffer_memory;
	
	*/
	// Read the output line by line.
	//index = 0;
	while (fgets (line, PATH_MAX, file) != NULL)
	{
		//printf ("%s", line);
		strcpy (group_id_text, line);
		//group_id = stoi (group_id_text);
		(* groups) [* index] = group_id;
		
		//++ (* index);
	}
	
	result_error = WEXITSTATUS (pclose (file));
	if (result_error != 0)
	{
		//for (unsigned long long int i = 0; i < index; ++ i)
		//{
		//	free (groups [i]);
		//}
		//free (groups);
			//result -> gr_mem = NULL;
		free (count_text);
		
		//NSS_DEBUG ("Retrieving group #%d : Failure\n", id);
		
		return NSS_STATUS_UNAVAIL;
	}
	
	free (count_text);
	
	//NSS_DEBUG ("Retrieved group #%d: %s\n", id, name);
	
	return NSS_STATUS_SUCCESS;
}

