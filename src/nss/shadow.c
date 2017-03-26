#include "../main.h"
#include "../tool.h"

//#include <errno.h>
#include <grp.h>
#include <malloc.h>
#include <pwd.h>
#include <shadow.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

// Get shadow information using username.
enum nss_status _nss_exo_getspnam_r (const char * name, struct spwd * result, char * buffer, size_t buffer_size, int * error)
{
	NSS_DEBUG ("_nss_exo_getspnam_r() : Called.\n");
	
	return NSS_STATUS_UNAVAIL;
}
