#include "default.h"
//#include "tool.h"

//#include <errno.h>
#include <grp.h>
#include <malloc.h>
#include <pwd.h>
#include <shadow.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * Get shadow information using username.
 */

enum nss_status _nss_exo_getspnam_r (const char * name, struct spwd * result, char * buffer, size_t buffer_size, int * error)
{
	//const xmlChar * name;
	xmlChar * password;
	//char * change_text;
	//char * minimum_text;
	//char * maximum_text;
	//char * warning_text;
	//char * inactivity_text;
	//char * expiration_text;
	//const unsigned char * flag_text;
	long int * change;
	long int * minimum;
	long int * maximum;
	long int * warning;
	long int * inactivation;
	long int * expiration;
	//unsigned long int * flag;
	char ** document_text;
	
	signed int result_error;
	
	//FILE * file;
	//char line [PATH_MAX];
	char command [PATH_MAX] = "";
	
	// The parsed XML document tree.
	xmlDocPtr document;
	
	// executable . " password get " . get_type . " " . get
	// Argument count: 5
	strcat (command, executable);
	strcat (command, " shadow get name ");
	strcat (command, name);
	///*
	// Open the command for reading.
	result_error = system_execute (command, document_text);
	
	if (result_error != 0)
	{
		NSS_DEBUG ("_nss_exo_getspnam_r : Failed to run the executable.\n");
		
		* error = result_error;
		
		return NSS_STATUS_UNAVAIL;
	}
	
	// At this point, * document_text should point to valid content.
	
	
	/*
		As the document is in memory, it does not have a base, according to RFC 2396;
		then, the "noname.xml" argument will serve as its base.
	*/
	//document = xmlReadMemory (command_output, strlen (command_output), "noname.xml", NULL, 0);
	document = xmlReadMemory (* document_text, strlen (* document_text), "noname.xml", NULL, 0);
	//document = xmlReadMemory (command_output, 477, "noname.xml", NULL, 0);
	//document = xmlReadMemory ("<?xml version=\"1.0\"?><doc/>", 27, "noname.xml", NULL, 0);
	
	free (document_text);
	
	//return NSS_STATUS_UNAVAIL;
	
