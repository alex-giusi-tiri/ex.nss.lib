#include "default.h"
#include "tool.h"

#include <string.h>
//#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <grp.h>
#include <pwd.h>

// Get user by either its ID or its name.
enum nss_status user_get (const char * get_type, const char * get, struct passwd * user, char * buffer, size_t buffer_size, int * error)
{
	NSS_DEBUG ("user_get : Looking for the user \"%s\" (%s).\n", get, get_type);
	
	signed int result_error;
	uid_t * id;
	gid_t * group_id;
	xmlChar * name;
	//xmlChar * id_text;
	xmlChar * password;
	//const unsigned char * group_name;
	xmlChar * group_id_text;
	xmlChar * shell;
	xmlChar * home;
	xmlChar * gecos;
	
	FILE * file;
	
	// The parsed XML document tree.
	xmlDocPtr document;
	//xmlNodePtr root_element;
	// Reused variables for each user detail.
    //xmlXPathObjectPtr xpobj;	// XPath OBJect
    //const xmlNodeSetPtr nodes;
	//const xmlNodePtr node;
    
	//char line [PATH_MAX];
	//char line [INT_MAX / 8];
	unsigned char line [USHRT_MAX];
	char command [PATH_MAX] = "";
	//char command_output [ULLONG_MAX] = "";
	//char command_output [SHRT_MAX + 1] = "";
	unsigned char * command_output = malloc (sizeof (unsigned char) * (SHRT_MAX + 1));
	
	
	// executable . " user get " . argument_type . " " . argument
	// Argument count: 5
	strcat (command, executable);
	strcat (command, " user get ");
	strcat (command, get_type);
	strcat (command, " ");
	strcat (command, get);
	
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

	//while (fgets (path, sizeof (path)/*-1*/, fp) != NULL)
	//{
	//	//printf ("%s", path);
	//}
	
	//NSS_DEBUG ("PATH_MAX:%d\n", PATH_MAX);
	//NSS_DEBUG ("INT_MAX:%d\n", INT_MAX);
	
	//return NSS_STATUS_UNAVAIL;
	
	// Read the output one line at a time and output the line.
	while (fgets (line, USHRT_MAX - 1, file) != NULL)
	{
		//printf ("%s", line);
		NSS_DEBUG ("user_get : Read : \"%s\"", line);
		
		if (strlen (command_output) + strlen (line) + 1 > sizeof (command_output))
			command_output = realloc (command_output, sizeof (unsigned char) * sizeof (command_output) * 2);
		
		// Fill the "command_output".
		strcat (command_output, line);
	}
	
	//return NSS_STATUS_UNAVAIL;
	
	
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
		
		* error = errno;
		
		NSS_DEBUG ("user_get : Failed to close the executable : \"%s\".\n", command);
		
