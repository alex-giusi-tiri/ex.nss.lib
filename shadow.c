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
	//const unsigned char * name;
	char * password;
	char * change_text;
	char * minimum_text;
	char * maximum_text;
	char * warning_text;
	char * inactivity_text;
	char * expiration_text;
	//const unsigned char * flag_text;
	signed long int change;
	signed long int minimum;
	signed long int maximum;
	signed long int warning;
	signed long int inactivity;
	signed long int expiration;
	//unsigned long int flag;
	
	FILE * file;
	char line [PATH_MAX];
	char command [PATH_MAX] = "";
	
	
	// executable . " password get " . get_type . " " . get
	// Argument count: 5
	strcat (command, executable);
	strcat (command, " password get name ");
	strcat (command, name);
	
	// Open the command for reading.
	file = popen (command, "r");

	if (file == NULL)
	{
		NSS_DEBUG ("getspnam_r : Failed to run the executable.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	
	//while (fgets (path, sizeof (path)/*-1*/, fp) != NULL)
	//{
	//	//printf ("%s", path);
	//}
	
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
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		//free (name);
		
		NSS_DEBUG ("getspnam_r : Failed to parse the executable's output : Password\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	password = strdup (line);
	password [strcspn (password, "\r\n")] = NULL;
	
	
	// Get the time when the password was last changed.
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		//free (name);
		free (password);
		
		NSS_DEBUG ("getspnam_r : Failed to parse the executable's output : Group\n");
	
		return NSS_STATUS_UNAVAIL;
	}
	change_text = strdup (line);
	change_text [strcspn (change_text, "\r\n")] = NULL;
	errno = 0;
	change = strtol (change_text, NULL, 10);
	if (errno != 0)
	{
		pclose (file);
		
		//free (name);
		free (password);
		free (change_text);
		
		NSS_DEBUG ("getspnam_r : Failed to parse number.n");
	
		return NSS_STATUS_UNAVAIL;
	}
	
	
	// Get the days until changed allowed.
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		//free (name);
		free (password);
		free (change_text);
		
		NSS_DEBUG ("getspnam_r : Failed to parse the executable's output : Group\n");
	
		return NSS_STATUS_UNAVAIL;
	}
	minimum_text = strdup (line);
	minimum_text [strcspn (minimum_text, "\r\n")] = NULL;
	errno = 0;
	minimum = strtol (minimum_text, NULL, 10);
	if (errno != 0)
	{
		pclose (file);
		
		//free (name);
		free (password);
		free (change_text);
		free (minimum_text);
		
		NSS_DEBUG ("getspnam_r : Failed to parse number.n");
	
		return NSS_STATUS_UNAVAIL;
	}
	
	
	// Get the days before change required.
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		//free (name);
		free (password);
		free (change_text);
		free (minimum_text);
		
		NSS_DEBUG ("getspnam_r : Failed to parse the executable's output : Shell\n");
	
		return NSS_STATUS_UNAVAIL;
	}
	maximum_text = strdup (line);
	maximum_text [strcspn (maximum_text, "\r\n")] = NULL;
	errno = 0;
	maximum = strtol (maximum_text, NULL, 10);
	if (errno != 0)
	{
		pclose (file);
		
		//free (name);
		free (password);
		free (change_text);
		free (minimum_text);
		free (maximum_text);
		
		NSS_DEBUG ("getspnam_r : Failed to parse number.n");
	
		return NSS_STATUS_UNAVAIL;
	}
	

	// Get the days warning for expiration.
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		//free (name);
		free (password);
		free (change_text);
		free (minimum_text);
		free (maximum_text);
		
		NSS_DEBUG ("getspnam_r : Failed to parse the executable's output : Home\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	warning_text = strdup (line);
	warning_text [strcspn (warning_text, "\r\n")] = NULL;
	errno = 0;
	warning = strtol (warning_text, NULL, 10);
	if (errno != 0)
	{
		pclose (file);
		
		//free (name);
		free (password);
		free (change_text);
		free (minimum_text);
		free (maximum_text);
		free (warning_text);
		
		NSS_DEBUG ("getspnam_r : Failed to parse number.n");
	
		return NSS_STATUS_UNAVAIL;
	}
	

	// Get the number of days before account inactivation.
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		//free (name);
		free (password);
		free (change_text);
		//free (shell);
		free (minimum_text);
		free (maximum_text);
		free (warning_text);
		
		NSS_DEBUG ("getspnam_r : Failed to parse the executable's output : GECOS\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	inactivity_text = strdup (line);
	inactivity_text [strcspn (inactivity_text, "\r\n")] = NULL;
	errno = 0;
	inactivity = strtol (inactivity_text, NULL, 10);
	if (errno != 0)
	{
		pclose (file);
		
		//free (name);
		free (password);
		free (change_text);
		free (minimum_text);
		free (maximum_text);
		free (warning_text);
		free (inactivity_text);
		
		NSS_DEBUG ("getspnam_r : Failed to parse number.n");
	
		return NSS_STATUS_UNAVAIL;
	}
	
	
	// Get the time when account expires.
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		//free (name);
		free (password);
		free (change_text);
		//free (shell);
		free (minimum_text);
		free (maximum_text);
		free (warning_text);
		free (inactivity_text);
		
		NSS_DEBUG ("getspnam_r : Failed to parse the executable's output : GECOS\n");
	
		return NSS_STATUS_UNAVAIL;
	}
	expiration_text = strdup (line);
	expiration_text [strcspn (expiration_text, "\r\n")] = NULL;
	errno = 0;
	expiration = strtol (expiration_text, NULL, 10);
	if (errno != 0)
	{
		pclose (file);
		
		//free (name);
		free (password);
		free (change_text);
		free (minimum_text);
		free (maximum_text);
		free (warning_text);
		free (inactivity_text);
		free (expiration_text);
		
		NSS_DEBUG ("getspnam_r : Failed to parse number.n");
	
		return NSS_STATUS_UNAVAIL;
	}
	
	/*
	// Get the flag.
	if (fgets (path, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		//free (name);
		free (password);
		free (change_text);
		//free (shell);
		free (minimum_text);
		free (maximum_text);
		free (warning_text);
		free (inactivity_text);
		free (expiration_text);
		
		NSS_DEBUG ("getspnam_r : Failed to parse the executable's output : GECOS\n");
	
		return NSS_STATUS_UNAVAIL;
	}
	flag_text = strdup (line);
	flag_text [strcspn (flag_text, "\r\n")] = NULL;
	errno = 0;
	flag = strtoul (flag_text, NULL, 10);
	if (errno != 0)
	{
		pclose (file);
		
		//free (name);
		free (password);
		free (change_text);
		free (minimum_text);
		free (maximum_text);
		free (warning_text);
		free (inactivity_text);
		free (expiration_text);
		free (flag_text);
		
		NSS_DEBUG ("getspnam_r : Failed to parse number.n");
	
		return NSS_STATUS_UNAVAIL;
	}
	*/
	
	//name = sqlite3_column_text(pSt, 0);
	//gid = sqlite3_column_int(pSt, 1);
	////gid = sqlite3_column_text(pSt, 1);
	//shell = sqlite3_column_text(pSt, 2);
	//home = sqlite3_column_text(pSt, 3);

	// Close executable's file pointer.
	//pclose (fp);
	// ...and get the error code of the executed command.
	error = WEXITSTATUS (pclose (file));
	
    result -> sp_namp = strdup (name);
    result -> sp_pwdp = password;
    result -> sp_lstchg = change;
    result -> sp_min = minimum;
    result -> sp_max = maximum;
    result -> sp_warn = warning;
    result -> sp_inact = inactivity;
    result -> sp_expire = expiration;
    //result -> sp_flag = flag;
    
	free (change_text);
	free (minimum_text);
	free (maximum_text);
	free (warning_text);
	free (inactivity_text);
	free (expiration_text);
	//free (flag_text);
	
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
	
	return NSS_STATUS_SUCCESS;
}


