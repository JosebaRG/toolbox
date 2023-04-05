#ifndef _LIBXLM_H
#define _LIBXLM_H

#include <stdio.h>

typedef struct xml_t
{
	struct xml_attribute_t * instruction_t;
	struct xml_tag_t       * content_t;
}xml_t;

typedef struct xml_attribute_t
{
	char * name;
	char * value;
	struct xml_attribute_t * next_attribute_t;
}xml_attribute_t;

typedef struct xml_tag_t
{
	char * name;
	char * value;
	struct xml_attribute_t * attribute_t;
	struct xml_tag_t       * nested_tag_t;
	struct xml_tag_t       * sibling_tag_t;
}xml_tag_t;

long free_xml_mem (xml_t * xml_mem_t);
char  * libxml_mem_to_xml  (xml_t * xml_mem_t);
FILE  * libxml_mem_to_file (xml_t * xml_mem_t);
xml_t * libxml_xml_to_mem  (char  * xml_txt);
xml_t * libxml_file_to_mem (char  * xml_name);


#endif //_LIBXLM_H
