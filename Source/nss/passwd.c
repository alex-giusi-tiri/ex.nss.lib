#include "../0.h"
#include "../tool.h"

#include <string.h>
//#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <grp.h>
#include <pwd.h>

// Get user by either its ID or its name.
enum nss_status user_get (const char * get_type, const char * get, struct passwd * result, char * buffer, size_t buffer_size, int * error)
{
	//* error = 123;
	
	NSS_DEBUG ("user_get : Called with buffer size of %u.\n", buffer_size);
	NSS_DEBUG ("user_get : Looking for the user %s \"%s\".\n", get_type, get);
	
	//return NSS_STATUS_UNAVAIL;
	
	signed int result_error;
	unsigned int required_buffer_size;
	//unsigned long long int command_output_size = sizeof (char) * (SHRT_MAX + 1);
	//size_t command_output_size = sizeof (char) * 8;
	uid_t * id;
	gid_t * group_id;
	xmlChar * name;
	//xmlChar * id_text;
	xmlChar * password;
	//const unsigned char * group_name;
	//xmlChar * group_id_text;
	xmlChar * shell;
	xmlChar * home;
	xmlChar * gecos;
	
	//FILE * file;
	
	// The parsed XML document tree.
	xmlDocPtr document;
	//xmlNodePtr root_element;
	// Reused variables for each user detail.
	//xmlXPathObjectPtr xpobj;	// XPath OBJect
	//const xmlNodeSetPtr nodes;
	//const xmlNodePtr node;
	
	// char line [PATH_MAX];
	//char line [INT_MAX / 8];
	//char line [USHRT_MAX];
	//char line [SSIZE_MAX];
	char command [PATH_MAX] = "";
	//char command_output [ULLONG_MAX] = "";
	//char command_output [SHRT_MAX + 1] = "";
	char * document_text;
	
	// executable . " user get " . argument_type . " " . argument
	// Argument count: 5
	strcat (command, executable);
	strcat (command, " passwd get ");
	strcat (command, get_type);
	strcat (command, " '");
	strcat (command, get);
	strcat (command, "'");
	/*
	NSS_DEBUG ("user_get : Opening command... : \"%s\"", command);
	// Open the command for reading.
	file = popen (command, "r");
	
	NSS_DEBUG ("user_get : Reading...");
	
	//NSS_DEBUG ("user_get : fgets (line, PATH_MAX, file) : \"%d\".", fgets (line, PATH_MAX, file));
	//NSS_DEBUG ("user_get : Retrieved line : \"%s\"\n", line);
	
	// Read the output one line at a time and output the line.
	//while (fgets (line, PATH_MAX, file) != NULL)
	//{
		//printf ("%s", line);
	//	NSS_DEBUG ("user_get : Read : \"%s\"", line);
	//}
	
	//NSS_DEBUG ("user_get : Closing command...");
	//pclose (file);
	
	//NSS_DEBUG ("user_get : Returning...");
	//return NSS_STATUS_UNAVAIL;
	
	if (file == NULL)
	{
		NSS_DEBUG ("user_get : Failed to run the executable : \"%s\".\n", command);
		
		* error = errno;
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("user_get : Succeeded in running the executable : \"%s\".\n", command);
	
	//while (fgets (path, sizeof (path), fp) != NULL)
	//{
	//	//printf ("%s", path);
	//}
	
	//NSS_DEBUG ("PATH_MAX:%d\n", PATH_MAX);
	//NSS_DEBUG ("INT_MAX:%d\n", INT_MAX);
	
	//return NSS_STATUS_UNAVAIL;
	
	command_output = malloc (command_output_size);
	strcpy (command_output, "");
	//* command_output = "";
	//command_output [0] = '\0';
	
	
	//strcat (command_output, "ABC");
	
	//free (document_text);
	//free (command_output);
	//command_output = NULL;
	
	//return NSS_STATUS_UNAVAIL;
	
	//NSS_DEBUG ("user_get : (sizeof (char) * (SHRT_MAX + 1)) : \"%d\"", (sizeof (char) * (SHRT_MAX + 1)));
	
	
	//
	// Loops here.
	//
	// Read the output one line at a time and save the line.
	while (fgets (line, PATH_MAX, file) != NULL)
	{
		//printf ("%s", line);
		NSS_DEBUG ("user_get : Read : \"%s\"", line);
		
		//NSS_DEBUG ("user_get : strlen (command_output) + strlen (line) + 1 : \"%d\"", strlen (command_output) + strlen (line) + 1);
		//NSS_DEBUG ("user_get : sizeof (command_output) : \"%d\"", sizeof (command_output));
		//NSS_DEBUG ("user_get : sizeof (command_output) / sizeof (command_output [0]) : \"%d\"", sizeof (command_output) / sizeof (command_output [0]));
		
		
		if (strlen (command_output) + strlen (line) + 1 > command_output_size)
		{
			command_output_size *= 2;
			command_output_temp = realloc (command_output, sizeof (char) * command_output_size);
			
			if (command_output_temp == NULL)
			{
				pclose (file);
				
				free (command_output);
				
				NSS_DEBUG ("user_get : \"realloc ()\" failed.\n");
				
				return NSS_STATUS_UNAVAIL;
			}
			
			command_output = command_output_temp;
			
			//NSS_DEBUG ("user_get : sizeof (char) * sizeof (command_output) * 2) : size : new \"%d\"", sizeof (char) * sizeof (command_output) * 2);
			//NSS_DEBUG ("user_get : sizeof (command_output) : size : post \"%d\"", sizeof (command_output));
		}
		
		// Fill "command_output".
		strcat (command_output, line);
	}
	
	document_text = strdup (command_output);
	*/
	
