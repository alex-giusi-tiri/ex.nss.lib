/*
   Simple test with libxml2 <http://xmlsoft.org>. It displays the name
   of the root element and the names of all its children (not
   descendents, just children).

   On Debian, compiles with:
   gcc -Wall -o read-xml2 $(xml2-config --cflags) $(xml2-config --libs) \
                    read-xml2.c    

*/

#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <malloc.h>

void display (xmlNode * node, const char * indentation)
{
	xmlNodePtr current;
	char * indentation_local = (char *) malloc (sizeof (char) * (strlen (indentation) + 1));
	//char indentation_local [strlen (indentation) + 2/* these extra characters: our character and the terminating NULL */] = "";
	char * indentation_local_child = (char *) malloc (sizeof (indentation_local) + 1);
	
	//printf ("Line 23.\n");
	
	strcpy (indentation_local, indentation);
	strcpy (indentation_local_child, indentation_local);
	strcat (indentation_local_child, "\t");
	
	//printf ("Line 28.\n");
	
	//document = xmlReadFile(filename, NULL, 0);
	//root = xmlDocGetRootElement(document);
	//fprintf(stdout, "Root is <%s> (%i)\n", root->name, root->type);
	//first_child = root->children;
	
	for (xmlNode * current = node; current != NULL; current = current -> next)
	{
		//fprintf (stdout, "%s<%s>\n", indentation_local, current -> name);
		
		if (current -> type == XML_ELEMENT_NODE)
		{
			//printf ("Line 39.\n");
			fprintf (stdout, "%s<%s>\n", indentation_local, current -> name);
			display (current -> children, indentation_local_child);
			//fprintf (stdout, "%s%s\n", indentation_local, (const char *) xmlNodeGetContent (current));
			fprintf (stdout, "%s</%s>\n", indentation_local, current -> name);
		}
		else //if (current -> type == XML_TEXT_NODE)
		{
			//printf ("Line 44.\n");
			
			//fprintf (stdout, "%s<%s>%s", indentation_local, current -> name, (char *) xmlNodeGetContent (current));
			//fprintf (stdout, "%s<%s>", indentation_local, current -> name);
			//fprintf (stdout, "%s", (char *) xmlNodeGetContent (current));
			fprintf (stdout, "%s%s\n", indentation_local, (const char *) xmlNodeGetContent (current));
			//fprintf (stdout, "%s", (char *) xmlNodeGetContent (current));
			//fprintf (stdout, "</%s>\n", current -> name);
			//fprintf (stdout, "\n");
		}
		
		/*
		if (current -> type == XML_ELEMENT_NODE)
		{
			//printf ("Line 52.\n");
			fprintf (stdout, "%stype: [%i] -- <%s>\n", indentation_local, current -> type, current -> name);
			//display (current -> next, indentation_local);
		}
		
		display (current -> children, indentation_local);
		*/
	}
	
	free (indentation_local);
	free (indentation_local_child);
	
	//fprintf(stdout, "...\n");
	//return 0;
}

int main (int argc, char ** argv)
{
	LIBXML_TEST_VERSION
	
	xmlDocPtr document;
	//xmlNode * root;
	const char * file_name;
	
	if (argc < 2)
	{
		fprintf (stderr, "Usage: \"%s\" file.xml\n", argv [0]);
		
		return 1;
	}
	
	file_name = argv [1];
	
	document = xmlReadFile (file_name, NULL, 0);
	//root = xmlDocGetRootElement (document);
	//fprintf (stdout, "Root : <%s> (%i)\n", root -> name, root -> type);
	//fprintf (stdout, "<xml>\n");
	/*
	first_child = root->children;

	for (node = first_child; node != NULL; node = node->next)
	{
		fprintf (stdout, "\t Child is <%s> (%i)\n", node->name, node->type);
	}
	*/
	//fprintf(stdout, "...\n");
	display (xmlDocGetRootElement (document), "");
	//display (document, "");
	//fprintf (stdout, "</xml>\n");
	
	xmlFreeDoc (document);
	xmlCleanupParser ();
	
	return 0;
}

