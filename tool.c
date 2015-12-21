#include "default.h"
#include "tool.h"

#include <string.h>
#include <malloc.h>
#include <errno.h>
/*
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <sqlite3.h>
*/

/*
void fn_tst (void)
{
	NSS_DEBUG ("fn_tst\n");
}
*/

const long double * xml_xpath_evaluate_content_number (const xmlDocPtr document, const char * expression/*, const int radix*/, const unsigned long long int index, const bool strict)
{
	//NSS_DEBUG ("xml_xpath_evaluate_content_number : Line 16.\n");
	//return NULL;
	
	if (document == NULL || expression == NULL)
	{
		NSS_DEBUG ("xml_xpath_evaluate_content_number : Document or expression is null.\n");
		
		return NULL;
	}
	
	//NSS_DEBUG ("xml_xpath_evaluate_content_number : Line 26.\n");
	//return NULL;
	
	xmlChar * text;
	//long double * number, _number;
	signed long long int * number, _number;
	
	// Get the text.
	text = xml_xpath_evaluate_content (document, expression, index, strict);
	if (text == NULL)
	{
		NSS_DEBUG ("xml_xpath_evaluate_content_number : Failed to obtain text.\n");
		
		return NULL;
	}
	NSS_DEBUG ("xml_xpath_evaluate_content_number (): Obtain text [%s].\n", text);
	
	// Convert the text ID into a number.
	errno = 0;
	//_number = strtoul (text, NULL, radix);
	//_number = strtold (text, NULL);
	_number = strtoul (text, NULL, 10);
	if (errno != 0)
	{
		//pclose (file);
		
		xmlFree (text);
		//xmlFreeDoc (document);
		
		//NSS_DEBUG ("user_get: Failed to parse number : ID.\n");
		
		//* error = errno;
		
		NSS_DEBUG ("xml_xpath_evaluate_content_number : Failed to convert text into number.\n");
		
		return NULL;
	}
	//NSS_DEBUG ("user_get: Converted ID : %d.\n", id);
	// xmlFree (text);
	
	//number = malloc (sizeof (long double));
	number = malloc (sizeof (signed long long int));
	if (number == NULL)
	{
		//pclose (file);
		
		xmlFree (text);
		//xmlFreeDoc (document);
		
		//NSS_DEBUG ("user_get: Failed to parse number : ID.\n");
		
		//* error = errno;
		
		NSS_DEBUG ("xml_xpath_evaluate_content_number : Failed to allocate memory for the number.\n");
		
		return NULL;
	}
	
	* number = _number;
	
	NSS_DEBUG ("xml_xpath_evaluate_content_number : Obtained text [%s], converted numer [%ld], and memory allocated number [%ld].\n", text, _number, * number);
	
	xmlFree (text);
	
	return number;
}

const xmlChar * xml_xpath_evaluate_content (const xmlDocPtr document, const char * expression, const unsigned long long int index, const bool strict)
{
	if (document == NULL || expression == NULL)
	{
		NSS_DEBUG ("xml_xpath_evaluate_content : Document or expression is null.\n");
		
		return NULL;
	}
	
	const xmlChar * text;
	xmlXPathObjectPtr xpath_object;
	
	// Get the text.
	xpath_object = xml_xpath_evaluate (document, expression);
	if (xpath_object == NULL)
	{
		NSS_DEBUG ("xml_xpath_evaluate_content : Failed to obtain XPath object.\n");
		
		return NULL;
	}
	
	text = xml_xpath_node_content (xpath_object, index, strict);
	if (text == NULL)
	{
		xmlXPathFreeObject (xpath_object);
		
		NSS_DEBUG ("xml_xpath_evaluate_content : Failed to obtain text.\n");
		
		return NULL;
	}
	
	
	xmlXPathFreeObject (xpath_object);
	//NSS_DEBUG ("user_get : Retrieved ID as text : \"%s\"\n", (const char *) id_text);
	
	//// Remove any terminating line delimiter.
	//((char *) (id_text)) [strcspn (id_text, "\r\n")] = 0;
	//NSS_DEBUG ("user_get : Duplicated ID text : \"%s\"\n",(const char *)  id_text);
	
	NSS_DEBUG ("xml_xpath_evaluate_content : Obtained the XPath object and the text [%s].\n", text);
	
	return text;
}

