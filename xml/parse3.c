/**
 * section: Parsing
 * synopsis: Parse an XML document in memory to a tree and free it
 * purpose: Demonstrate the use of xmlReadMemory() to read an XML file
 *          into a tree and and xmlFreeDoc() to free the resulting tree
 * usage: parse3
 * test: parse3
 * author: Daniel Veillard
 * copy: see Copyright for the status of this software.
 */

#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include <string.h>

static const char *document = "<doc/>";
//static const char *document = "<?xml version=\"1.0\"?><xml><user><id>112233</id><name>my awesome user name</name><authentication><password><cipher>none</cipher><value/></password></authentication><shell>/my/shell</shell><home>/my/home/path</home><gecos>my:gecos:field</gecos><group><id>12321</id><name>the main group</name></group><groups><count>2</count><list><member><id>1</id><name>my awesome group</name></member><member><id>2</id><name>my other awesome group</name></member></list></groups></user></xml>";

/**
 * example3Func:
 * @content: the content of the document
 * @length: the length in bytes
 *
 * Parse the in memory document and free the resulting tree
 */
static void
example3Func(const char *content, int length) {
    xmlDocPtr doc; /* the resulting document tree */

    /*
     * The document being in memory, it have no base per RFC 2396,
     * and the "noname.xml" argument will serve as its base.
     */
    doc = xmlReadMemory(content, length, "noname.xml", NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse document\n");
	return;
    }
    
    fprintf(stdout, "Successfully parsed document\n");
    
    xmlFreeDoc(doc);
}

int main(void) {
    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    example3Func(document, strlen (document));

    /*
     * Cleanup function for the XML library.
     */
    xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();
    return(0);
}
