//#ifndef nss_exo_tool_c
//#define nss_exo_tool_c

#include "nss.exo.h"
#include "tool.h"

#include <stdbool.h>
#include <string.h>
//#include <malloc.h>
//#include <errno.h>
/*
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <sqlite3.h>
*/
#include <zmq.h>

/*
void fn_tst (void)
{
	NSS_DEBUG ("fn_tst\n");
}
*/

//const bool transmit (const char * message, const char * uri)
const bool nss_exo_transmit (const char * message)
{
	unsigned int message_length;
	unsigned int message_sent_length;
	//void * context;
	//void * socket;
	
	//context = zmq_ctx_new ();
	//socket = zmq_socket (context, ZMQ_PUB);
	/*
	NSS_DEBUG ("transmit()::socket.bind()");
	if (zmq_bind (nss_exo_passwd_socket_transmitter, "tcp://*:2132") != 0)
	{
		NSS_DEBUG ("transmit()::socket.bind()::failed");
		//zmq_close (socket);
		//zmq_ctx_destroy (context);
		
		return false;
	}
	NSS_DEBUG ("transmit()::socket.bind()::succeeded");
	*/
	message_length = strlen (message);
	
	message_sent_length = zmq_send (nss_exo_passwd_socket_transmitter, message, message_length, 0);
	
	//zmq_close (socket);
	//zmq_ctx_destroy (context);
	
	return message_sent_length >= message_length;
}

//const bool receive (char * message, const char * uri)
const bool nss_exo_receive (char * message)
{
	char buffer [256];
	unsigned short int message_received_length;
	//void * context;
	//void * socket;
	
	//context = zmq_ctx_new ();
	//socket = zmq_socket (context, ZMQ_SUB);
	/*
	NSS_DEBUG ("receive()::socket.connect()");
	if (zmq_connect (nss_exo_passwd_socket_receiver, "tcp://0.0.0.0:2131") != 0)
	{
		NSS_DEBUG ("receive()::socket.connect()::failed");
		//zmq_close (socket);
		//zmq_ctx_destroy (context);
		
		return false;
	}
	NSS_DEBUG ("receive()::socket.connect()::succeeded");
	*/
	/*
	if (zmq_setsockopt (socket, ZMQ_SUBSCRIBE, filter, strlen (filter)) != 0)
	{
		zmq_close (socket);
		zmq_ctx_destroy (context);
		
		return false;
	}
	*/
	
	NSS_DEBUG ("receive()::zmq.recv()");
	message_received_length = zmq_recv (nss_exo_passwd_socket_receiver, buffer, 255, 0);
	
	if (message_received_length < 0)
	{
		NSS_DEBUG ("receive()::zmq.recv()::failed");
		//zmq_close (socket);
		//zmq_ctx_destroy (context);
		
		return false;
	}
	NSS_DEBUG ("receive()::zmq.recv()::succeeded");
	
	buffer [message_received_length] = '\0';
	
	#if (defined (WIN32))
		message = strdup (buffer);
	#else
		message = strndup (buffer, sizeof (buffer) - 1);
	#endif
	
	// remember that the strdup family of functions use malloc/alloc for space for the new string.  It must be manually
	// freed when you are done with it.  Failure to do so will allow a heap attack.
	
	
	//zmq_close (socket);
	//zmq_ctx_destroy (context);
	
	return true;
}

const bool nss_exo_fin (void)
{
	NSS_DEBUG ("Finilizing [passwd] functionality...\n");
	
	//// Cleanup function for the XML library.
	//xmlCleanupParser ();
	
	//// This is to debug memory for regression tests.
	//#if defined (DEBUG)
	//	xmlMemoryDump ();
	//#endif
	
	if (nss_exo_passwd_socket_receiver != NULL)
		zmq_close (nss_exo_passwd_socket_receiver);
	if (nss_exo_passwd_socket_transmitter != NULL)
		zmq_close (nss_exo_passwd_socket_transmitter);
	
	if (nss_exo_passwd_context_receiver != NULL)
		zmq_ctx_destroy (nss_exo_passwd_context_receiver);
	if (nss_exo_passwd_context_transmitter != NULL)
		zmq_ctx_destroy (nss_exo_passwd_context_transmitter);
	
	
	NSS_DEBUG ("Finilized [passwd] functionality\n");
	
	return true;
}

const bool nss_exo_init (void)
{
	NSS_DEBUG ("Initializing [passwd] functionality...\n");
	
	/*
		This initialize the library and check potential ABI mismatches
		between the version it was compiled for and the actual shared
		library used.
	*/
	//LIBXML_TEST_VERSION
	
	//xmlInitParser ();
	
	if (nss_exo_passwd_context_receiver == NULL)
		nss_exo_passwd_context_receiver = zmq_ctx_new ();
	if (nss_exo_passwd_context_transmitter == NULL)
		nss_exo_passwd_context_transmitter = zmq_ctx_new ();
	
	if (nss_exo_passwd_socket_receiver == NULL)
		nss_exo_passwd_socket_receiver = zmq_socket (nss_exo_passwd_context_receiver, ZMQ_SUB);
	if (nss_exo_passwd_socket_transmitter == NULL)
		nss_exo_passwd_socket_transmitter = zmq_socket (nss_exo_passwd_context_transmitter, ZMQ_PUB);
	
	
	NSS_DEBUG ("init()::socket.rx.connect()");
	if (zmq_connect (nss_exo_passwd_socket_receiver, "tcp://0.0.0.0:2131") != 0)
	{
		NSS_DEBUG ("init()::socket.rx.connect()::failed");
		
		zmq_close (nss_exo_passwd_socket_receiver);
		zmq_close (nss_exo_passwd_socket_transmitter);
	
		zmq_ctx_destroy (nss_exo_passwd_context_receiver);
		zmq_ctx_destroy (nss_exo_passwd_context_transmitter);
		
		
		nss_exo_passwd_context_receiver = NULL;
		nss_exo_passwd_context_transmitter = NULL;
		
		nss_exo_passwd_socket_receiver = NULL;
		nss_exo_passwd_socket_transmitter = NULL;
		
		
		return false;
	}
	NSS_DEBUG ("init()::socket.rx.connect()::succeeded");
	
	NSS_DEBUG ("init()::socket.tx.bind()");
	if (zmq_bind (nss_exo_passwd_socket_transmitter, "tcp://*:2132") != 0)
	{
		NSS_DEBUG ("init()::socket.tx.bind()::failed");
		
		zmq_close (nss_exo_passwd_socket_receiver);
		zmq_close (nss_exo_passwd_socket_transmitter);
	
		zmq_ctx_destroy (nss_exo_passwd_context_receiver);
		zmq_ctx_destroy (nss_exo_passwd_context_transmitter);
		
		
		nss_exo_passwd_context_receiver = NULL;
		nss_exo_passwd_context_transmitter = NULL;
		
		nss_exo_passwd_socket_receiver = NULL;
		nss_exo_passwd_socket_transmitter = NULL;
		
		
		return false;
	}
	NSS_DEBUG ("init()::socket.tx.bind()::succeeded");
	
	
	NSS_DEBUG ("init()::socket.rx.subscribe()");
	if (zmq_setsockopt (nss_exo_passwd_socket_receiver, ZMQ_SUBSCRIBE, "", 0) != 0)
	{
		NSS_DEBUG ("init()::socket.rx.subscribe()::failed");
		
		return false;
	}
	NSS_DEBUG ("init()::socket.rx.subscribe()::succeeded");
	
	
	NSS_DEBUG ("Initialized [passwd] functionality\n");
	
	return true;
}

//#endif
