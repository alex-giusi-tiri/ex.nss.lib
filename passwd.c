#include "default.h"
//#include "tool.h"

#include <string.h>
//#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <grp.h>
#include <pwd.h>

// Get user by ID.
enum nss_status user_get (const char * get_type, const char * get, struct passwd * result, char * buffer, size_t buffer_size, int * error)
{
	NSS_DEBUG ("user_get : Looking for the user \"%s\" (%s).\n", get, get_type);
	
	signed int result_error;
	uid_t id;
	gid_t group_id;
	char * name;
	char * id_text;
	char * password;
	//const unsigned char * group_name;
	char * group_id_text;
	char * shell;
	char * home;
	char * gecos;
	
	FILE * file;
	char line [PATH_MAX];
	char command [PATH_MAX] = "";
	
	// executable . " user get " . get_type . " " . get
	// Argument count: 5
	strcat (command, executable);
	strcat (command, " user get ");
	strcat (command, get_type);
	strcat (command, " ");
	strcat (command, get);
	
	NSS_DEBUG ("user_get : Opening command... : \"%s\"", command);
	// Open the command for reading.
	file = popen (command, "r");
	
	NSS_DEBUG ("user_get : Reading...");
	
	NSS_DEBUG ("user_get : fgets (line, PATH_MAX, file) : \"%d\".", fgets (line, PATH_MAX, file));
	NSS_DEBUG ("user_get : Retrieved line : \"%s\"\n", line);
	
	// Read the output one line at a time and output the line.
	while (fgets (line, PATH_MAX, file) != NULL)
	{
		//printf ("%s", line);
		NSS_DEBUG ("user_get : Read : \"%s\"", line);
	}
	
	NSS_DEBUG ("user_get : Closing command...");
	pclose (file);
	
	NSS_DEBUG ("user_get : Returning...");
	return NSS_STATUS_UNAVAIL;

	if (file == NULL)
	{
		NSS_DEBUG ("user_get : Failed to run the executable : \"%s\".\n", command);
		
		* error = errno;
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("user_get : Succeeded in running the executable : \"%s\".\n", command);

	//while (fgets (path, sizeof (path)/*-1*/, fp) != NULL)
	//{
	//	//printf ("%s", path);
	//}
	
	NSS_DEBUG ("user_get : fgets (line, PATH_MAX, file) : \"%d\".", fgets (line, PATH_MAX, file));
	NSS_DEBUG ("user_get : Retrieved ID as text : \"%s\"\n", line);
	
	return NSS_STATUS_UNAVAIL;

	// Get the ID.
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		NSS_DEBUG ("user_get : Failed to parse the executable's output : ID\n");
		
		* error = errno;
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("user_get : Retrieved ID as text : \"%s\"\n", line);
	id_text = strdup (line);
	id_text [strcspn (id_text, "\r\n")] = 0;
	NSS_DEBUG ("user_get : Duplicated ID text : \"%s\"\n", id_text);
	errno = 0;
	id = strtoul (id_text, NULL, 10);
	if (errno != 0)
	{
		pclose (file);
		
		free (id_text);
		
		NSS_DEBUG ("user_get: Failed to parse number : ID.\n");
	
		* error = errno;
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("user_get: Converted ID : %d.\n", id);
	
	
	// Get the name.
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		free (id_text);
		
		NSS_DEBUG ("getpwuid_r : Failed to parse the executable's output : Name\n");
		
		* error = errno;
		
		return NSS_STATUS_UNAVAIL;
	}
	name = strdup (line);
	name [strcspn (name, "\\r\\n")] = 0;
	
	
	// Get the password.
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		free (id_text);
		free (name);
		
		NSS_DEBUG ("getpwuid_r : Failed to parse the executable's output : Password\n");
		
		* error = errno;
		
		return NSS_STATUS_UNAVAIL;
	}
	password = strdup (line);
	password [strcspn (password, "\\r\\n")] = 0;
	
	
	// Get the main group' ID.
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		free (id_text);
		free (name);
		free (password);
		
