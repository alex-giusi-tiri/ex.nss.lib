#include "passwd.tool.h"

#include "../nss.exo.h"
#include "../tool.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include <nss.h>
#include <pwd.h>
#include <grp.h>


// Get user by either its ID or its name.
enum nss_status nss_exo_tool_user_get (const char * getter_type, const char * getter_content, struct passwd * result, char * buffer, size_t buffer_size, int * error)
{
	//* error = 123;
	
	NSS_DEBUG ("nss_exo_tool_user_get():: Called with buffer size of [%u]\n", buffer_size);
	NSS_DEBUG ("nss_exo_tool_user_get():: Looking for user %s [%s]\n", getter_type, getter_content);
	
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
	NSS_DEBUG ("nss_exo_tool_user_get()::transmit(get)\n");
	if (!nss_exo_transmit ("get"))
	{
		NSS_DEBUG ("nss_exo_tool_user_get()::transmit(get)::failed\n");
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_tool_user_get()::transmit(get)::succeeded\n");
	
	NSS_DEBUG ("nss_exo_tool_user_get()::transmit(user)\n");
	if (!nss_exo_transmit ("user"))
	{
		NSS_DEBUG ("nss_exo_tool_user_get()::transmit(user)::failed\n");
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_tool_user_get()::transmit(user)::succeeded\n");
	
	NSS_DEBUG ("nss_exo_tool_user_get()::transmit(getter_type)\n");
	if (!nss_exo_transmit (getter_type))
	{
		NSS_DEBUG ("nss_exo_tool_user_get()::transmit(getter_type)::failed\n");
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_tool_user_get()::transmit(getter_type)::succeeded\n");
	
	NSS_DEBUG ("nss_exo_tool_user_get()::transmit(getter_content)\n");
	if (!nss_exo_transmit (getter_content))
	{
		NSS_DEBUG ("nss_exo_tool_user_get()::transmit(getter_content)::failed\n");
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_tool_user_get()::transmit(getter_content)::succeeded\n");
	
	
	NSS_DEBUG ("nss_exo_tool_user_get()::receive(success_text)\n");
	// Get the success status:
	if (!nss_exo_receive (success_text))
	{
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_tool_user_get()::receive(success_text)::succeeded\n");
	
	NSS_DEBUG ("nss_exo_tool_user_get()::(success_text?=false)\n");
	if (success_text == "0")
	{
		NSS_DEBUG ("nss_exo_tool_user_get()::success_text==false\n");
		free (success_text);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_tool_user_get()::success_text==true\n");
	free (success_text);
	
	// Get the user ID (as a character pointer):
	NSS_DEBUG ("nss_exo_tool_user_get()::receive(id_text)\n");
	if (!nss_exo_receive (id_text))
	{
		NSS_DEBUG ("nss_exo_tool_user_get()::receive(id_text)::failed\n");
		free (success_text);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_tool_user_get()::receive(id_text)::succeeded\n");
	
	// Get the primary group ID (as a character pointer):
	NSS_DEBUG ("nss_exo_tool_user_get()::receive(group_id_text)\n");
	if (!nss_exo_receive (group_id_text))
	{
		NSS_DEBUG ("nss_exo_tool_user_get()::receive(group_id_text)\n");
		//free (success_text);
		free (id_text);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_tool_user_get()::receive(grouup_id_text)\n");
	
	// Get the name:
	NSS_DEBUG ("nss_exo_tool_user_get()::receive(name)\n");
	if (!nss_exo_receive (name))
	{
		NSS_DEBUG ("nss_exo_tool_user_get()::receive(name)::failed\n");
		//free (success_text);
		free (id_text);
		free (group_id_text);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_tool_user_get()::receive(name)::succeeded\n");
	
	// Get the password:
	NSS_DEBUG ("nss_exo_tool_user_get()::receive(password)\n");
	if (!nss_exo_receive (password))
	{
		NSS_DEBUG ("nss_exo_tool_user_get()::receive(password)::failed\n");
		//free (success_text);
		free (id_text);
		free (group_id_text);
		free (name);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_tool_user_get()::receive(password)::succeeded\n");
	
	// Get the shell:
	NSS_DEBUG ("nss_exo_tool_user_get()::receive(shell)\n");
	if (!nss_exo_receive (shell))
	{
		NSS_DEBUG ("nss_exo_tool_user_get()::receive(shell)::failed\n");
		//free (success_text);
		free (id_text);
		free (group_id_text);
		free (name);
		free (password);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_tool_user_get()::receive(shell)::succeeded\n");
	
	NSS_DEBUG ("nss_exo_tool_user_get()::receive(home)\n");
	if (!nss_exo_receive (home))
	{
		NSS_DEBUG ("nss_exo_tool_user_get()::receive(home)::failed\n");
		//free (success_text);
		free (id_text);
		free (group_id_text);
		free (name);
		free (password);
		free (shell);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_tool_user_get()::receive(home)::succeeded\n");
	
	NSS_DEBUG ("nss_exo_tool_user_get()::receive(gecos)\n");
	if (!nss_exo_receive (gecos))
	{
		NSS_DEBUG ("nss_exo_tool_user_get()::receive(gecos)::failed\n");
		//free (success_text);
		free (id_text);
		free (group_id_text);
		free (name);
		free (password);
		free (shell);
		free (home);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_tool_user_get()::receive(gecos)::succeeded\n");
	
	
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
	
	//free (success_text);
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
	
	NSS_DEBUG ("nss_exo_tool_user_get()::return::success\n");
	
	return NSS_STATUS_SUCCESS;
}
