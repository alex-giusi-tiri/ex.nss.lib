#include <stdio.h>
#include <stdlib.h>
//#include <linux/limits.h>
#include <errno.h>
#include <string.h>

signed int main (signed int argc, char * argv [])
{
	signed long int number;
	char number_text [12] = "1234567";
	
	errno = 0;
	number = strtoul (number_text, NULL, 10);
	//number = strtoul (NULL, NULL, 10);
	if (errno != 0)
	{
		printf ("Could not convert.\n");
		
		return 1;
	}
	
	printf ("Converted:\nText  : %s\nNumber: %d.\n", number_text, number);
	
	
	return 0;
}

