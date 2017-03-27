#include "group.tool.h"

#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include <nss.h>
#include <grp.h>
#include <string.h>

#include "../nss.exo.h"
#include "../tool.h"


enum nss_status nss_exo_tool_group_get (const char * getter_type, const char * getter_content, struct group * result, char * buffer, size_t buffer_size, int * error)
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
