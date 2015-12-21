#include "default.h"
#include "tool.h"

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
enum nss_status group_get (const char * getter_type, const char * getter, struct group * result, char * buffer, size_t buffer_size, int * error)
{
	signed int result_error;
	unsigned long long int command_output_size = sizeof (char) * (SHRT_MAX + 1);
	char * command_output, * command_output_temp, * document_text;
	gid_t * id;
	//const char * id_text;
	xmlChar * name;
	xmlChar * password;
	//const char * members_count_text;
	unsigned long long int * members_count;
	unsigned long long int member_index;
	//void * buffer_memory;
	
	// The parsed XML document tree.
	xmlDocPtr document;
	
	FILE * file;
	char line [PATH_MAX];
	char command [PATH_MAX] = "";
	
	
	// executable . " group get " . getter_type . " " . getter
	// 5
	strcat (command, executable);
	strcat (command, " group get ");
	strcat (command, getter_type);
	strcat (command, " ");
	strcat (command, getter);
	
	// Open the command for reading.
	NSS_DEBUG ("group_get (): Opening file...");
	file = popen (command, "r");
	
	NSS_DEBUG ("group_get (): Opened file.");
	
	if (file == NULL)
	{
		NSS_DEBUG ("group_get (): Failed to run the executable: \"%s\".\n", command);
		
		* error = errno;
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("group_get (): Succeeded in running the executable: \"%s\".\n", command);
	
	command_output = malloc (command_output_size);
	strcpy (command_output, "");
	
	
	
	// Read the output one line at a time and save the line.
	while (fgets (line, USHRT_MAX - 1, file) != NULL)
	{
		NSS_DEBUG ("group_get (): Read line: \"%s\"", line);
		
		if (strlen (command_output) + strlen (line) + 1 > command_output_size)
		{
			command_output_size *= 2;
			command_output_temp = realloc (command_output, sizeof (char) * command_output_size);
			
			if (command_output_temp == NULL)
			{
				pclose (file);
				
				free (command_output);
				
				NSS_DEBUG ("group_get (): \"realloc ()\"ation failed.\n");
				
				return NSS_STATUS_UNAVAIL;
			}
			
			command_output = command_output_temp;
		}
		
		// Fill "command_output".
		strcat (command_output, line);
	}
	
	document_text = strdup (command_output);
	NSS_DEBUG ("group_get (): Duplicated the output: \"%s\".\n", document_text);
	
	
	// Uneeded anymore.
	free (command_output);
	
	result_error = WEXITSTATUS (pclose (file));
	if (result_error != 0)
	{
		/*
		free (id_text);
		free (name);
		free (password);
		free (group_id_text);
		free (shell);
		free (home);
		free (gecos);
		*/
		
		free (document_text);
		
		* error = errno;
		
		NSS_DEBUG ("group_get (): The executable did not run properly: the command was \"%s\"; the error was [%i].\n", command, result_error);
		
		return NSS_STATUS_UNAVAIL;
	}
	
	NSS_DEBUG ("group_get : Obtained command output : \"%s\".\n", /*command_output*/document_text);
	
	if (document_text == NULL)
	{
		NSS_DEBUG ("group_get : Failed to duplicate the XML document.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	/*
		As the document is in memory, it does not have a base, according to RFC 2396;
		then, the "noname.xml" argument will serve as its base.
	*/
	//document = xmlReadMemory (command_output, strlen (command_output), "noname.xml", NULL, 0);
	document = xmlReadMemory (document_text, strlen (document_text), "noname.xml", NULL, 0);
	//document = xmlReadMemory (command_output, 477, "noname.xml", NULL, 0);
	//document = xmlReadMemory ("<?xml version=\"1.0\"?><doc/>", 27, "noname.xml", NULL, 0);
	
	free (document_text);
	
	if (document == NULL)
	{
		NSS_DEBUG ("group_get : Failed to parse the XML document.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	
	NSS_DEBUG ("group_get : Parsed the XML document.\n");
	
	// Get the ID.
	id = (uid_t *) xml_xpath_evaluate_content_number (document, "/xml/group/id"/*, 10*/, 0, false);
	if (id == NULL)
	{
		//pclose (file);
		
		NSS_DEBUG ("group_get : \"document\" [%d].\n", document);
		//if (document == NULL)
		//	NSS_DEBUG ("group_get : \"document\" is null.\n");
		
		//free (document_text);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("group_get (): Failed to obtain: ID.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("group_get (): Obtained: ID: [%d].\n", * id);
	
	//return NSS_STATUS_UNAVAIL;
	
	
	// Get the name.
	name = xml_xpath_evaluate_content (document, "/xml/group/name", 0, false);
	if (name == NULL)
	{
		//pclose (file);
		
		free (id);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("group_get : Failed to obtain : Name.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("group_get : Obtained : Name : [%s].\n", (const char *) name);
	
	//return NSS_STATUS_UNAVAIL;
	
	
	// Get the password.
	password = xml_xpath_evaluate_content (document, "/xml/group/authentication/password/value", 0, false);
	if (password == NULL)
	{
		//pclose (file);
		
		xmlFree (name);
		free (id);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("group_get : Failed to obtain : Password.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("group_get : Obtained : Password : [%s].\n", (const char *) password);
	
	
	// Get the members count.
	members_count = (const unsigned long long int *) xml_xpath_evaluate_content_number (document, "/xml/group/users/count", 0, false);
	if (members_count == NULL)
	{
		//pclose (file);
		
		xmlFree (password);
		xmlFree (name);
		free (id);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("group_get : Failed to obtain : Members count.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("group_get : Obtained : Members count : [%d].\n", * members_count);
	
	
	// Get the members.
	result -> gr_mem = malloc (sizeof (char *) * ((* members_count) + 1));
	if (result -> gr_mem == NULL)
	{
		//pclose (file);
		
		free (members_count);
		xmlFree (password);
		xmlFree (name);
		free (id);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("group_get : Failed to allocate memory for the members.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	
	NSS_DEBUG ("group_get : Allocated memory for the members.\n");
	
	//member_index = 0;
	for (member_index = 0; member_index < * members_count; ++ member_index)
	//do
	{
		//++ member_index;
		
		//printf ("%s", line);
		//result -> gr_mem [member_index] = strdup (line);
		NSS_DEBUG ("group_get: Groups : Pre : result -> gr_mem [%i] == [%i]", member_index, result -> gr_mem [member_index]);
		//result -> gr_mem [member_index] = xml_xpath_evaluate_content (document, "/xml/group/users/list/member/name", member_index, false);
		result -> gr_mem [member_index] = xml_xpath_evaluate_content (document, "/xml/group/users/list/member/name", member_index, false);
		NSS_DEBUG ("group_get: Groups : Post : result -> gr_mem [%i] == [%i] (%s)", member_index, result -> gr_mem [member_index], result -> gr_mem [member_index]);
		//NSS_DEBUG ("group_get: Groups : Post : result -> gr_mem [%i] == [%s]", member_index, name);
		
		//NSS_DEBUG ("xml_xpath_evaluate_content (0):[%s]", xml_xpath_evaluate_content (document, "/xml/group/users/list/member/name", 0, false));
		//NSS_DEBUG ("xml_xpath_evaluate_content (1):[%s]", xml_xpath_evaluate_content (document, "/xml/group/users/list/member/name", 1, false));
		
		//return NSS_STATUS_UNAVAIL;
		
		if (name == NULL)
		{
			NSS_DEBUG ("group_get : Failed to get member [%i].\n", member_index);
			
			for (unsigned long long int i = 0; i < member_index; ++ i)
			{
				free (result -> gr_mem [i]);
			}
			free (result -> gr_mem);
			
			//free (result -> gr_name);
			//free (result -> gr_passwd);
			
			free (members_count);
			free (id);
			
			xmlFree (name);
			xmlFree (password);
			
			* error = errno;
			//* error = ERANGE;
			
			//NSS_DEBUG ("group_get : Failed to run the executable : \"%s\".\n", command);
			
			return NSS_STATUS_UNAVAIL;
		}
		
		//result -> gr_mem [member_index] = strdup (name);
		
		//NSS_DEBUG ("group_get: Groups : Post : result -> gr_mem [%i] == [%s]", member_index, result -> gr_mem [member_index]);
		
		//xmlFree (name);
		
		NSS_DEBUG ("group_set: Obtained member [%i]; name = [%s]", member_index, result -> gr_mem [member_index]);
	}
	//while (member_index < members_count && result -> gr_mem [member_index ++] != NULL);
	result -> gr_mem [member_index] == NULL;
	NSS_DEBUG ("group_get : obtained [%i] members out of [%i] total.\n", member_index, * members_count);
	
	
	xmlFreeDoc (document);
	
	if (strlen ((const char *) name) + 1 + strlen ((const char *) password) + 1 > buffer_size)
	{
		NSS_DEBUG ("group_get : Provided buffer is too small.\n");
		
		for (unsigned long long int i = 0; i < * members_count; ++ i)
		{
			free (result -> gr_mem [i]);
		}
		free (result -> gr_mem);
		//free (result -> gr_name);
		//free (result -> gr_passwd);
		free (members_count);
		free (id);
		
		xmlFree (name);
		xmlFree (password);
		//xmlFree (home);
		//xmlFree (shell);
		//xmlFree (gecos);
		
		//* error = errno;
		* error = ERANGE;
		
		//NSS_DEBUG ("group_get : Failed to run the executable : \"%s\".\n", command);
		
		return NSS_STATUS_TRYAGAIN;
	}
	
	
	NSS_DEBUG ("group_get : Obtained group.\n");
	
	
	
	result -> gr_gid = * id;
	
	result -> gr_name = (char *) name;
	result -> gr_passwd = (char *) password;
	//result -> gr_mem = /*already set*/;
	
	
	free (id);
	//free (group_id);
	//xmlFree (name);
	//xmlFree (password);
	//xmlFree (home);
	//xmlFree (shell);
	//xmlFree (gecos);
	
	
	* error = errno;
	
	NSS_DEBUG ("group_get : Returning.\n");
	
	return NSS_STATUS_SUCCESS;
}

// Initialize grent functionality.
enum nss_status _nss_sqlite_setgrent (void)
{
	NSS_DEBUG ("Initializing \"passwd\" functionality.\n");
	
	/*
		this initialize the library and check potential ABI mismatches
		between the version it was compiled for and the actual shared
		library used.
	*/
	LIBXML_TEST_VERSION
	
	xmlInitParser ();
	
	return NSS_STATUS_SUCCESS;
}

// Finalize grent functionality.
enum nss_status _nss_sqlite_endgrent (void)
{
	NSS_DEBUG ("Finilizing \"passwd\" functionality.\n");
	
	// Cleanup function for the XML library.
	xmlCleanupParser ();
	
	// This is to debug memory for regression tests.
	#if defined (DEBUG)
		xmlMemoryDump ();
	#endif
	
	return NSS_STATUS_SUCCESS;
}

/*
	Get group by its name.
	
	name		:	Name of the group.
	buffer		:	Buffer which will contain all string pointed to by gbuf entries.
	buffer_size	:	Size of the Buffer.
	error		:	Pointer to "error_number", which is filled up if an error occurs.
*/

enum nss_status _nss_exo_getgrnam_r (const char * name, struct group * result, char * buffer, size_t buffer_size, int * error)
{
	NSS_DEBUG ("_nss_exo_getgrgid_r (): calling with group name [%s] and buffer size [%i].\n", name, buffer_size);
	
	return group_get ("name", name, result, buffer, buffer_size, error);
}

/*
	Get group by its ID.
	
	@param gid GID.
	@param buf Buffer which will contain all string pointed to by gbuf entries.
	@param buflen buf length.
	@param errnop Pointer to errno, will be filled if an error occurs.
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
	NSS_DEBUG ("_nss_exo_initgroups_dyn : Entering.");
	
	NSS_DEBUG ("_nss_exo_initgroups_dyn : size: %i; index: %i; limit: %i.", * size, * index, limit);
	
	signed int result_error;
	gid_t * id, * groups_temp;
	//const char * id_text;
	//const xmlChar * name;
	//const xmlChar * password;
	//const char * members_count_text;
	long int size_new;
	unsigned long long int command_output_size = sizeof (char) * (SHRT_MAX + 1);
	char * command_output, * command_output_temp, * document_text;
	unsigned long long int * members_count;
	unsigned long long int member_index;
	//void * buffer_memory;
	
	// The parsed XML document tree.
	xmlDocPtr document;
	
	FILE * file;
	char line [PATH_MAX];
	char command [PATH_MAX] = "";
	
	
	// executable . " group get " . getter_type . " " . getter
	// 5
	strcat (command, executable);
	strcat (command, " groups get ");
	strcat (command, "name");
	strcat (command, " '");
	strcat (command, name);
	strcat (command, "'");
	
	// Open the command for reading.
	file = popen (command, "r");
	
	NSS_DEBUG ("_nss_exo_initgroups_dyn : Reading...");
	
	if (file == NULL)
	{
		NSS_DEBUG ("_nss_exo_initgroups_dyn : Failed to run the executable : \"%s\".\n", command);
		
		* error = errno;
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("_nss_exo_initgroups_dyn : Succeeded in running the executable : \"%s\".\n", command);
	
	command_output = malloc (command_output_size);
	strcpy (command_output, "");
	
	
	
	// Read the output one line at a time and save the line.
	while (fgets (line, USHRT_MAX - 1, file) != NULL)
	{
		NSS_DEBUG ("_nss_exo_initgroups_dyn : Read : \"%s\"", line);
		
		if (strlen (command_output) + strlen (line) + 1 > command_output_size)
		{
			command_output_size *= 2;
			command_output_temp = realloc (command_output, sizeof (char) * command_output_size);
			
			if (command_output_temp == NULL)
			{
				pclose (file);
				
				free (command_output);
				
				NSS_DEBUG ("_nss_exo_initgroups_dyn : \"realloc ()\" failed.\n");
				
				return NSS_STATUS_UNAVAIL;
			}
			
			command_output = command_output_temp;
		}
		
		// Fill "command_output".
		strcat (command_output, line);
	}
	
	document_text = strdup (command_output);
	
	
	// Uneeded anymore.
	free (command_output);
	
	result_error = WEXITSTATUS (pclose (file));
	if (result_error != 0)
	{
		/*
		free (id_text);
		free (name);
		free (password);
		free (group_id_text);
		free (shell);
		free (home);
		free (gecos);
		*/
		
		free (document_text);
		
		* error = errno;
		
		NSS_DEBUG ("_nss_exo_initgroups_dyn : The executable did not run properly : the command was \"%s\".\n", command);
		
		return NSS_STATUS_UNAVAIL;
	}
	
	NSS_DEBUG ("_nss_exo_initgroups_dyn : Obtained command output : \"%s\".\n", /*command_output*/document_text);
	
	if (document_text == NULL)
	{
		NSS_DEBUG ("_nss_exo_initgroups_dyn : Failed to duplicate the XML document.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	/*
		As the document is in memory, it does not have a base, according to RFC 2396;
		then, the "noname.xml" argument will serve as its base.
	*/
	//document = xmlReadMemory (command_output, strlen (command_output), "noname.xml", NULL, 0);
	document = xmlReadMemory (document_text, strlen (document_text), "noname.xml", NULL, 0);
	//document = xmlReadMemory (command_output, 477, "noname.xml", NULL, 0);
	//document = xmlReadMemory ("<?xml version=\"1.0\"?><doc/>", 27, "noname.xml", NULL, 0);
	
	free (document_text);
	
	if (document == NULL)
	{
		NSS_DEBUG ("_nss_exo_initgroups_dyn : Failed to parse the XML document.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	
	NSS_DEBUG ("_nss_exo_initgroups_dyn : Parsed the XML document.\n");
	
	// Get the count of groups (this should exclude the main group).
	members_count = (unsigned long long int *) xml_xpath_evaluate_content_number (document, "/xml/user/groups/count", 0, false);
	if (members_count == NULL)
	{
		//pclose (file);
		
		//xmlFree (password);
		//xmlFree (name);
		free (id);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("_nss_exo_initgroups_dyn : Failed to obtain : Members count.\n");
		//
		return NSS_STATUS_UNAVAIL;
	}
	
	
	size_new = (* index)/* + 1*/ + /*sizeof (gid_t *) * */(* members_count);
	
	if (limit > 0 && size_new > limit)
	{
		//free (count_text);
		
		// limit exceeded; tell caller to try with a larger one
		//NSS_ERROR ("initgroups_dyn: limit was too low\n");
		NSS_DEBUG ("_nss_exo_initgroups_dyn : Limit exceeded.\n");
		
		* error = ERANGE;
		
		return NSS_STATUS_TRYAGAIN;
	}
	NSS_DEBUG ("_nss_exo_initgroups_dyn : Limit not exceeded. Continuing.\n");
	
	//* size = count;
	
	NSS_DEBUG ("size_new [%u], * size [%u]", size_new, * size);
	if (size_new > (* size))
	{
		//(* size) = (* index) + 1 + (* members_count);
		
		if (* index > 0)
		{
			NSS_DEBUG ("_nss_exo_initgroups_dyn : Reallocated memory for member groups.\n");
			
			//(* groups) = realloc (* groups, sizeof (** groups) * (* size));
			//groups_temp = realloc (groups, sizeof (gid_t *) * size_new);
			groups_temp = realloc (* groups, sizeof (gid_t) * size_new);
		}
		else
		{
			NSS_DEBUG ("_nss_exo_initgroups_dyn : Allocated memory for member groups.\n");
			
			//groups_temp = malloc (sizeof (gid_t *) * size_new);
			groups_temp = malloc (sizeof (gid_t) * size_new);
		}
		
		if (groups_temp == NULL)
		{
			//pclose (file);
			
			free (members_count);
			xmlFreeDoc (document);
			
			* error = errno;
			
			NSS_DEBUG ("_nss_exo_initgroups_dyn : Failed to (re)allocate memory for members.\n");
			//
			return NSS_STATUS_UNAVAIL;
		}
		
		* size = size_new;
		
		* groups = groups_temp;
	}
	
	//(* size) = size_new;
	
	//NSS_DEBUG ("_nss_exo_initgroups_dyn : (Re)Allocated memory for the members.\n");
	
	NSS_DEBUG ("_nss_exo_initgroups_dyn : Looping through the user's member groups.\n");
	
	//member_index = 0;
	for (member_index = 0; member_index < (* members_count); ++ member_index)
	//do
	{
		//++ member_index;
		
		//printf ("%s", line);
		//result -> gr_mem [member_index] = strdup (line);
		id = (gid_t *) xml_xpath_evaluate_content_number (document, "/xml/user/groups/list/member/id", member_index, false);
		
		if (id == NULL)
		{
			NSS_DEBUG ("_nss_exo_initgroups_dyn : Failed to get the ID for member indexed %i.\n", member_index);
			
			//for (unsigned long long int i = * index; i <= member_index; ++ i)
			//{
			//	free (groups [i]);
			//}
			//free (groups);
			
			//free (result -> gr_name);
			//free (result -> gr_passwd);
			
			free (members_count);
			//free (id);
			
			//xmlFree (name);
			//xmlFree (password);
			
			//* error = errno;
			* error = ERANGE;
			
			//NSS_DEBUG ("group_get : Failed to run the executable : \"%s\".\n", command);
			
			return NSS_STATUS_UNAVAIL;
		}
		
		//(*groupsp)[*start] = gid;
		//groups [member_index] = id;
		(* groups) [member_index] = * id;
		
		NSS_DEBUG ("_nss_exo_initgroups_dyn : id == %i.\n", * id);
		free (id);
		
		NSS_DEBUG ("_nss_exo_initgroups_dyn : (* groups) [%i] == %i", member_index, (* groups) [member_index]);
		//NSS_DEBUG ("_nss_exo_initgroups_dyn : * (groups [%i]) == %i", member_index, * (groups [member_index]));
		
		NSS_DEBUG ("_nss_exo_initgroups_dyn : (* groups) [%i] == %i.\n", member_index, (* groups) [member_index]);
		//NSS_DEBUG ("_nss_exo_initgroups_dyn : * (groups [%i]) == %i.\n", member_index, * (groups [member_index]));
	}
	//while (member_index < members_count && result -> gr_mem [member_index ++] != NULL);
	
	
	free (members_count);
	
	xmlFreeDoc (document);
	
	
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
	
	
	* error = errno;
	
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

