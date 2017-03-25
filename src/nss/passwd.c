#include "../main.h"
#include "../tool.h"

#include <string.h>
//#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <grp.h>
#include <pwd.h>

// Get user by either its ID or its name.
enum nss_status user_get (const char * getter_type, const char * getter_content, struct passwd * result, char * buffer, size_t buffer_size, int * error)
{
	//* error = 123;
	
	NSS_DEBUG ("user_get : Called with buffer size of [%u]\n", buffer_size);
	NSS_DEBUG ("user_get : Looking for user %s [%s]\n", getter_type, getter_content);
	
	//return NSS_STATUS_UNAVAIL;
	
//	signed int result_error_code;
//	unsigned int required_buffer_size;
	//unsigned long long int command_output_size = sizeof (char) * (SHRT_MAX + 1);
	//size_t command_output_size = sizeof (char) * 8;
	uid_t id;
	gid_t group_id;
	char * success_text;
	char * id_text;
	char * group_id_text;
	char * name;
	char * password;
	char * shell;
	char * home;
	char * gecos;
	
	char message [256];
	
	/*
	// Prepare the message.
	// executable . " user get " . argument_type . " " . argument
	// Argument count: 5
	message = "";
	//strcat (message, executable);
	strcat (message, "get user ");
	strcat (message, getter_type);
	strcat (message, " ");
	strcat (message, getter_content);
	//strcat (message, "\0");
	*/
	
	// Send the request (in parts):
	if (!transmit ("get"))
		return NSS_STATUS_UNAVAIL;
	if (!transmit ("user"))
		return NSS_STATUS_UNAVAIL;
	if (!transmit (getter_type))
		return NSS_STATUS_UNAVAIL;
	if (!transmit (getter_content))
		return NSS_STATUS_UNAVAIL;
	
	
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
	if (!receive (id_text))
	{
		free (success_text);
		
		return NSS_STATUS_UNAVAIL;
	}
	
	// Get the primary group ID (as a character pointer):
	if (!receive (group_id_text))
	{
		free (success_text);
		free (id_text);
		
		return NSS_STATUS_UNAVAIL;
	}
	
	// Get the name:
	if (!receive (name))
	{
		free (success_text);
		free (id_text);
		free (group_id_text);
		
		return NSS_STATUS_UNAVAIL;
	}
	
	// Get the password:
	if (!receive (password))
	{
		free (success_text);
		free (id_text);
		free (group_id_text);
		free (name);
		
		return NSS_STATUS_UNAVAIL;
	}
	
	// Get the shell:
	if (!receive (shell))
	{
		free (success_text);
		free (id_text);
		free (group_id_text);
		free (name);
		free (password);
		
		return NSS_STATUS_UNAVAIL;
	}
	
	if (!receive (home))
	{
		free (success_text);
		free (id_text);
		free (group_id_text);
		free (name);
		free (password);
		free (shell);
		
		return NSS_STATUS_UNAVAIL;
	}
	
	if (!receive (gecos))
	{
		free (success_text);
		free (id_text);
		free (group_id_text);
		free (name);
		free (password);
		free (shell);
		free (home);
		
		return NSS_STATUS_UNAVAIL;
	}
	
	
	////return 1;
	
	//// At this point, document_text should point to valid content.
	
	// Convert the texts into numbers:
	id = strtoul (id_text, NULL, 10);
	group_id = strtoul (group_id_text, NULL, 10);
	
	//printf ("%i = %i\n", result -> pw_uid, * id);
	result -> pw_uid = id;
	//printf ("%i = %i\n", result -> pw_uid, * id);
	//return 1;
	
	result -> pw_gid = group_id;
	
	//user -> pw_name = (char *) name;
	//user -> pw_passwd = (char *) password;
	//user -> pw_dir = (char *) home;
	//user -> pw_shell = (char *) shell;
	//user -> pw_gecos = (char *) gecos;
	
	result -> pw_name = name;
	result -> pw_passwd = password;
	result -> pw_dir = home;
	result -> pw_shell = shell;
	result -> pw_gecos = gecos;
	
	free (success_text);
	free (id_text);
	free (group_id_text);
	//xmlFree (name);
	//xmlFree (password);
	//xmlFree (home);
	//xmlFree (shell);
	//xmlFree (gecos);
	
	/*
	free (id_text);
	free (group_id_text);
	free (name);
	free (password);
	free (shell);
	free (home);
	*/
	
	*error = errno;
	
	NSS_DEBUG ("user_get : Returning successfully\n");
	
	return NSS_STATUS_SUCCESS;
}

// Initialize getpwent functionality:
// Setup everything needed to retrieve passwd entries.
enum nss_status _nss_exo_setpwent (void)
{
	NSS_DEBUG ("Initializing [passwd] functionality...\n");
	
	/*
		This initialize the library and check potential ABI mismatches
		between the version it was compiled for and the actual shared
		library used.
	*/
	//LIBXML_TEST_VERSION
	
	//xmlInitParser ();
	
	NSS_DEBUG ("Initialized [passwd] functionality\n");
	
	return NSS_STATUS_SUCCESS;
}

// Finalize getpwent functionality.
enum nss_status _nss_exo_endpwent (void)
{
	NSS_DEBUG ("Finilizing [passwd] functionality...\n");
	
	//// Cleanup function for the XML library.
	//xmlCleanupParser ();
	
	//// This is to debug memory for regression tests.
	//#if defined (DEBUG)
	//	xmlMemoryDump ();
	//#endif
	
	NSS_DEBUG ("Finilized [passwd] functionality\n");
	
	return NSS_STATUS_SUCCESS;
}

// Return next passwd entry.
// Unimplemeted.
enum nss_status _nss_exo_getpwent_r (struct passwd * result, char * buffer, size_t buffer_length, int * error)
{
	NSS_DEBUG ("Getting next [passwd] entry...\n");
	
	NSS_DEBUG ("Gotten next [passwd] entry\n");
	return NSS_STATUS_UNAVAIL;
}

// Get user by its name.
enum nss_status _nss_exo_getpwnam_r (const char * name, struct passwd * result, char * buffer, size_t buffer_size, int * error)
{
	NSS_DEBUG ("_nss_exo_getpwnam_r(): Getting user by name [%s]\n", name);
	
	//return NSS_STATUS_UNAVAIL;
	return user_get ("name", name, result, buffer, buffer_size, error);
}

// Get user by its ID.
enum nss_status _nss_exo_getpwuid_r (uid_t id, struct passwd * result, char * buffer, size_t buffer_size, int * error)
{
	NSS_DEBUG ("_nss_exo_getpwuid_r(): Getting user by ID [%i]\n", id);
	
	// Convert the number into text.
	// Include the terminating NULL character.
	char id_text [12];
	snprintf (id_text, 12, "%d", id);
	
	//return NSS_STATUS_UNAVAIL;
	return user_get ("id", id_text, result, buffer, buffer_size, error);
}
