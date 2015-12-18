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

const unsigned long int * xml_xpath_evaluate_content_number (const xmlDocPtr document, const char * expression, const int radix, const unsigned long long int index, unsigned short int strict)
{
	if (document == NULL || expression == NULL)
	{
		return NULL;
	}
	
	xmlChar * text;
	unsigned long int * number, _number;
	
	// Get the text.
	text = (xmlChar *) xml_xpath_evaluate_content (document, expression, index, strict);
	if (text == NULL)
	{
		return NULL;
	}
	
	// Convert the text ID into a number.
	//errno = 0;
	_number = strtoul (text, NULL, radix);
	if (errno != 0)
	{
		//pclose (file);
		
		xmlFree (text);
		//xmlFreeDoc (document);
		
		//NSS_DEBUG ("user_get: Failed to parse number : ID.\n");
		
		//* error = errno;
		
		return NULL;
	}
	//NSS_DEBUG ("user_get: Converted ID : %d.\n", id);
	xmlFree (text);
	
	number = malloc (sizeof (unsigned long int));
	* number = _number;
	
	return number;
}

const xmlChar * xml_xpath_evaluate_content (const xmlDocPtr document, const char * expression, const unsigned long long int index, unsigned short int strict)
{
	if (document == NULL || expression == NULL)
	{
		return NULL;
	}
	
	const xmlChar * text;
	xmlXPathObjectPtr xpath_object;
	
	// Get the text.
	//if (fgets (line, PATH_MAX, file) == NULL)
	xpath_object = xml_xpath_evaluate (document, expression);
	if (xpath_object == NULL)
	{
		return NULL;
	}
	
	//id_text = strdup (line);
	text = xml_xpath_node_content (xpath_object, index, strict);
	if (text == NULL)
	{
		return NULL;
	}
	xmlXPathFreeObject (xpath_object);
	//NSS_DEBUG ("user_get : Retrieved ID as text : \"%s\"\n", (const char *) id_text);
	
	//// Remove any terminating line delimiter.
	//((char *) (id_text)) [strcspn (id_text, "\r\n")] = 0;
	//NSS_DEBUG ("user_get : Duplicated ID text : \"%s\"\n",(const char *)  id_text);
	
	return text;
}

const xmlChar * xml_xpath_node_content (const xmlXPathObjectPtr/*xmlNodeSetPtr*/ xpath_object, const unsigned long long int index, unsigned short int strict)
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
		return NULL;
	
	if (xpath_object -> nodesetval -> nodeTab [index] == NULL)
		return NULL;
	
	//for(i = 0; i < nodes->nodeNr; ++ i)
	//{
		if (xpath_object -> nodesetval -> nodeTab [index] -> type == XML_NAMESPACE_DECL)
		{
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
		}
		else if (xpath_object -> nodesetval -> nodeTab [index] -> type == XML_ELEMENT_NODE)
		{
			if (strict == 0)
			{
				if (xpath_object -> nodesetval -> nodeTab [index] -> ns)
				{
					//fprintf (output, "= element node \"%s:%s\"\n", xpath_object -> nodesetval -> nodeTab [index] -> ns -> href, xpath_object -> nodesetval -> nodeTab [index] -> name);
				}
				else
				{
					//fprintf (output, "= element node \"%s\" [%s]\n", xpath_object -> nodesetval -> nodeTab [index] -> name, (const char *) xmlNodeGetContent (xpath_object -> nodesetval -> nodeTab [index]));
					
					return /*(const char *)*/ xmlNodeGetContent (xpath_object -> nodesetval -> nodeTab [index]);
				}
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			//fprintf (output, "= node \"%s\": type %d\n", xpath_object -> nodesetval -> nodeTab [index] -> name, xpath_object -> nodesetval -> nodeTab [index] -> type);
			
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