		return NSS_STATUS_UNAVAIL;
	}
	
	NSS_DEBUG ("user_get : Obtained command output : \"%s\".\n", command_output);
	
	//return NSS_STATUS_UNAVAIL;
	
	/*
		As the document is in memory, it does not have a base, according to RFC 2396;
		then, the "noname.xml" argument will serve as its base.
	*/
	document = xmlReadMemory (command_output, strlen (command_output), "noname.xml", NULL, 0);
	//document = xmlReadMemory (command_output, 477, "noname.xml", NULL, 0);
	//document = xmlReadMemory ("<?xml version=\"1.0\"?><doc/>", 27, "noname.xml", NULL, 0);
    
    // Uneeded anymore.
	//free (command_output);
	
	NSS_DEBUG ("user_get : Parsed the XML document.\n");
	
	return NSS_STATUS_UNAVAIL;
	
    if (document == NULL)
    {
		//fprintf (stderr, "Failed to parse document\n");
		NSS_DEBUG ("user_get : Failed to parse the XML document : \"%s\" (%d).\n", command_output, strlen (command_output));
		
		return NSS_STATUS_UNAVAIL;
    }
    
    // Get the root element.
    //root_element = xmlDocGetRootElement (doc);
    
	//NSS_DEBUG ("user_get : fgets (line, PATH_MAX, file) : \"%d\".", fgets (line, PATH_MAX, file));
	//NSS_DEBUG ("user_get : Retrieved ID as text : \"%s\"\n", line);
	
	//return NSS_STATUS_UNAVAIL;
	
	
	// Get the ID.
	id = (uid_t *) xml_xpath_evaluate_content_number (document, "/xml/user/id", 10, 0, 1);
	if (id == NULL)
	{
		//pclose (file);
		
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("user_get : Failed to obtain : ID.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("user_get : Obtained : ID : [%d].\n", * id);
	
	
	// Get the name.
	name = (xmlChar *) xml_xpath_evaluate_content (document, "/xml/user/name", 0, 1);
	if (name == NULL)
	{
		//pclose (file);
		
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("user_get : Failed to obtain : Name.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("user_get : Obtained : Name : [%s].\n", (const char *) name);
	
	
	// Get the password.
	password = (xmlChar *) xml_xpath_evaluate_content (document, "/xml/user/authentication/password/value", 0, 1);
	if (name == NULL)
	{
		//pclose (file);
		
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("user_get : Failed to obtain : Password.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("user_get : Obtained : Password : [%s].\n", (const char *) password);
	
	
	// Get the main group's ID.
	group_id = (gid_t *) xml_xpath_evaluate_content_number (document, "/xml/user/group/id", 10, 0, 1);
	if (group_id == NULL)
	{
		//pclose (file);
		
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("user_get : Faile to obtain : Group : ID.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("user_get : Obtained : Group : ID : [%d].\n", * group_id);
	
	
	// Get the shell.
	shell = (xmlChar *) xml_xpath_evaluate_content (document, "/xml/user/shell", 0, 1);
	if (name == NULL)
	{
		//pclose (file);
		
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("user_get : Failed to obtain : Shell.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("user_get : Obtained : Shell : [%s].\n", (const char *) shell);
	
	
	// Get the home path.
	home = (xmlChar *) xml_xpath_evaluate_content (document, "/xml/user/shell", 0, 1);
	if (name == NULL)
	{
		//pclose (file);
		
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("user_get : Failed to obtain : Home.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("user_get : Obtained : Home : [%s].\n", (const char *) home);
	

	// Get the GECOS field.
	gecos = (xmlChar *) xml_xpath_evaluate_content (document, "/xml/user/gecos", 0, 1);
	if (gecos == NULL)
	{
		//pclose (file);
		
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("user_get : Failed to obtain : GECOS field.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("user_get : Obtained : GECOS field : [%s].\n", (const char *) gecos);
	

	//name = sqlite3_column_text(pSt, 0);
	//gid = sqlite3_column_int(pSt, 1);
	////gid = sqlite3_column_text(pSt, 1);
	//shell = sqlite3_column_text(pSt, 2);
	//home = sqlite3_column_text(pSt, 3);
	
	xmlFreeDoc (document);
	
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
	
    if (strlen ((const char *) name) + 1 + strlen ((const char *) password) + 1 + strlen ((const char *) home) + 1 + strlen ((const char *) shell) + 1 + strlen ((const char *) gecos) + 1 > buffer_size)
    {
    	xmlFree (name);
    	xmlFree (password);
    	xmlFree (home);
    	xmlFree (shell);
    	xmlFree (gecos);
    	
		//* error = errno;
        * error = ERANGE;
		
		//NSS_DEBUG ("user_get : Failed to run the executable : \"%s\".\n", command);
		
		return NSS_STATUS_TRYAGAIN;
	}



	user -> pw_uid = * id;
	user -> pw_gid = * group_id;
	
	//user -> pw_name = (char *) name;
	//user -> pw_passwd = (char *) password;
	//user -> pw_gecos = (char *) gecos;
	//user -> pw_shell = (char *) shell;
	//user -> pw_dir = (char *) home;
	
	user -> pw_name = name;
	user -> pw_passwd = password;
	user -> pw_gecos = gecos;
	user -> pw_shell = shell;
	user -> pw_dir = home;
	
	free (id);
	free (group_id);
	//xmlFree (name);
	//xmlFree (password);
	//xmlFree (home);
	//xmlFree (shell);
	//xmlFree (gecos);
    
    
	* error = errno;
	
	return NSS_STATUS_SUCCESS;
}

// Setup everything needed to retrieve passwd entries.
enum nss_status _nss_sqlite_setpwent (void)
{
    NSS_DEBUG ("Initializing pw functions\n");
    
	/*
		this initialize the library and check potential ABI mismatches
		between the version it was compiled for and the actual shared
		library used.
	*/
    LIBXML_TEST_VERSION
    
    xmlInitParser ();
    
    return NSS_STATUS_SUCCESS;
}

// Free getpwent resources.
enum nss_status _nss_sqlite_endpwent (void)
{
    NSS_DEBUG ("Finishing pw functions\n");
    
    // Cleanup function for the XML library.
    xmlCleanupParser ();
    
    // This is to debug memory for regression tests.
    #if defined (DEBUG)
	    xmlMemoryDump ();
    #endif
    
    return NSS_STATUS_SUCCESS;
}

// Return next passwd entry.
// Not implemeted yet.
enum nss_status _nss_sqlite_getpwent_r (struct passwd * pwbuf, char * buf, size_t buflen, int * errnop)
{
    NSS_DEBUG ("Getting next pw entry\n");
    
    return NSS_STATUS_UNAVAIL;
}

// Get user by its name.
enum nss_status _nss_exo_getpwnam_r (const char * name, struct passwd * result, char * buffer, size_t buffer_size, int * error)
{
	return user_get ("name", name, result, buffer, buffer_size, error);
}

// Get user by its ID.
enum nss_status _nss_exo_getpwuid_r (uid_t id, struct passwd * result, char * buffer, size_t buffer_size, int * error)
{
	// Include the terminating NULL character (byte).
	char id_text [12];
	snprintf (id_text, 12, "%d", id);
	
	return user_get ("id", id_text, result, buffer, buffer_size, error);
}