		NSS_DEBUG ("getpwuid_r : Failed to parse the executable's output : Group\n");
	
		* error = errno;
		
		return NSS_STATUS_UNAVAIL;
	}
	group_id_text = strdup (line);
	group_id_text [strcspn (group_id_text, "\\r\\n")] = 0;
	errno = 0;
	group_id = strtoul (group_id_text, NULL, 10);
	if (errno != 0)
	{
		pclose (file);
		
		free (id_text);
		free (name);
		free (password);
		free (group_id_text);
		
		NSS_DEBUG ("getspnam_r : Failed to parse number.n");
	
		* error = errno;
		
		return NSS_STATUS_UNAVAIL;
	}
	
	
	// Get the shell.
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		free (id_text);
		free (name);
		free (password);
		free (group_id_text);
		
		NSS_DEBUG ("getpwuid_r : Failed to parse the executable's output : Shell\n");
	
		* error = errno;
		
		return NSS_STATUS_UNAVAIL;
	}
	shell = strdup (line);
	shell [strcspn (shell, "\\r\\n")] = 0;
	

	// Get the home path.
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		free (id_text);
		free (name);
		free (password);
		free (group_id_text);
		free (shell);
		
		NSS_DEBUG ("getpwuid_r : Failed to parse the executable's output : Home\n");
	
		* error = errno;
		
		return NSS_STATUS_UNAVAIL;
	}
	home = strdup (line);
	home [strcspn (home, "\\r\\n")] = 0;
	

	// Get the GECOS field.
	if (fgets (line, PATH_MAX, file) == NULL)
	{
		pclose (file);
		
		free (id_text);
		free (name);
		free (password);
		free (group_id_text);
		free (shell);
		free (home);
		
		NSS_DEBUG ("getpwuid_r : Failed to parse the executable's output : GECOS\n");
	
		* error = errno;
		
		return NSS_STATUS_UNAVAIL;
	}
	gecos = strdup (line);
	gecos [strcspn (gecos, "\\r\\n")] = 0;
	

	//name = sqlite3_column_text(pSt, 0);
	//gid = sqlite3_column_int(pSt, 1);
	////gid = sqlite3_column_text(pSt, 1);
	//shell = sqlite3_column_text(pSt, 2);
	//home = sqlite3_column_text(pSt, 3);

	// Close executable's file pointer.
	//pclose (fp);
	// ...and get the error code of the executed command.
	result_error = WEXITSTATUS (pclose (file));
	if (result_error != 0)
	{
		free (id_text);
		free (name);
		free (password);
		free (group_id_text);
		free (shell);
		free (home);
		free (gecos);
	
		* error = errno;
		
		return NSS_STATUS_UNAVAIL;
	}
	//else if (error != 0)
	//	return NSS_STATUS_FAILURE;
	
    if (strlen (name) + 1 + strlen (password) + 1 + strlen (home) + 1 + strlen (shell) + 1 + strlen (gecos) + 1 > buffer_size)
    {
		//* error = errno;
        * error = ERANGE;
		
        return NSS_STATUS_TRYAGAIN;
    }
    
    result -> pw_uid = id;
    result -> pw_gid = group_id;
    result -> pw_name = name;
    result -> pw_passwd = password;
    result -> pw_gecos = gecos;
    result -> pw_shell = shell;
    result -> pw_dir = home;
    
	* error = errno;
	
	return NSS_STATUS_SUCCESS;
}

// Get user by name.
enum nss_status _nss_exo_getpwnam_r (const char * name, struct passwd * result, char * buffer, size_t buffer_size, int * error)
{
	return user_get ("name", name, result, buffer, buffer_size, error);
}

// Get user by ID.
enum nss_status _nss_exo_getpwuid_r (uid_t id, struct passwd * result, char * buffer, size_t buffer_size, int * error)
{
	// Include the terminating NULL character (byte).
	char id_text [12];
	snprintf (id_text, 12, "%d", id);
	
	return user_get ("id", id_text, result, buffer, buffer_size, error);
}