const xmlChar * xml_xpath_node_content (const xmlXPathObjectPtr/*xmlNodeSetPtr*/ xpath_object, const unsigned long long int index, const bool strict)
{
	if (xpath_object == NULL)
		return NULL;
	
	//xmlNodePtr cur;
	//int size;
	//int i;
	
	//assert(output);
	//size = (nodes) ? nodes->nodeNr : 0;
	
	//fprintf(output, "Result (%d nodes):\n", size);
	
	// Out of bounds.
	if (index + 1 > xpath_object -> nodesetval -> nodeNr /*size*/)
	{
		NSS_DEBUG ("xml_xpath_node_content : Index is out of bounds.\n");
		
		return NULL;
	}
	
	if (xpath_object -> nodesetval -> nodeTab [index] == NULL)
	{
		NSS_DEBUG ("xml_xpath_node_content : Index is null.\n");
		
		return NULL;
	}
	
	//for(i = 0; i < nodes->nodeNr; ++ i)
	//{
		if (xpath_object -> nodesetval -> nodeTab [index] -> type == XML_NAMESPACE_DECL)
		{
			NSS_DEBUG ("xml_xpath_node_content : Name space : unimplemented.\n");
		/*
			xmlNsPtr ns;
			
			ns = (xmlNsPtr)nodes->nodeTab[i];
			cur = (xmlNodePtr)ns->next;
			if(cur->ns) { 
				fprintf(output, "= namespace \"%s\"=\"%s\" for node %s:%s\n", 
				ns->prefix, ns->href, cur->ns->href, cur->name);
			} else {
				fprintf(output, "= namespace \"%s\"=\"%s\" for node %s\n", 
				ns->prefix, ns->href, cur->name);
			}
		*/
			return NULL;
		}
		else if (xpath_object -> nodesetval -> nodeTab [index] -> type == XML_ELEMENT_NODE)
		{
			if (strict == 0)
			{
				if (xpath_object -> nodesetval -> nodeTab [index] -> ns)
				{
					//fprintf (output, "= element node \"%s:%s\"\n", xpath_object -> nodesetval -> nodeTab [index] -> ns -> href, xpath_object -> nodesetval -> nodeTab [index] -> name);
					
					NSS_DEBUG ("xml_xpath_node_content : Names space (2) : unimplemented.\n");
					
					return NULL;
				}
				else
				{
					//fprintf (output, "= element node \"%s\" [%s]\n", xpath_object -> nodesetval -> nodeTab [index] -> name, (const char *) xmlNodeGetContent (xpath_object -> nodesetval -> nodeTab [index]));
					
					NSS_DEBUG ("xml_xpath_node_content : Strict mode has not been enabled => May return contents.\n");
					
					return /*(const char *)*/ xmlNodeGetContent (xpath_object -> nodesetval -> nodeTab [index]);
				}
			}
			else
			{
				NSS_DEBUG ("xml_xpath_node_content : Strict mode has been enabled => May not return contents.\n");
				
				return NULL;
			}
		}
		else
		{
			//fprintf (output, "= node \"%s\": type %d\n", xpath_object -> nodesetval -> nodeTab [index] -> name, xpath_object -> nodesetval -> nodeTab [index] -> type);
			
			NSS_DEBUG ("xml_xpath_node_content : Returning content.\n");
			
			return /*(const char *)*/ xmlNodeGetContent (xpath_object -> nodesetval -> nodeTab [index]);
		}
	//}
}

