#include "../nss.exo.h"
#include "../tool.h"
#include "passwd.tool.h"

//#include <string.h>
//#include <errno.h>
#include <unistd.h>
//#include <malloc.h>
#include <stdio.h>
#include <nss.h>
//#include <grp.h>
#include <pwd.h>

#include <zmq.h>

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
	
	/*
	if (!nss_exo_init ())
	{
		NSS_DEBUG ("Initialization of [passwd] functionality failed\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	*/
	
	NSS_DEBUG ("Initialized [passwd] functionality\n");
	
	return NSS_STATUS_SUCCESS;
}

// Finalize getpwent functionality.
enum nss_status _nss_exo_endpwent (void)
{
	NSS_DEBUG ("Finalizing [passwd] functionality...\n");
	
	//// Cleanup function for the XML library.
	//xmlCleanupParser ();
	
	//// This is to debug memory for regression tests.
	//#if defined (DEBUG)
	//	xmlMemoryDump ();
	//#endif
	
	/*
	if (!nss_exo_fin ())
	{
		NSS_DEBUG ("Finalization of [passwd] functionality failed\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	*/
	
	NSS_DEBUG ("Finalized [passwd] functionality\n");
	
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
	
	//if (!nss_exo_init ())
	//	return NSS_STATUS_UNAVAIL;
	
	return nss_exo_user_get ("name", name, result, buffer, buffer_size, error);
}

// Get user by its ID.
enum nss_status _nss_exo_getpwuid_r (uid_t id, struct passwd * result, char * buffer, size_t buffer_size, int * error)
{
	//NSS_DEBUG ("_nss_exo_getpwuid_r(): Getting user by ID [%i]\n", id);
	NSS_DEBUG ("_nss_exo_getpwuid_r(): Getting user by ID [%u]\n", id);
	
	//if (!nss_exo_init ())
	//	return NSS_STATUS_UNAVAIL;
	
	// Convert the number into text.
	// Include the terminating NULL character.
	char id_text [12];
	snprintf (id_text, 12, "%d", id);
	
	//return NSS_STATUS_UNAVAIL;
	return nss_exo_user_get ("id", id_text, result, buffer, buffer_size, error);
}