	if (document == NULL)
	{
		//fprintf (stderr, "Failed to parse document\n");
		
		//free (document_text);
		
		//NSS_DEBUG ("user_get : Failed to parse the XML document : \"%s\" (%d).\n", command_output, strlen (command_output));
		//NSS_DEBUG ("user_get : Failed to parse the XML document : \"%s\" (%d).\n", document_text, strlen (document_text));
		NSS_DEBUG ("_nss_exo_getspnam_r : Failed to parse the XML document.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	
	
	/*
	// Get the name.
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		NSS_DEBUG ("getspnam_r : Failed to parse the executable's output : Name\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	name = strdup (line);
	*/
	
	
	
	// Get the password.
	password = xml_xpath_evaluate_content (document, "/xml/user/authentication/password/crypted/value", 0, false);
	if (password == NULL)
	{
		//pclose (file);
		
		//free (name);
		//free (id);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("_nss_exo_getspnam_r : Failed to obtain : Password.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("_nss_exo_getspnam_r : Obtained : Password : [%s].\n", password);
	
	
	// Get the time when the password was last changed.
	change = xml_xpath_evaluate_content_number (document, "/xml/user/authentication/password/time/change/previous", 0, false);
	if (change == NULL)
	{
		//pclose (file);
		
		//free (name);
		//free (id);
		xmlFree (password);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("_nss_exo_getspnam_r : Failed to obtain : Change.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("_nss_exo_getspnam_r : Obtained : Change : [%i].\n", * change);
	
	
	// Get the days until change allowed.
	minimum = xml_xpath_evaluate_content_number (document, "/xml/user/authentication/password/time/change/minimum", 0, false);
	if (minimum == NULL)
	{
		//pclose (file);
		
		//free (name);
		free (change);
		xmlFree (password);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("_nss_exo_getspnam_r : Failed to obtain : Minimum.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("_nss_exo_getspnam_r : Obtained : Minimum : [%i].\n", * minimum);
	
	
	// Get the days before change required.
	maximum = xml_xpath_evaluate_content_number (document, "/xml/user/authentication/password/time/change/maximum", 0, false);
	if (maximum == NULL)
	{
		//pclose (file);
		
		free (minimum);
		free (change);
		xmlFree (password);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("_nss_exo_getspnam_r : Failed to obtain : Maximum.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("_nss_exo_getspnam_r : Obtained : Maximum : [%i].\n", * maximum);
	

	// Get the days warning for expiration.
	warning = xml_xpath_evaluate_content_number (document, "/xml/user/authentication/password/time/expiration_warning", 0, false);
	if (warning == NULL)
	{
		//pclose (file);
		
		free (maximum);
		free (minimum);
		free (change);
		xmlFree (password);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("_nss_exo_getspnam_r : Failed to obtain : Warning.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("_nss_exo_getspnam_r : Obtained : Change : [%i].\n", * warning);
	
	
	// Get the number of days before account inactivates.
	inactivation = xml_xpath_evaluate_content_number (document, "/xml/user/authentication/password/time/inactivation", 0, false);
	if (inactivation == NULL)
	{
		//pclose (file);
		
		free (warning);
		free (maximum);
		free (minimum);
		free (change);
		xmlFree (password);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("_nss_exo_getspnam_r : Failed to obtain : Inactivation.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("_nss_exo_getspnam_r : Obtained : Inactivation : [%i].\n", * inactivation);
	
	
	// Get the time when account expires.
	expiration = xml_xpath_evaluate_content_number (document, "/xml/user/authentication/password/time/expiration", 0, false);
	if (expiration == NULL)
	{
		//pclose (file);
		
		free (inactivation);
		free (warning);
		free (maximum);
		free (minimum);
		free (change);
		xmlFree (password);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("_nss_exo_getspnam_r : Failed to obtain : Expiration.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("_nss_exo_getspnam_r : Obtained : Expiration : [%i].\n", * expiration);
	
	/*
	// Get the flag.
	flag = xml_xpath_evaluate_content_number (document, "/xml/user/authentication/password/time/expiration", 0, false);
	if (flag == NULL)
	{
		//pclose (file);
		
		free (expiration);
		free (inactivation);
		free (warning);
		free (maximum);
		free (minimum);
		free (change);
		xmlFree (password);
		xmlFreeDoc (document);
		
		* error = errno;
		
		NSS_DEBUG ("_nss_exo_getspnam_r : Failed to obtain : Expiration.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("_nss_exo_getspnam_r : Obtained : Expiration : [%i].\n", * expiration);
	*/
	
	xmlFreeDoc (document);
	
	//name = sqlite3_column_text(pSt, 0);
	//gid = sqlite3_column_int(pSt, 1);
	////gid = sqlite3_column_text(pSt, 1);
	//shell = sqlite3_column_text(pSt, 2);
	//home = sqlite3_column_text(pSt, 3);

	// Close executable's file pointer.
	//pclose (fp);
	// ...and get the error code of the executed command.
	//error = WEXITSTATUS (pclose (file));
	
    result -> sp_namp = strdup (name);
    result -> sp_pwdp = password;
    result -> sp_lstchg = * change;
    result -> sp_min = * minimum;
    result -> sp_max = * maximum;
    result -> sp_warn = * warning;
    result -> sp_inact = * inactivation;
    result -> sp_expire = * expiration;
    //result -> sp_flag = flag;
    
    /*
	free (change_text);
	free (minimum_text);
	free (maximum_text);
	free (warning_text);
	free (inactivity_text);
	free (expiration_text);
	//free (flag_text);
	*/
	
	free (expiration);
	free (inactivation);
	free (warning);
	free (maximum);
	free (minimum);
	free (change);
	//xmlFree (password);
	xmlFreeDoc (document);
	/*
	if (error != 0)
	{
		//free (id_text);
		//free (name);
		free (result -> sp_namp);
		free (password);
		
		return NSS_STATUS_UNAVAIL;
	}
	//else if (error != 0)
	//	return NSS_STATUS_FAILURE;
	*/
	
	return NSS_STATUS_SUCCESS;
	//*/
}


