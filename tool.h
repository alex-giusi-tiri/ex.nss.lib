#pragma once

//#include <sqlite3.h>
//#include <grp.h>
//#include <pwd.h>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

const unsigned long int * xml_xpath_evaluate_content_number (const xmlDocPtr/* document*/, const char */* expression*/, const int/* radix*/, const unsigned long long int/* index*/, unsigned short int/* strict*/);
const xmlChar * xml_xpath_evaluate_content (const xmlDocPtr/* document*/, const char */* expression*/, const unsigned long long int/* index*/, unsigned short int/* strict*/);
const xmlChar * xml_xpath_node_content (const xmlXPathObjectPtr/* xpath_object*/, const unsigned long long int/* index*/, unsigned short int/* strict*/);
const xmlXPathObjectPtr xml_xpath_evaluate (const xmlDocPtr/* document*/, const xmlChar */* expression*/);

