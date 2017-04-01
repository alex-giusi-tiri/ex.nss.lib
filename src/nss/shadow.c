#include "../nss.exo.h"
#include "../tool.h"

//#include <errno.h>
//#include <malloc.h>
//#include <string.h>
//#include <sys/types.h>
//#include <unistd.h>

#include <nss.h>
//#include <pwd.h>
//#include <grp.h>
#include <shadow.h>

#include <zmq.h>
#include <zmq/zhelpers.h>


// Get shadow information using username.
enum nss_status _nss_exo_getspnam_r (const char * name, struct spwd * result, char * buffer, size_t buffer_size, int * error)
{
	NSS_DEBUG ("_nss_exo_getspnam_r(): Called.\n");
	
	char * name_temp;
	char * password;
	long int change;
	long int minimum;
	long int maximum;
	long int warning;
	long int inactivation;
	long int expiration;
	unsigned long int flag;
	
	char * success_text;
	char * change_text;
	char * minimum_text;
	char * maximum_text;
	char * warning_text;
	char * inactivation_text;
	char * expiration_text;
	char * flag_text;
	
	void * context;
	void * socket;
	
	
	context = zmq_ctx_new ();
	socket = zmq_socket (context, ZMQ_DEALER);
	
	
	NSS_DEBUG ("_nss_exo_getspnam_r()::socket.connect()");
	if (zmq_connect (socket, "tcp://0.0.0.0:2131") != 0)
	{
		NSS_DEBUG ("_nss_exo_getspnam_r()::socket.connect()::failure");
		
		zmq_close (socket);
		zmq_ctx_destroy (context);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("_nss_exo_getspnam_r()::socket.connect()::success");
	
	
	zclock_sleep (200);
	
	// Send the request (in parts):
	NSS_DEBUG ("_nss_exo_getspnam_r()::tx(get)\n");
	if (s_sendmore (socket, "get") < 3)
	{
		NSS_DEBUG ("_nss_exo_getspnam_r()::tx(get)::failure\n");
		
		zmq_close (socket);
		zmq_ctx_destroy (context);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("_nss_exo_getspnam_r()::tx(get)::success\n");
	
	NSS_DEBUG ("_nss_exo_getspnam_r()::tx(shadow)\n");
	if (s_sendmore (socket, "shadow") < 6)
	{
		NSS_DEBUG ("_nss_exo_getspnam_r()::tx(shadow)::failure\n");
		
		zmq_close (socket);
		zmq_ctx_destroy (context);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("_nss_exo_getspnam_r()::tx(shadow)::success\n");
	
	NSS_DEBUG ("_nss_exo_getspnam_r()::tx(getter_type)\n");
	if (s_sendmore (socket, "name") < 4)
	{
		NSS_DEBUG ("_nss_exo_getspnam_r()::tx(getter_type)::failure\n");
		
		zmq_close (socket);
		zmq_ctx_destroy (context);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("_nss_exo_getspnam_r()::tx(getter_type)::success\n");
	
	NSS_DEBUG ("_nss_exo_getspnam_r()::tx(getter_content)\n");
	if (s_send (socket, name) < strlen (name))
	{
		NSS_DEBUG ("_nss_exo_getspnam_r()::tx(getter_content)::failure\n");
		
		zmq_close (socket);
		zmq_ctx_destroy (context);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("_nss_exo_getspnam_r()::tx(getter_content)::succeess\n");
	
	
	// Manage the response.
	NSS_DEBUG ("_nss_exo_getspnam_r()::rx(success_text)\n");
	// Get the success status:
	success_text = s_recv (socket);
	NSS_DEBUG ("_nss_exo_getspnam_r()::rx::(success_text?=false)\n");
	if (strcmp (success_text, "0") == 0)
	{
		NSS_DEBUG ("_nss_exo_getspnam_r()::rx::(success_text==false)\n");
		
		free (success_text);
		
		zmq_close (socket);
		zmq_ctx_destroy (context);
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("_nss_exo_getspnam_r()::rx::(success_text==true)\n");
	free (success_text);
	
	
	// Get the password details:
	NSS_DEBUG ("_nss_exo_getspnam_r()::rx()...\n");
	name_temp = s_recv (socket);
	password = s_recv (socket);
	change_text = s_recv (socket);
	minimum_text = s_recv (socket);
	maximum_text = s_recv (socket);
	warning_text = s_recv (socket);
	inactivation_text = s_recv (socket);
	expiration_text = s_recv (socket);
	flag_text = s_recv (socket);
	NSS_DEBUG ("_nss_exo_getspnam_r()::rx()::complete\n");
	
	// Convert the texts into numbers:
	change = strtol (change_text, NULL, 10);
	minimum = strtol (minimum_text, NULL, 10);
	maximum = strtol (maximum_text, NULL, 10);
	warning = strtol (warning_text, NULL, 10);
	inactivation = strtol (inactivation_text, NULL, 10);
	expiration = strtol (expiration_text, NULL, 10);
	//flag = strtoul (flag_text, NULL, 10);
	flag = strtol (flag_text, NULL, 10);
	
	// Free the texts:
	free (change_text);
	free (minimum_text);
	free (maximum_text);
	free (warning_text);
	free (inactivation_text);
	free (expiration_text);
	free (flag_text);
	
	//result -> sp_namp = strdup (name);
	result -> sp_namp = name_temp;
	result -> sp_pwdp = password;
	result -> sp_lstchg = change;
	result -> sp_min = minimum;
	result -> sp_max = maximum;
	result -> sp_warn = warning;
	result -> sp_inact = inactivation;
	result -> sp_expire = expiration;
	result -> sp_flag = flag;
	//NSS_DEBUG ("result -> sp_flag :: [%d]", result -> sp_flag);
	
	
	return NSS_STATUS_SUCCESS;
}