	//document_text = malloc (sizeof (char *));
	
	result_error = execute (command, & document_text);
	
	if (result_error != 0)
	{
		NSS_DEBUG ("user_get : Failed to run the command \"%s\".\n", command);
		
		* error = result_error;
		
		return NSS_STATUS_UNAVAIL;
	}
	
	
	//return 1;
	
	// At this point, document_text should point to valid content.
	
	
	//return NSS_STATUS_UNAVAIL;
	
	// Uneeded anymore.
	// free (command_output);
	//free (document_text);
	//command_output = NULL;
	
	//return NSS_STATUS_UNAVAIL;
	
	/*
	result_error = WEXITSTATUS (pclose (file));
	if (result_error != 0)
	{
		
		free (document_text);
		
		* error = errno;
		
		NSS_DEBUG ("user_get : The executable did not run properly : the command was \"%s\"; the error was [%i].\n", command, result_error);
		
		return NSS_STATUS_UNAVAIL;
	}
	*/
	NSS_DEBUG ("user_get : Obtained command output : \"%s\".\n", /*command_output*/document_text);
	
	//printf ("user_get : Obtained command output [24] : \"%c\".\n", command_output [24]);
	
	//return NSS_STATUS_UNAVAIL;
	/*
	if (document_text == NULL)
	{
		NSS_DEBUG ("user_get : Failed to duplicate the XML document.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	*/
	/*
		As the document is in memory, it does not have a base, according to RFC 2396;
		then, the "noname.xml" argument will serve as its base.
	*/
	
	NSS_DEBUG ("nss-exo : user_get : Command output : \"%s\".\n", document_text);
	
	//document = xmlReadMemory (command_output, strlen (command_output), "noname.xml", NULL, 0);
	document = xmlReadMemory (document_text, strlen (document_text), "noname.xml", NULL, 0);
	//document = xmlReadMemory (command_output, 477, "noname.xml", NULL, 0);
	//document = xmlReadMemory ("<?xml version=\"1.0\"?><doc/>", 27, "noname.xml", NULL, 0);
	
	// Uneeded anymore.
	//free (* document_text);
	free (document_text);
	