const xmlXPathObjectPtr/*xmlNodeSetPtr*/ xml_xpath_evaluate (const xmlDocPtr document, const xmlChar * expression)
{
	if (document == NULL || expression == NULL)
		return NULL;
	
	//// The default document to be returned in case of an error.
	//xmlNodePtr node;
	xmlXPathObjectPtr xpath_object;
	xmlXPathContextPtr xpath_context;
	
	
	// Create an XPath evaluation context.
	xpath_context = xmlXPathNewContext (document);
	
	if (xpath_context == NULL)
	{
		//fprintf (stderr, "Error: Unable to create new XPath context.\n");
		
		return NULL;
	}
    
    
	// Evaluate the XPath expression.
	xpath_object = xmlXPathEvalExpression (expression, xpath_context);
	
	//xmlXPathFreeObject (xpath_object);
	xmlXPathFreeContext (xpath_context);
	
	return xpath_object;
}

const signed int system_execute (const char * command, char ** output)
{
	/*
	//* error = 123;
	
	NSS_DEBUG ("user_get : Looking for the user \"%s\" (%s).\n", get, get_type);
	
	//return NSS_STATUS_UNAVAIL;
	
	signed int result_error;
	unsigned long long int command_output_size = sizeof (char) * (SHRT_MAX + 1);
	//size_t command_output_size = sizeof (char) * 8;
	uid_t * id;
	gid_t * group_id;
	xmlChar * name;
	//xmlChar * id_text;
	xmlChar * password;
	//const unsigned char * group_name;
	//xmlChar * group_id_text;
	xmlChar * shell;
	xmlChar * home;
	xmlChar * gecos;
	
	FILE * file;
	
	// The parsed XML document tree.
	xmlDocPtr document;
	//xmlNodePtr root_element;
	// Reused variables for each user detail.
	//xmlXPathObjectPtr xpobj;	// XPath OBJect
	//const xmlNodeSetPtr nodes;
	//const xmlNodePtr node;
	
	//char line [PATH_MAX];
	//char line [INT_MAX / 8];
	char line [USHRT_MAX];
	char command [PATH_MAX] = "";
	//char command_output [ULLONG_MAX] = "";
	//char command_output [SHRT_MAX + 1] = "";
	char * command_output, * command_output_temp, * document_text;
	
	// executable . " user get " . argument_type . " " . argument
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
	
	//NSS_DEBUG ("user_get : fgets (line, PATH_MAX, file) : \"%d\".", fgets (line, PATH_MAX, file));
	//NSS_DEBUG ("user_get : Retrieved line : \"%s\"\n", line);
	
	// Read the output one line at a time and output the line.
	//while (fgets (line, PATH_MAX, file) != NULL)
	//{
		//printf ("%s", line);
	//	NSS_DEBUG ("user_get : Read : \"%s\"", line);
	//}
	
	//NSS_DEBUG ("user_get : Closing command...");
	//pclose (file);
	
	//NSS_DEBUG ("user_get : Returning...");
	//return NSS_STATUS_UNAVAIL;
	
	if (file == NULL)
	{
		NSS_DEBUG ("user_get : Failed to run the executable : \"%s\".\n", command);
		
		* error = errno;
		
		return NSS_STATUS_UNAVAIL;
	}
	NSS_DEBUG ("user_get : Succeeded in running the executable : \"%s\".\n", command);
	
	//while (fgets (path, sizeof (path), fp) != NULL)
	//{
	//	//printf ("%s", path);
	//}
	
	//NSS_DEBUG ("PATH_MAX:%d\n", PATH_MAX);
	//NSS_DEBUG ("INT_MAX:%d\n", INT_MAX);
	
	//return NSS_STATUS_UNAVAIL;
	
	command_output = malloc (command_output_size);
	strcpy (command_output, "");
	//* command_output = "";
	//command_output [0] = '\0';
	
	
	//strcat (command_output, "ABC");
	
	//free (document_text);
	//free (command_output);
	//command_output = NULL;
	
	//return NSS_STATUS_UNAVAIL;
	
	//NSS_DEBUG ("user_get : (sizeof (char) * (SHRT_MAX + 1)) : \"%d\"", (sizeof (char) * (SHRT_MAX + 1)));
	
	
	// Read the output one line at a time and save the line.
	while (fgets (line, USHRT_MAX - 1, file) != NULL)
	{
		//printf ("%s", line);
		NSS_DEBUG ("user_get : Read : \"%s\"", line);
		
		//NSS_DEBUG ("user_get : strlen (command_output) + strlen (line) + 1 : \"%d\"", strlen (command_output) + strlen (line) + 1);
		//NSS_DEBUG ("user_get : sizeof (command_output) : \"%d\"", sizeof (command_output));
		//NSS_DEBUG ("user_get : sizeof (command_output) / sizeof (command_output [0]) : \"%d\"", sizeof (command_output) / sizeof (command_output [0]));
		
		
		if (strlen (command_output) + strlen (line) + 1 > command_output_size)
		{
			command_output_size *= 2;
			command_output_temp = realloc (command_output, sizeof (char) * command_output_size);
			
			if (command_output_temp == NULL)
			{
				pclose (file);
				
				free (command_output);
				
				NSS_DEBUG ("user_get : \"realloc ()\" failed.\n");
				
				return NSS_STATUS_UNAVAIL;
			}
			
			command_output = command_output_temp;
			
			//NSS_DEBUG ("user_get : sizeof (char) * sizeof (command_output) * 2) : size : new \"%d\"", sizeof (char) * sizeof (command_output) * 2);
			//NSS_DEBUG ("user_get : sizeof (command_output) : size : post \"%d\"", sizeof (command_output));
		}
		
		// Fill "command_output".
		strcat (command_output, line);
	}
	
	document_text = strdup (command_output);
	
	
	//return NSS_STATUS_UNAVAIL;
	
	// Uneeded anymore.
	free (command_output);
	//free (document_text);
	//command_output = NULL;
	
	//return NSS_STATUS_UNAVAIL;
	
	
	result_error = WEXITSTATUS (pclose (file));
	if (result_error != 0)
	{
		
		free (document_text);
		
		* error = errno;
		
		NSS_DEBUG ("user_get : The executable did not run properly : the command was \"%s\"; the error was [%i].\n", command, result_error);
		
		return NSS_STATUS_UNAVAIL;
	}
	
	NSS_DEBUG ("user_get : Obtained command output : \"%s\".\n", document_text);
	
	//printf ("user_get : Obtained command output [24] : \"%c\".\n", command_output [24]);
	
	//return NSS_STATUS_UNAVAIL;
	
	if (document_text == NULL)
	{
		NSS_DEBUG ("user_get : Failed to duplicate the XML document.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	/*
		As the document is in memory, it does not have a base, according to RFC 2396;
		then, the "noname.xml" argument will serve as its base.
	* /
	//document = xmlReadMemory (command_output, strlen (command_output), "noname.xml", NULL, 0);
	document = xmlReadMemory (document_text, strlen (document_text), "noname.xml", NULL, 0);
	//document = xmlReadMemory (command_output, 477, "noname.xml", NULL, 0);
	//document = xmlReadMemory ("<?xml version=\"1.0\"?><doc/>", 27, "noname.xml", NULL, 0);
	
	free (document_text);
	
	//return NSS_STATUS_UNAVAIL;
	
	if (document == NULL)
	{
		//fprintf (stderr, "Failed to parse document\n");
		
		//free (document_text);
		
		//NSS_DEBUG ("user_get : Failed to parse the XML document : \"%s\" (%d).\n", command_output, strlen (command_output));
		//NSS_DEBUG ("user_get : Failed to parse the XML document : \"%s\" (%d).\n", document_text, strlen (document_text));
		NSS_DEBUG ("user_get : Failed to parse the XML document.\n");
		
		return NSS_STATUS_UNAVAIL;
	}
	
	//xml_xpath_evaluate_content_number (document, "/xml/user/id", 10, 0, 1);
	
	NSS_DEBUG ("user_get : Parsed the XML document.\n");
	
	// Get the root element.
	//root_element = xmlDocGetRootElement (doc);
	
	//NSS_DEBUG ("user_get : fgets (line, PATH_MAX, file) : \"%d\".", fgets (line, PATH_MAX, file));
	//NSS_DEBUG ("user_get : Retrieved ID as text : \"%s\"\n", line);
	//xml_xpath_evaluate_content_number (document, "/xml/user/id", 10, 0, 1);
	//return NSS_STATUS_UNAVAIL;
	*/
}

