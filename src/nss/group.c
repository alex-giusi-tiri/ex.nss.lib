#include "../main.h"
#include "../tool.h"

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
enum nss_status group_get (const char * getter_type, const char * getter_content, struct group * result, char * buffer, size_t buffer_size, int * error)
{
	NSS_DEBUG ("group_get : Called with buffer size of [%u]\n", buffer_size);
	NSS_DEBUG ("group_get : Looking for group %s [%s]\n", getter_type, getter_content);
	
	//signed int result_error_code;
	//unsigned long long int command_output_size = sizeof (char) * (SHRT_MAX + 1);
	//char * document_text;
	gid_t id;
	char * id_text;
	char * name;
	char * password;
	//const char * members_count_text;
//	unsigned long long int * members_count;
//	unsigned long long int member_index;
	//void * buffer_memory;
	//char * document_text;
	
	// The parsed XML document tree.
	//xmlDocPtr document;
	
	//FILE * file;
	//char line [PATH_MAX];
	//char command [PATH_MAX] = "";
	
	/*
	char message [256];
	// Prepare the message.
	// executable . " group get " . getter_type . " " . getter
	// Argument count: 5
	message = "";
	//strcat (message, executable);
	strcat (message, "get group ");
	strcat (message, getter_type);
	strcat (message, " ");
	strcat (message, getter_content);
	//strcat (message, "\0");
	*/
	
	//// Send the request:
	//if (!transmit (message))
	//	return NSS_STATUS_UNAVAIL;
	// Send the request (in parts):
	if (!transmit ("get"))
		return NSS_STATUS_UNAVAIL;
	if (!transmit ("group"))
		return NSS_STATUS_UNAVAIL;
	if (!transmit (getter_type))
		return NSS_STATUS_UNAVAIL;
	if (!transmit (getter_content))
		return NSS_STATUS_UNAVAIL;
	
	
	// Get the group ID (as a character pointer):
	if (!receive (id_text))
	{
		return NSS_STATUS_UNAVAIL;
	}
	
	// Get the name:
	if (!receive (name))
	{
		free (id_text);
		
		return NSS_STATUS_UNAVAIL;
	}
	
	// Get the password:
	if (!receive (password))
	{
		free (id_text);
		free (name);
		
		return NSS_STATUS_UNAVAIL;
	}
	
	
	// Convert the texts into numbers:
	id = strtoul (id_text, NULL, 10);
	
	result -> gr_gid = id;
	
	result -> gr_name = name;
	result -> gr_passwd = password;
	//result -> gr_mem = /*already set*/;
	
	//return NSS_STATUS_UNAVAIL;
	
	
	free (id_text);
	//free (group_id);
	//xmlFree (name);
	//xmlFree (password);
	//xmlFree (home);
	//xmlFree (shell);
	//xmlFree (gecos);
	
	
	*error = errno;
	
	NSS_DEBUG ("group_get : Returning successfully\n");
	
	return NSS_STATUS_SUCCESS;
}

// Initialize grent functionality.
enum nss_status _nss_exo_setgrent (void)
{
	NSS_DEBUG ("Initializing [group] functionality...\n");
	
	NSS_DEBUG ("Initialized [group] functionality\n");
	return NSS_STATUS_SUCCESS;
}

// Finalize grent functionality.
enum nss_status _nss_exo_endgrent (void)
{
	NSS_DEBUG ("Finalizing [group] functionality...\n");
	
	NSS_DEBUG ("Finalized [group] functionality\n");
	return NSS_STATUS_SUCCESS;
}

/*
	Get a group by its name.
	
	name          : Name of the group.
	buffer        : Buffer which will contain all strings pointed to by gbuf entries.
	buffer_size   : Size of the buffer.
	error         : Pointer to "error_number", which is set if an error occurs.
*/
enum nss_status _nss_exo_getgrnam_r (const char * name, struct group * result, char * buffer, size_t buffer_size, int * error)
{
	NSS_DEBUG ("_nss_exo_getgrnam_r(): calling with group name [%s] and buffer size [%i].\n", name, buffer_size);
	
	return group_get ("name", name, result, buffer, buffer_size, error);
}

/*
	Get group by its ID.
	
	@param gid GID.
	@param buf Buffer which will contain all string pointed to by gbuf entries.
	@param buflen buf length.
	@param errnop Pointer to errno; it is set if an error occurs.
*/

enum nss_status _nss_exo_getgrgid_r (gid_t id, struct group * result, char * buffer, size_t buffer_size, int * error)
{
	NSS_DEBUG ("_nss_exo_getgrgid_r (): calling with group ID [%d] and buffer size [%i].\n", id, buffer_size);
	
