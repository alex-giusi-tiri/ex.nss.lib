#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>

signed int main (signed int argc, char * argv [])
{
	FILE * file;
	char line [PATH_MAX];
	
	// Open the command for reading.
	file = popen ("/bin/ls /etc/", "r");
	
	if (file == NULL)
	{
		printf ("Failed to run the command.\n");
		
		return WEXITSTATUS (pclose (file));
	}
	
	// Read the output one line at a time and output the line.
	while (fgets (line, PATH_MAX, file) != NULL)
	{
		printf ("%s", line);
	}
	
	// Finalize.
	//pclose (file);
	return WEXITSTATUS (pclose (file));
	
	//return 0;
}

