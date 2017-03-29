#include "passwd.tool.h"

#include "../nss.exo.h"
#include "../tool.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include <zmq/zhelpers.h>

#include <nss.h>
#include <pwd.h>
#include <grp.h>


// Get user by either its ID or its name.
enum nss_status nss_exo_user_get (const char * getter_type, const char * getter_content, struct passwd * result, char * buffer, size_t buffer_size, int * error)
{
	//* error = 123;
	
	NSS_DEBUG ("nss_exo_user_get():: Called with buffer size of [%u]\n", buffer_size);
	NSS_DEBUG ("nss_exo_user_get():: Looking for user %s [%s]\n", getter_type, getter_content);
	
	//return NSS_STATUS_UNAVAIL;
	
	void * context_rx;
	void * context_tx;
	void * socket_rx;
	void * socket_tx;
	
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
	
	//char message [256];
	
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
	
	
	context_rx = zmq_ctx_new ();
	context_tx = zmq_ctx_new ();
	
	socket_rx = zmq_socket (context_rx, ZMQ_SUB);
	socket_tx = zmq_socket (context_tx, ZMQ_PUB);
	
	
	NSS_DEBUG ("nss_exo_user_get()::socket_rx.connect()");
	if (zmq_connect (socket_rx, "tcp://0.0.0.0:2131") != 0)
	{
		NSS_DEBUG ("nss_exo_user_get()::socket_rx.connect()::failure");
		zmq_close (socket_rx);
		zmq_close (socket_tx);
		zmq_ctx_destroy (context_rx);
		zmq_ctx_destroy (context_tx);
		
		return false;
	}
	NSS_DEBUG ("receive()::socket_rx.connect()::success");
	
	
	NSS_DEBUG ("nss_exo_user_get()::socket_tx.bind()");
	if (zmq_bind (socket_tx, "tcp://*:2132") != 0)
	{
		NSS_DEBUG ("nss_exo_user_get()::socket_tx.bind()::failure");
		zmq_close (socket_rx);
		zmq_close (socket_tx);
		zmq_ctx_destroy (context_rx);
		zmq_ctx_destroy (context_tx);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_user_get()::socket_tx.bind()::success");
	
	
	NSS_DEBUG ("nss_exo_user_get()::socket_rx.subscribe()");
	//if (zmq_setsockopt (nss_exo_passwd_socket_receiver, ZMQ_SUBSCRIBE, filter, strlen (filter)) != 0)
	if (zmq_setsockopt (socket_rx, ZMQ_SUBSCRIBE, "", 0) != 0)
	{
		NSS_DEBUG ("nss_exo_user_get()::socket_rx.subscribe():failure");
		zmq_close (socket_rx);
		zmq_close (socket_tx);
		zmq_ctx_destroy (context_rx);
		zmq_ctx_destroy (context_tx);
		
		return false;
	}
	NSS_DEBUG ("nss_exo_user_get()::socket_rx.subscribe()::success");
	
	zclock_sleep (500);
	
	// Send the request (in parts):
	NSS_DEBUG ("nss_exo_user_get()::transmit(get)\n");
	if (s_sendmore (socket_tx, "get") < 3)
	{
		NSS_DEBUG ("nss_exo_user_get()::transmit(get)::failed\n");
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_user_get()::transmit(get)::succeeded\n");
	
	NSS_DEBUG ("nss_exo_user_get()::transmit(user)\n");
	if (s_sendmore (socket_tx, "user") < 4)
	{
		NSS_DEBUG ("nss_exo_user_get()::transmit(user)::failed\n");
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_user_get()::transmit(user)::succeeded\n");
	
	NSS_DEBUG ("nss_exo_user_get()::transmit(getter_type)\n");
	if (s_sendmore (socket_tx, getter_type) < strlen (getter_type))
	{
		NSS_DEBUG ("nss_exo_user_get()::transmit(getter_type)::failed\n");
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_user_get()::transmit(getter_type)::succeeded\n");
	
	NSS_DEBUG ("nss_exo_user_get()::transmit(getter_content)\n");
	if (s_send (socket_tx, getter_content) < strlen (getter_type))
	{
		NSS_DEBUG ("nss_exo_user_get()::transmit(getter_content)::failed\n");
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_user_get()::transmit(getter_content)::succeeded\n");
	
	
	zclock_sleep (500);
	
	NSS_DEBUG ("nss_exo_user_get()::receive(success_text)\n");
	// Get the success status:
	success_text = s_recv (socket_rx);
	//NSS_DEBUG ("nss_exo_user_get()::receive(success_text)::succeeded\n");
	NSS_DEBUG ("nss_exo_user_get()::(success_text?=false)\n");
	if (strcmp (success_text, "0") == 0)
	{
		free (success_text);
		NSS_DEBUG ("nss_exo_user_get()::success_text==false\n");
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_user_get()::success_text==true\n");
	free (success_text);
	
	// Get the user details:
	NSS_DEBUG ("nss_exo_user_get()::receive()\n");
	id_text = s_recv (socket_rx);
	group_id_text = s_recv (socket_rx);
	name = s_recv (socket_rx);
	password = s_recv (socket_rx);
	shell = s_recv (socket_rx);
	home = s_recv (socket_rx);
	gecos = s_recv (socket_rx);
	
	
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
	
	
	zmq_close (socket_rx);
	zmq_close (socket_tx);
	zmq_ctx_destroy (context_rx);
	zmq_ctx_destroy (context_tx);
	
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
	
	NSS_DEBUG ("nss_exo_user_get()::return::success\n");
	
	return NSS_STATUS_SUCCESS;
}