	// Include the terminating NULL character (byte).
	char id_text [12];
	snprintf (id_text, 12, "%d", id);
	
	return group_get ("id", id_text, result, buffer, buffer_size, error);
}

/*
 * Haven't seen any detailed documentation about this function.
 * Anyway it has to fill in all groups for the specified user without adding their main group (group parameter).
 * 
 * @param   user      Username whose groups are wanted.
 * @param   group     Main group of user (should not be included in groupsp).
 * @param   start     Index from where groups filling should begin (initgroups_dyn is called for every backend). May be updated
 * @param   size      Size of groups vector. Can be modified if function needs more space (should not exceed limit).
 * @param   groupsp   Pointer to the group vector. May be realloc'ed if more space is needed.
 * @param   limit     Max size of groupsp (<= 0 if no limit).
 * @param   errnop    Pointer to errno (filled if an error occurs).
 */
/*
	this function returns a list of groups, documentation for the
	interface is scarce (any pointers are welcome) but this is
	what is assumed the parameters mean:
	
	user      IN     - the user name to find groups for
	maingroup IN     - the user's main group; it should not be included in the list
	*start    IN/OUT - where to write in the array; it is incremented
	*size     IN/OUT - the size of the supplied array (gid_t entries, not bytes)
	**groupsp IN/OUT - pointer to the array of returned groupids
	limit     IN     - the maxium permitted size of the array
	*errnop   OUT    - for returning errno
*/

enum nss_status _nss_exo_initgroups_dyn (const char * user_name, gid_t group_main_id, long int * index_next, long int * size_current, gid_t ** groups, long int size_limit_max, int * error)
{
	NSS_DEBUG ("_nss_exo_initgroups_dyn : Called.");
	
	NSS_DEBUG ("_nss_exo_initgroups_dyn : size: %i; index: %i; limit: %i.", *size_current, *index_next, size_limit_max);
	
	char * success_text;
	char * groups_count_text;
	char * id_temp_text;
	unsigned long int groups_count;
	unsigned long int size_new;
	gid_t id_temp;
	gid_t * groups_temp;
	
//	signed int result_error;
	//const char * id_text;
	//const xmlChar * name;
	//const xmlChar * password;
	//unsigned long long int command_output_size = sizeof (char) * (SHRT_MAX + 1);
//	char * document_text;
//	unsigned long long int * members_count;
//	unsigned long long int member_index;
	//void * buffer_memory;
	//char * document_text;
	
	
	
	
	
	/*
	char message [256];
	// Prepare the message.
	// executable . " group get " . getter_type . " " . getter
	// Argument count: 5
	message = "";
	//strcat (message, executable);
	strcat (message, "get groups ");
	strcat (message, "name");
	strcat (message, " ");
	strcat (message, user_name);
	//strcat (message, "\0");
	*/
	// Send the request (in parts):
	if (!transmit ("get"))
		return NSS_STATUS_UNAVAIL;
	if (!transmit ("groups"))
		return NSS_STATUS_UNAVAIL;
	if (!transmit ("user_name"))
		return NSS_STATUS_UNAVAIL;
	if (!transmit (user_name))
		return NSS_STATUS_UNAVAIL;
	
	
	//// Send the request:
	//if (!transmit (message))
	//	return NSS_STATUS_UNAVAIL;
	
	
	// Get the success status:
	if (!receive (success_text))
	{
		return NSS_STATUS_UNAVAIL;
	}

	if (success_text == "0")
	{
		free (success_text);
		return NSS_STATUS_UNAVAIL;
	}
	
	
	// Get the user ID (as a character pointer):
	if (!receive (groups_count_text))
	{
		free (success_text);
		return NSS_STATUS_UNAVAIL;
	}
	
	// Convert the groups_count_text to number.
	groups_count = strtoul (groups_count_text, NULL, 10);
	
	
	size_new = (*index)/* + 1*/ + /*sizeof (gid_t *) * */groups_count;
	
	if (size_limit_max > 0 && size_new > size_limit_max)
	{
		free (success_text);
		//free (count_text);
		
		// limit exceeded; tell caller to try with a larger one
		//NSS_ERROR ("initgroups_dyn: limit was too low\n");
		NSS_DEBUG ("_nss_exo_initgroups_dyn : Limit exceeded.\n");
		
		*error = ERANGE;
		
		return NSS_STATUS_TRYAGAIN;
	}
	NSS_DEBUG ("_nss_exo_initgroups_dyn : Limit not exceeded. Continuing.\n");
	
