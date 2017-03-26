#include "main.h"
#include "tool.h"

#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
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
const bool transmit (const char * message)
{
	unsigned int message_length;
	unsigned int message_sent_length;
	void * context;
	void * socket;
	
	context = zmq_ctx_new ();
	socket = zmq_socket (context, ZMQ_PUB);
	
	if (zmq_bind (socket, "tcp://127.0.0.1:2131") != 0)
	{
		zmq_close (socket);
		zmq_ctx_destroy (context);
		
		return false;
	}
	
	message_length = strlen (message);
	
	message_sent_length = zmq_send (socket, message, message_length, 0);
	
	zmq_close (socket);
	zmq_ctx_destroy (context);
	
	return message_sent_length >= message_length;
}

//const bool receive (char * message, const char * uri)
const bool receive (char * message)
{
	char buffer [256];
	unsigned short int message_received_length;
	void * context;
	void * socket;
	
	context = zmq_ctx_new ();
	socket = zmq_socket (context, ZMQ_SUB);
	
	if (zmq_connect (socket, "tcp://127.0.0.1:2132") != 0)
	{
		zmq_close (socket);
		zmq_ctx_destroy (context);
		
		return false;
	}
	
	/*
	if (zmq_setsockopt (socket, ZMQ_SUBSCRIBE, filter, strlen (filter)) != 0)
	{
		zmq_close (socket);
		zmq_ctx_destroy (context);
		
		return false;
	}
	*/
	
	message_received_length = zmq_recv (socket, buffer, 255, 0);
	
	if (message_received_length < 0)
	{
			zmq_close (socket);
			zmq_ctx_destroy (context);
			
			return false;
	}
	
	buffer [message_received_length] = '\0';
	
	#if (defined (WIN32))
		message = strdup (buffer);
	#else
		message = strndup (buffer, sizeof (buffer) - 1);
	#endif
	
	// remember that the strdup family of functions use malloc/alloc for space for the new string.  It must be manually
	// freed when you are done with it.  Failure to do so will allow a heap attack.
	
	
	zmq_close (socket);
	zmq_ctx_destroy (context);
	
	return true;
}