	//return NSS_STATUS_UNAVAIL;
	
	if (document == NULL)
	{
		//fprintf (stderr, "Failed to parse document\n");
		
		//free (document_text);
		
		//NSS_DEBUG ("user_get : Failed to parse the XML document : \"%s\" (%d).\n", command_output, strlen (command_output));
		//NSS_DEBUG ("user_get : Failed to parse the XML document : \"%s\" (%d).\n", document_text, strlen (document_text));
		NSS_DEBUG ("user_get : Failed to parse the XML document.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	
	//xml_xpath_evaluate_content_number (document, "/xml/user/id", 10, 0, 1);
	
	NSS_DEBUG ("user_get : Parsed the XML document.\n");
	
	// Get the root element.
	//root_element = xmlDocGetRootElement (doc);
	
	//NSS_DEBUG ("user_get : fgets (line, PATH_MAX, file) : \"%d\".", fgets (line, PATH_MAX, file));
	//NSS_DEBUG ("user_get : Retrieved ID as text : \"%s\"\n", line);
	//xml_xpath_evaluate_content_number (/*document*/NULL, "/xml/user/id", 10, 0, 1);
	//return NSS_STATUS_UNAVAIL;
	
	// Get the ID.
	id = (uid_t *) xml_xpath_evaluate_content_number (document, "/xml/user/id"/*, 10*/, 0, false);
	if (id == NULL)
	{
		//pclose (file);
		
		NSS_DEBUG ("user_get : \"document\" [%d].\n", document);
		//if (document == NULL)
		//	NSS_DEBUG ("user_get : \"document\" is null.\n");
		
		//free (document_text);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("user_get : Failed to obtain : ID.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("user_get : Obtained : ID : [%d].\n", * id);
	
	//return NSS_STATUS_UNAVAIL;
	
	
	// Get the name.
	name = xml_xpath_evaluate_content (document, "/xml/user/name", 0, false);
	if (name == NULL)
	{
		//pclose (file);
		
		free (id);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("user_get : Failed to obtain : Name.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("user_get : Obtained : Name : [%s].\n", name);
	
	//return NSS_STATUS_UNAVAIL;
	
	
	// Get the password.
	password = xml_xpath_evaluate_content (document, "/xml/user/authentication/password/value", 0, false);
	if (password == NULL)
	{
		//pclose (file);
		
		free (name);
		free (id);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("user_get : Failed to obtain : Password.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("user_get : Obtained : Password : [%s].\n", password);
	
	
	// Get the main group's ID.
	group_id = (gid_t *) xml_xpath_evaluate_content_number (document, "/xml/user/group/id"/*, 10*/, 0, false);
	if (group_id == NULL)
	{
		//pclose (file);
		
		free (password);
		free (name);
		free (id);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("user_get : Failed to obtain : Group : ID.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("user_get : Obtained : Group : ID : [%d].\n", * group_id);
	
	
	// Get the shell.
	shell = xml_xpath_evaluate_content (document, "/xml/user/shell", 0, false);
	if (shell == NULL)
	{
		//pclose (file);
		
		free (group_id);
		free (password);
		free (name);
		free (id);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("user_get : Failed to obtain : Shell.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("user_get : Obtained : Shell : [%s].\n", shell);
	
	
	// Get the home path.
	home = xml_xpath_evaluate_content (document, "/xml/user/home", 0, false);
	if (home == NULL)
	{
		//pclose (file);
		
		free (shell);
		free (group_id);
		free (password);
		free (name);
		free (id);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("user_get : Failed to obtain : Home.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("user_get : Obtained : Home : [%s].\n", home);
	
	
	// Get the GECOS field.
	gecos = xml_xpath_evaluate_content (document, "/xml/user/gecos", 0, false);
	if (gecos == NULL)
	{
		//pclose (file);
		
		free (home);
		free (group_id);
		free (password);
		free (name);
		free (id);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("user_get : Failed to obtain : GECOS field.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("user_get : Obtained : GECOS field : [%s].\n", gecos);
	
	//
	// Loops here?
	//
	
	//name = sqlite3_column_text(pSt, 0);
	//gid = sqlite3_column_int(pSt, 1);
	////gid = sqlite3_column_text(pSt, 1);
	//shell = sqlite3_column_text(pSt, 2);
	//home = sqlite3_column_text(pSt, 3);
	
	xmlFreeDoc (document);
	NSS_DEBUG ("user_get : Freed XML document.\n");
	
	// Close executable's file pointer.
	//pclose (fp);
	// ...and get the error code of the executed command.
	/*
	result_error = WEXITSTATUS (pclose (file));
	if (result_error != 0)
	{
		free (id_text);
		free (name);
		free (password);
		free (group_id_text);
		free (shell);
		free (home);
		free (gecos);
	
		* error = errno;
		
		return NSS_STATUS_UNAVAIL;
	}
	*/
	//else if (error != 0)
	//	return NSS_STATUS_FAILURE;
	
	required_buffer_size = strlen (name) + 1 + strlen (password) + 1 + strlen (home) + 1 + strlen (shell) + 1 + strlen (gecos) + 1;
	//if (strlen (name) + 1 + strlen (password) + 1 + strlen (home) + 1 + strlen (shell) + 1 + strlen (gecos) + 1 > buffer_size)
	if (required_buffer_size > buffer_size)
	{
		xmlFree (name);
		xmlFree (password);
		xmlFree (home);
		xmlFree (shell);
		xmlFree (gecos);
		
		//* error = errno;
		* error = ERANGE;
		
		NSS_DEBUG ("user_get : Buffer size exceeded : %i.\n", required_buffer_size);
		
		NSS_DEBUG ("user_get : Returning retrial.\n");
		
		return NSS_STATUS_TRYAGAIN;
	}
	
	
	//printf ("%i = %i\n", result -> pw_uid, * id);
	result -> pw_uid = * id;
	//printf ("%i = %i\n", result -> pw_uid, * id);
	//return 1;
	
	result -> pw_gid = * group_id;
	
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
	
	free (id);
	free (group_id);
	//xmlFree (name);
	//xmlFree (password);
	//xmlFree (home);
	//xmlFree (shell);
	//xmlFree (gecos);
	
	
	* error = errno;
	
	NSS_DEBUG ("user_get : Returning successfully.\n");
	
	return NSS_STATUS_SUCCESS;
}

// Initialize getpwent functionality:
// Setup everything needed to retrieve passwd entries.
enum nss_status _nss_exo_setpwent (void)
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

// Finalize getpwent functionality.
enum nss_status _nss_exo_endpwent (void)
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

// Return next passwd entry.
// Unimplemeted.
enum nss_status _nss_exo_getpwent_r (struct passwd * result, char * buffer, size_t buffer_length, int * error)
{
	NSS_DEBUG ("Getting next \"passwd\" entry.\n");
	
	return NSS_STATUS_UNAVAIL;
}

// Get user by its name.
enum nss_status _nss_exo_getpwnam_r (const char * name, struct passwd * result, char * buffer, size_t buffer_size, int * error)
{
	NSS_DEBUG ("_nss_exo_getpwnam_r : Getting user by its name \"%s\".\n", name);
	
	//return NSS_STATUS_UNAVAIL;
	return user_get ("name", name, result, buffer, buffer_size, error);
}

// Get user by its ID.
enum nss_status _nss_exo_getpwuid_r (uid_t id, struct passwd * result, char * buffer, size_t buffer_size, int * error)
{
	NSS_DEBUG ("_nss_exo_getpwuid_r : Getting user by its ID %i.\n", id);
	
	// Include the terminating NULL character.
	char id_text [12];
	snprintf (id_text, 12, "%d", id);
	
	//return NSS_STATUS_UNAVAIL;
	return user_get ("id", id_text, result, buffer, buffer_size, error);
}