	//* size = count;
	
	NSS_DEBUG ("size_new==[%u], *size==[%u]", size_new, *size_current);
	if (size_new > *size_current)
	{
		//(* size) = (* index) + 1 + (* members_count);
		
		if (*index_next > 0)
		{
			NSS_DEBUG ("_nss_exo_initgroups_dyn : Reallocating memory for member groups.\n");
			
			//(* groups) = realloc (* groups, sizeof (** groups) * (* size));
			//groups_temp = realloc (groups, sizeof (gid_t *) * size_new);
			groups_temp = realloc (*groups, sizeof (gid_t) * size_new);
		}
		else
		{
			NSS_DEBUG ("_nss_exo_initgroups_dyn : Allocating memory for member groups.\n");
			
			//groups_temp = malloc (sizeof (gid_t *) * size_new);
			groups_temp = malloc (sizeof (gid_t) * size_new);
		}
		
		if (groups_temp == NULL)
		{
			//pclose (file);
			
			free (success_text);
			//free (members_count);
			//xmlFreeDoc (document);
			
			*error = errno;
			
			NSS_DEBUG ("_nss_exo_initgroups_dyn : Failed to (re)allocate memory for the groups.\n");
			//
			return NSS_STATUS_UNAVAIL;
		}
		
		*size_current = size_new;
		
		free (*groups);
		*groups = groups_temp;
	}
	
	//(* size) = size_new;
	
	//NSS_DEBUG ("_nss_exo_initgroups_dyn : (Re)Allocated memory for the members.\n");
	
	NSS_DEBUG ("_nss_exo_initgroups_dyn : Looping through the user's member groups.\n");
	
	//for (unsigned long int i = 0; i < group_count; ++i)
	//{
	//	
	//}
	//member_index = 0;
	for (unsigned long int group_index = *index_next; group_index < *size_current; ++group_index)
	//do
	{
		//++ member_index;
		
		// Get the group ID (as a character pointer):
		if (!receive (id_temp_text))
		{
			return NSS_STATUS_UNAVAIL;
		}
		
		// Convert the ID into a number:
		id_temp = strtoul (id_temp_text, NULL, 10);
		
		//printf ("%s", line);
		//result -> gr_mem [member_index] = strdup (line);
		
		// Finally, set the ID:
		
		//(*groupsp)[*start] = gid;
		//groups [member_index] = id;
		(*groups) [group_index] = id_temp;
		
		NSS_DEBUG ("_nss_exo_initgroups_dyn : id_text == [%s].\n", id_temp_text);
		NSS_DEBUG ("_nss_exo_initgroups_dyn : id == [%i].\n", id_temp);
		//free (id);
		
		//NSS_DEBUG ("_nss_exo_initgroups_dyn : (*groups) [%i] == %i", group_index, (*groups) [member_index]);
		//NSS_DEBUG ("_nss_exo_initgroups_dyn : * (groups [%i]) == %i", member_index, * (groups [member_index]));
		
		NSS_DEBUG ("_nss_exo_initgroups_dyn : (*groups) [%i] == [%i].\n", group_index, (*groups) [group_index]);
		//NSS_DEBUG ("_nss_exo_initgroups_dyn : * (groups [%i]) == %i.\n", member_index, * (groups [member_index]));
	}
	//while (member_index < members_count && result -> gr_mem [member_index ++] != NULL);
	
	//* index = member_index;
	
	//free (members_count);
	
	//xmlFreeDoc (document);
	
	
	//result -> pw_gid = * id;
	
	//result -> gr_name = (char *) name;
	//result -> gr_passwd = (char *) password;
	//result -> gr_mem = /*already set*/;
	
	
	//free (id);
	//free (group_id);
	//xmlFree (name);
	//xmlFree (password);
	//xmlFree (home);
	//xmlFree (shell);
	//xmlFree (gecos);
	
	
	//*error = errno;
	
	/*
	if (* error == 0)
	{
		printf ("Groups\n");
		printf ("\tGroup : Size = [%i]\n\n", * size);
		
		for (unsigned long int i = 0; i + 1 < * size; ++ i)
		{
			printf ("\tGroup [%i] = [%i]\n", i, (* groups) [i]);
		}
		
		if (size > 0)
			printf ("\tGroup [%i] = [NULL]\n", (* size) - 1);
	}
	else
	{
		printf ("Error: groups: [%i].\n", * error);
		
		//return EXIT_FAILURE + 1;
	}
	*/
	
	NSS_DEBUG ("_nss_exo_initgroups_dyn : Returning.");
	
	return NSS_STATUS_SUCCESS;
}
