#include "group.tool.h"

#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include <zmq.h>
#include <zmq/zhelpers.h>

#include <nss.h>
#include <grp.h>
#include <string.h>

#include "../nss.exo.h"
#include "../tool.h"


enum nss_status nss_exo_group_get (const char * getter_type, const char * getter_content, struct group * result, char * buffer, size_t buffer_size, int * error)
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
	char * success_text;
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
	
	void * context;
	void * socket;
	
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
	
	context = zmq_ctx_new ();
	
	socket = zmq_socket (context, ZMQ_DEALER);
	
	//zclock_sleep (200);
	
	NSS_DEBUG ("nss_exo_group_get()::socket.connect()");
	if (zmq_connect (socket, "tcp://0.0.0.0:2131") != 0)
	{
		NSS_DEBUG ("nss_exo_group_get()::socket.connect()::failure");
		
		zmq_close (socket);
		zmq_ctx_destroy (context);
		
		return false;
	}
	NSS_DEBUG ("receive()::socket.connect()::success");
	
	/*
	NSS_DEBUG ("nss_exo_group_get()::socket_tx.bind()");
	if (zmq_bind (socket_tx, "tcp://*:2132") != 0)
	{
		NSS_DEBUG ("nss_exo_group_get()::socket_tx.bind()::failure");
		zmq_close (socket_rx);
		zmq_close (socket_tx);
		zmq_ctx_destroy (context_rx);
		zmq_ctx_destroy (context_tx);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_group_get()::socket_tx.bind()::success");
	*/
	/*
	NSS_DEBUG ("nss_exo_group_get()::socket_rx.subscribe()");
	//if (zmq_setsockopt (nss_exo_passwd_socket_receiver, ZMQ_SUBSCRIBE, filter, strlen (filter)) != 0)
	if (zmq_setsockopt (socket_rx, ZMQ_SUBSCRIBE, "", 0) != 0)
	{
		NSS_DEBUG ("nss_exo_group_get()::socket_rx.subscribe():failure");
		zmq_close (socket_rx);
		zmq_close (socket_tx);
		zmq_ctx_destroy (context_rx);
		zmq_ctx_destroy (context_tx);
		
		return false;
	}
	NSS_DEBUG ("nss_exo_group_get()::socket_rx.subscribe()::success");
	*/
	zclock_sleep (200);
	
	//// Send the request:
	//if (!nss_exo_transmit (message))
	//	return NSS_STATUS_UNAVAIL;
	// Send the request (in parts):
	NSS_DEBUG ("nss_exo_group_get()::tx(get)\n");
	if (s_sendmore (socket, "get") < 3)
	{
		NSS_DEBUG ("nss_exo_group_get()::tx(get)::failed\n");
		
		zmq_close (socket);
		zmq_ctx_destroy (context);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_group_get()::tx(get)::succeeded\n");
	
	NSS_DEBUG ("nss_exo_group_get()::tx(group)\n");
	if (s_sendmore (socket, "group") < 5)
	{
		NSS_DEBUG ("nss_exo_group_get()::tx(group)::failed\n");
		
		zmq_close (socket);
		zmq_ctx_destroy (context);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_group_get()::tx(group)::succeeded\n");
	
	NSS_DEBUG ("nss_exo_group_get()::tx(getter_type)\n");
	if (s_sendmore (socket, getter_type) < strlen (getter_type))
	{
		NSS_DEBUG ("nss_exo_group_get()::tx(getter_type)::failed\n");
		
		zmq_close (socket);
		zmq_ctx_destroy (context);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_group_get()::tx(getter_type)::succeeded\n");
	
	NSS_DEBUG ("nss_exo_group_get()::tx(getter_content)\n");
	if (s_send (socket, getter_content) < strlen (getter_content))
	{
		NSS_DEBUG ("nss_exo_group_get()::tx(getter_content)::failed\n");
		
		zmq_close (socket);
		zmq_ctx_destroy (context);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_group_get()::tx(getter_content)::succeeded\n");
	
	
	NSS_DEBUG ("nss_exo_group_get()::receive(success_text)\n");
	// Get the success status:
	success_text = s_recv (socket);
	//NSS_DEBUG ("nss_exo_group_get()::receive(success_text)::succeeded\n");
	NSS_DEBUG ("nss_exo_group_get()::(success_text?=false)\n");
	if (strcmp (success_text, "0") == 0)
	{
		NSS_DEBUG ("nss_exo_group_get()::success_text==false\n");
		
		free (success_text);
		
		zmq_close (socket);
		zmq_ctx_destroy (context);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("nss_exo_group_get()::success_text==true\n");
	free (success_text);
	
	
	//zclock_sleep (200);
	
	
	id_text = s_recv (socket);
	name = s_recv (socket);
	password = s_recv (socket);
	
	
	// Convert the texts into numbers:
	id = strtoul (id_text, NULL, 10);
	free (id_text);
	
	result -> gr_gid = id;
	
	result -> gr_name = name;
	result -> gr_passwd = password;
	//result -> gr_mem = /*already set*/;
	
	//return NSS_STATUS_UNAVAIL;
	
	
	//free (group_id);
	//xmlFree (name);
	//xmlFree (password);
	//xmlFree (home);
	//xmlFree (shell);
	//xmlFree (gecos);
	
	zmq_close (socket);
	zmq_ctx_destroy (context);
	
	*error = errno;
	
	NSS_DEBUG ("group_get : Returning successfully\n");
	
	return NSS_STATUS_SUCCESS;
}
