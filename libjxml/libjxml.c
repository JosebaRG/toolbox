/**
 * @file libjxml.c
 *
 * @brief Library to work with xml files.
 *
 * A structure tree is defined to store an XML file in memory. This library provides
 * functions to convert the XML file into the tree structure and vice versa.
 *
 * @author Joseba R.G.
 *         joseba.rg@protonmail.com
 */

#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <ctype.h>

#include "libjxml.h"
#include "libstring.h"
#include "libassert.h"

/*********************************************************************************
 *                                 DECLARATIONS
 *********************************************************************************/

int libjxml_free_attribute (xml_attribute_t * attribute_t);
int libjxml_free_tag (xml_tag_t * tag_t);

void libjxml_write_attribute (FILE * xml_file, xml_attribute_t * attribute_t);
void libjxml_write_instruction (FILE * xml_file, xml_attribute_t * instruction_t);
void libjxml_write_tag (FILE * xml_file, xml_tag_t * tag_t, int indent);

FILE * libjxml_create (char * xml_name);
FILE * libjxml_open (char * xml_name);
FILE * libjxml_close (FILE * xml_file);
long libjxml_length (FILE * xml_file);
char * libjxml_read (FILE * xml_file, long xml_length);

long libjxml_advance_spaces (char * text, long position);
long libjxml_search_space (char * text, long position);
char * libjxml_check_empty (char * element);
char * libjxml_read_instruction (char * xml_txt);
char * libjxml_read_content (char * xml_txt);
long libjxml_find_next_tag (char * content_txt, long position);
char * libjxml_create_close_tag (char * name);
char * libjxml_parse_tag_name (char * content_txt, long position);
char * libjxml_parse_tag_value (char * content_txt, long position, char * name);
xml_tag_t * libjxml_parse_tag_nested (char * content_txt, long position, char * name);
xml_tag_t * libjxml_parse_tag_sibling (char * content_txt, long position, char * name);
xml_attribute_t * libjxml_parse_tag_attribute (char * content_txt, long position, char * name);
void libjxml_parse_tag (char * content_txt, long position, xml_tag_t * tag_t);
xml_tag_t * libjxml_parse_content (char * xml_txt);
xml_attribute_t * libjxml_parse_instruction (char * xml_txt);
xml_t * libjxml_init_xml_mem (char * xml_txt);

void libjxml_test_atributes (xml_attribute_t * attribute_t);

/*********************************************************************************
 *                                   API
 *********************************************************************************/

int libjxml_free_xml_mem (xml_t * xml_mem_t)
{
	int attributes = 0;
	int content = 0;

	if (xml_mem_t != NULL)
	{
		attributes = libjxml_free_attribute (xml_mem_t->instruction_t);
		content = libjxml_free_tag (xml_mem_t->content_t);
		free (xml_mem_t);
	}

	return attributes + content;
}

FILE  * libjxml_mem_to_file (xml_t * xml_mem_t, char * xml_name, bool close)
{
	FILE * xml_file;

	xml_file = libjxml_create (xml_name);
	libjxml_write_instruction (xml_file, xml_mem_t->instruction_t);
	libjxml_write_tag (xml_file, xml_mem_t->content_t, 0);

	if (close)
		libjxml_close (xml_file);

	return xml_file;
}

xml_t * libjxml_xml_to_mem (char * xml_txt)
{
	xml_t * xml_mem_t;

	xml_mem_t = libjxml_init_xml_mem (xml_txt);
	xml_mem_t->instruction_t = libjxml_parse_instruction (xml_txt);
	xml_mem_t->content_t = libjxml_parse_content (xml_txt);

	return xml_mem_t;
}

xml_t * libjxml_file_to_mem (char * xml_name)
{
	char * xml_txt;
	FILE * xml_file;
	xml_t * xml_mem_t;

	xml_file = libjxml_open (xml_name);
	xml_txt = libjxml_read (xml_file, -1);
	xml_file = libjxml_close (xml_file);
	
	xml_mem_t = libjxml_xml_to_mem (xml_txt);

	free (xml_txt);
	return xml_mem_t;
}

/*********************************************************************************
 *                                  FREE MEMORY
 *********************************************************************************/

int libjxml_free_attribute (xml_attribute_t * attribute_t)
{
	int quantity = 0;

	if (attribute_t != NULL)
	{
		free (attribute_t->name);
		free (attribute_t->value);
		quantity = libjxml_free_attribute (attribute_t->next_attribute_t);
		
		free (attribute_t);
		quantity++;
	}

	return quantity;
}

int libjxml_free_tag (xml_tag_t * tag_t)
{
	int quantity = 0;
	int attributes;
	int tags;

	if (tag_t != NULL)
	{
		free (tag_t->name);
		free (tag_t->value);

		attributes = libjxml_free_attribute (tag_t->attribute_t);
		tags = libjxml_free_tag (tag_t->nested_tag_t);
		quantity = attributes + tags;
		tags = libjxml_free_tag (tag_t->sibling_tag_t);
		quantity = quantity + tags;
		
		free (tag_t);
		quantity++;
	}

	return quantity;
}

/*********************************************************************************
 *                             MEM TO FILE FUNCTIONS
 *********************************************************************************/

void libjxml_write_attribute (FILE * xml_file, xml_attribute_t * attribute_t)
{
	if (attribute_t != NULL)
	{
		fprintf (xml_file, " %s=\"%s\"", attribute_t->name, attribute_t->value);

		libjxml_write_attribute (xml_file, attribute_t->next_attribute_t);
	}
}

void libjxml_write_instruction (FILE * xml_file, xml_attribute_t * instruction_t)
{
	fprintf (xml_file, "<?xml");

	libjxml_write_attribute (xml_file, instruction_t);

	fprintf (xml_file, "?>");
}

void libjxml_write_tag (FILE * xml_file, xml_tag_t * tag_t, int indent)
{
	fprintf (xml_file, "\n");

	int i;
	for (i=0; i<indent; i++)
		fprintf (xml_file, "\t");

	fprintf (xml_file, "<%s", tag_t->name);

	libjxml_write_attribute (xml_file, tag_t->attribute_t);

	if ((tag_t->value == NULL) && (tag_t->nested_tag_t == NULL))
		fprintf (xml_file, "/>");
	else
		fprintf (xml_file, ">");

	if ((tag_t->value == NULL) && (tag_t->nested_tag_t != NULL))
	{
		libjxml_write_tag (xml_file, tag_t->nested_tag_t, indent + 1);
		fprintf (xml_file, "\n");
		for (i=0; i<indent; i++)
			fprintf (xml_file, "\t");
	}

	if ((tag_t->value != NULL) && (tag_t->nested_tag_t == NULL))
		fprintf (xml_file, "%s", tag_t->value);

	if ((tag_t->value != NULL) && (tag_t->nested_tag_t != NULL))
		printf ("\nLibXML: Error writing tag with value and nested_tag");

	if ((tag_t->value != NULL) || (tag_t->nested_tag_t != NULL))
		fprintf (xml_file, "</%s>", tag_t->name);

	if (tag_t->sibling_tag_t != NULL)
		libjxml_write_tag (xml_file, tag_t->sibling_tag_t, indent);
}


/*********************************************************************************
 *                                 FILE FUNCTIONS
 *********************************************************************************/

FILE * libjxml_create (char * xml_name)
{
	FILE * xml_file;

    xml_file = fopen (xml_name, "w");
	if (xml_file == NULL)
	{
        printf ("\nLibXML: Error creating file");
        return NULL;
    }

	return xml_file;
}

FILE * libjxml_open (char * xml_name)
{
	FILE * xml_file;

    xml_file = fopen (xml_name, "r");
	if (xml_file == NULL)
	{
        printf ("\nLibXML: Error opening file");
        return NULL;
    }

	return xml_file;
}

FILE * libjxml_close (FILE * xml_file)
{
	fclose (xml_file);

	return xml_file;
}

long libjxml_length (FILE * xml_file)
{
	long xml_length;

    fseek (xml_file, 0, SEEK_END);
    xml_length = ftell(xml_file);

    fseek (xml_file, 0, SEEK_SET);

	return xml_length;
}

char * libjxml_read (FILE * xml_file, long xml_length)
{
	long read_len;
		
	if (xml_length < 0)
	{
		xml_length = libjxml_length (xml_file);
	}

	char * xml;
	xml = (char *) malloc (xml_length * sizeof (char));
	LIBASSERT_PTR (xml);
	read_len = fread (xml, sizeof (char), xml_length, xml_file);
	
	if (read_len == xml_length)
	{
		return xml;
	}
	else
	{
		printf ("\nLibXML: Error reading file. Expected %ld and received %ld.", xml_length, read_len);
		free (xml);
		return NULL;
	}
}

/*********************************************************************************
 *                                PARSE FUNCTIONS
 *********************************************************************************/

long libjxml_advance_spaces (char * text, long position)
{
	while (isspace (text [position]) != 0)
		position++;

	return position;
}

long libjxml_search_space (char * text, long position)
{
	long space;
	long newline;
	long tab_horizontal;
	long tab_vertical;
	long form_feed;
	long carriage;

	long result = LONG_MAX; // Maximum possible value for long

 	space          = libstring_search (text, position, " ");
 	newline        = libstring_search (text, position, "\n");
 	tab_horizontal = libstring_search (text, position, "\t");
 	tab_vertical   = libstring_search (text, position, "\v");
 	form_feed      = libstring_search (text, position, "\f");
 	carriage       = libstring_search (text, position, "\r");

	if ((space >= 0) && (space < result))
		result = space;

	if ((newline >= 0) && (newline < result))
		result = newline;

	if ((tab_horizontal >= 0) && (tab_horizontal < result))
		result = tab_horizontal;

	if ((tab_vertical >= 0) && (tab_vertical < result))
		result = tab_vertical;

	if ((form_feed >= 0) && (form_feed < result))
		result = form_feed;

	if ((carriage >= 0) && (carriage < result))
		result = carriage;

	if (result == LONG_MAX)
		result = -1;

	return result;
}

char * libjxml_check_empty (char * element)
{
	long length;
	length = libstring_length (element);

	while (length > 0)
	{
		if ((element [length - 1] != ' ')  && 
			(element [length - 1] != '\n') &&
			(element [length - 1] != '\t') && 
			(element [length - 1] != '\v') &&
			(element [length - 1] != '\f') && 
			(element [length - 1] != '\r'))
		{
			length--;
		}
		else
		{
			free (element);
			element = NULL;
			break;
		}
	}

	return element;
}

char * libjxml_read_instruction (char * xml_txt)
{
	char * instruction;

	long inst_start;
	long inst_end;
	long inst_length;
	
	inst_start = libstring_search (xml_txt, 0, "<?");
	inst_end   = libstring_search (xml_txt, 0, "?>");

	int bracket_len;
	bracket_len = libstring_length ("<?");
	inst_length = inst_end - inst_start + bracket_len;

	if (inst_end <= inst_start)
		return NULL;
	
	instruction = (char *) malloc (inst_length * sizeof (char));
	LIBASSERT_PTR (instruction);
	libstring_subset (xml_txt, inst_start, inst_length, instruction);
	
	return instruction;
}

char * libjxml_read_content (char * xml_txt)
{
	long next_inst;
	long inst_end;
	long main_start;

	inst_end = libstring_search (xml_txt, 0, "?>");
	inst_end = inst_end + libstring_length ("?>");

	main_start = libstring_search (xml_txt, inst_end, "<");
	next_inst  = libstring_search (xml_txt, inst_end, "<?");

	if (next_inst > 0)
	{
		printf ("\nLibXML: Error reading content. Instruction detected.");
		return NULL;
	}

	return xml_txt + main_start;
}

long libjxml_find_next_tag (char * content_txt, long position)
{
	long content_length;
	content_length = libstring_length (content_txt);
	
	while ((content_txt [position] != '<') && (position < content_length))
			position++;

	if ((position >= 0) && (position < content_length))
		return position;
	else
		return -1;
}

char * libjxml_create_close_tag (char * name)
{
	long name_length;
	name_length = libstring_length (name);
	int bracket_len;
	bracket_len = libstring_length ("</>");
	
	long length;
	length = name_length + bracket_len;
	char * close;
	close = (char *) malloc (length * sizeof (char));
	LIBASSERT_PTR (close);

	close[0] = '<';
	close[1] = '/';
	close[2] = '\0';
	libstring_concat (close, name);
	libstring_concat (close, ">");

	return close;
}

char * libjxml_parse_tag_name (char * content_txt, long position)
{
	char * name = NULL;

	long blank;
	long bracket;
	long slash;
	long small = LONG_MAX;  // Maximum possible value for long

	position = position + libstring_length ("<");

	blank   = libjxml_search_space (content_txt, position);
	bracket = libstring_search (content_txt, position, ">");
	slash   = libstring_search (content_txt, position, "/>");

	if ((blank > 0) && (blank < small))
        small = blank;
    
    if ((bracket > 0) && (bracket < small))
    	small = bracket;

	if ((slash > 0) && (slash < small))
        small = slash;

	if ((small > 0) && (small < LONG_MAX))
	{
		long length;
		length = small - position;
		name = (char *) malloc (length * sizeof (char));
		LIBASSERT_PTR (name);
		libstring_subset (content_txt, position, length, name);
	}
	else
		printf ("\nLIBXML: Error parsing tag name");

	return name;
}

char * libjxml_parse_tag_value (char * content_txt, long position, char * name)
{
	char * value = NULL;
	
	long bracket;
	long slash;

	bracket = libstring_search (content_txt, position, ">");
	slash   = libstring_search (content_txt, position, "/>");

	if ((bracket < slash) || (slash < 0))
	{
		char * close;
		close = libjxml_create_close_tag (name);
		
		long close_pos;
		long next_tag;
		long start_pos;
		long length;

		start_pos = bracket + libstring_length (">");

		close_pos = libstring_search (content_txt, start_pos, close);
		next_tag = libjxml_find_next_tag (content_txt, start_pos);
		
		length = next_tag - start_pos;

		if (close_pos == next_tag)
		{
			if (length > 0)
			{
				value = (char *) malloc (length * sizeof (char));
				LIBASSERT_PTR (value);
				libstring_subset (content_txt, start_pos, length, value);
				value = libjxml_check_empty (value);
			}
			else
			{
				printf ("\nLibXML: Error reading tag value.");
			}
		}
		free (close);
	}
	return value;
}

xml_tag_t * libjxml_parse_tag_nested (char * content_txt, long position, char * name)
{
	xml_tag_t * tag_t = NULL;

	long bracket;
	long slash;

	bracket = libstring_search (content_txt, position, ">");
	slash   = libstring_search (content_txt, position, "/>");

	if ((bracket < slash) || (slash < 0))
	{
		char * close;
		close = libjxml_create_close_tag (name);

		long close_pos;
		long next_tag;
		long start_pos;

		start_pos = position + libstring_length (name);

		close_pos = libstring_search (content_txt, start_pos, close);
		next_tag = libjxml_find_next_tag (content_txt, start_pos);

		if (close_pos > next_tag)
		{
			long length;
			length = position + next_tag;
			printf ("\n%s has nested on %ld", name, length);
			tag_t = (xml_tag_t *) malloc (sizeof (xml_tag_t));
			LIBASSERT_PTR (tag_t);

			libjxml_parse_tag (content_txt, next_tag, tag_t);
		}
		free (close);
	}

	return tag_t;
}

xml_tag_t * libjxml_parse_tag_sibling (char * content_txt, long position, char * name)
{
	xml_tag_t * tag_t = NULL;

	long bracket;
	long slash;
	long tag_end = 0;

	bracket = libstring_search (content_txt, position, ">");
	slash   = libstring_search (content_txt, position, "/>");

	long name_length;
	name_length = libstring_length (name);

	if ((slash > 0) && (slash <= bracket))
	{
		tag_end = slash + libstring_length ("/>");
	}
	else if ((bracket < slash) || (slash < 0))
	{
		char * close;
		close = libjxml_create_close_tag (name);

		long close_pos;
		close_pos = libstring_search (content_txt, position, close);
		tag_end = close_pos + libstring_length (close);

		free (close);
	}

	if (tag_end > 0)
	{
		long next_tag;
		long next_close;
		next_tag = libjxml_find_next_tag (content_txt, tag_end);
		next_close = libstring_search (content_txt, tag_end, "</");

		if (next_tag < 0)
			return tag_t;

		if (next_tag < next_close)
		{
			tag_t = (xml_tag_t *) malloc (sizeof (xml_tag_t));
			LIBASSERT_PTR (tag_t);
			printf ("\n%s has sibling on %ld", name, next_tag);
			libjxml_parse_tag (content_txt, next_tag, tag_t);
			printf("\n\nSibling:%s", content_txt + next_tag);
		}
	}

	return tag_t;
}

xml_attribute_t * libjxml_parse_tag_attribute (char * content_txt, long position, char * name)
{
	xml_attribute_t * attribute_t = NULL;
	xml_attribute_t * attribute_last_t = NULL;

	long offset;
	long offset_aux;
	long length;

	position = position + libstring_length ("<") + libstring_length (name);

	while (1)
	{
		position = libjxml_advance_spaces (content_txt, position);

		offset = libstring_search (content_txt, position, "=");
		offset_aux = libstring_search (content_txt, position, ">");
		
		if ((offset_aux <= offset) || (offset < 0))
		{
			return attribute_t;
		}
		else
		{
			xml_attribute_t * attribute_aux_t;
			attribute_aux_t = (xml_attribute_t  *) malloc (sizeof (xml_attribute_t));
			LIBASSERT_PTR (attribute_aux_t);
			
			if (attribute_t == NULL)
				attribute_t = attribute_aux_t;

			if (attribute_last_t == NULL)
				attribute_last_t = attribute_aux_t;
			else
			{
				attribute_last_t->next_attribute_t = attribute_aux_t;
				attribute_last_t = attribute_last_t->next_attribute_t;
			}
				
			length = offset - position;
			char * name;
			name = (char *) malloc (length * sizeof (char));
			LIBASSERT_PTR (name);
			length = libstring_subset (content_txt, position, length, name);

			position = offset + libstring_length ("=");
			offset = libstring_search (content_txt, position, "\"");
			position = offset + libstring_length ("\"");
			offset = libstring_search (content_txt, position, "\"");

			length = offset - position;
			char * value;
			value = (char *) malloc (offset * sizeof (char));
			LIBASSERT_PTR (value);
			length = libstring_subset (content_txt, position, length, value);
			position = offset + libstring_length ("\"");

			attribute_last_t->name = name;
			attribute_last_t->value = value;
			attribute_last_t->next_attribute_t = NULL;
		}
	}

	return attribute_t;
}

void libjxml_parse_tag (char * content_txt, long position, xml_tag_t * tag_t)
{
	tag_t->name = libjxml_parse_tag_name (content_txt, position);
	tag_t->value = libjxml_parse_tag_value (content_txt, position, tag_t->name);
	tag_t->attribute_t = libjxml_parse_tag_attribute (content_txt, position, tag_t->name);
	tag_t->nested_tag_t = libjxml_parse_tag_nested (content_txt, position, tag_t->name);
	tag_t->sibling_tag_t = libjxml_parse_tag_sibling (content_txt, position, tag_t->name);
}

xml_tag_t * libjxml_parse_content (char * xml_txt)
{
	xml_tag_t * content_t; 
	char * content_txt;
	long position = 0;

	content_txt = libjxml_read_content (xml_txt);
	position = libjxml_find_next_tag (content_txt, position);
	content_t = (xml_tag_t *) malloc (sizeof (xml_tag_t));
	LIBASSERT_PTR (content_t);

	libjxml_parse_tag (content_txt, position, content_t);
	
	return content_t;
}

xml_attribute_t * libjxml_parse_instruction (char * xml_txt)
{
	xml_attribute_t * attribute_t = NULL;
	xml_attribute_t * attribute_last_t = NULL;

	long offset;
	long offset_aux;
	long length;
	long position = 0;

	char * instruction;
	instruction = libjxml_read_instruction (xml_txt);
	
	offset = libstring_search (instruction, position, "<?");
	position = position + libstring_length ("<?");

	offset = libjxml_search_space (instruction, position);
	offset_aux = libstring_search (instruction, position, "?>");
	if (offset_aux <= offset)
	{
		printf ("\nLibXML: Error parsing instruction. No instructions");
		return NULL;
	}

	offset = libstring_search (instruction, position, "xml");
	if (offset != libstring_length ("<?"))
	{
		printf ("\nLibXML: Error parsing instruction. No xml begining");
		return NULL;
	}

	position = libstring_length ("<?xml");

	while (1)
	{
		position = libjxml_advance_spaces (instruction, position);

		offset = libstring_search (instruction, position, "=");
		offset_aux = libstring_search (instruction, position, "?>");
		
		if ((offset_aux <= offset) || (offset < 0))
		{
			free (instruction);
			return attribute_t;
		}
		else
		{
			xml_attribute_t * attribute_aux_t;
			attribute_aux_t = (xml_attribute_t  *) malloc (sizeof (xml_attribute_t));
			LIBASSERT_PTR (attribute_aux_t);
			
			if (attribute_t == NULL)
				attribute_t = attribute_aux_t;

			if (attribute_last_t == NULL)
				attribute_last_t = attribute_aux_t;
			else
			{
				attribute_last_t->next_attribute_t = attribute_aux_t;
				attribute_last_t = attribute_last_t->next_attribute_t;
			}			
			
			length = offset - position;
			char * name;
			name = (char *) malloc (length * sizeof (char));
			LIBASSERT_PTR (name);

			length = libstring_subset (instruction, position, length, name);

			position = offset + libstring_length ("=");
			offset = libstring_search (instruction, position, "\"");
			position = offset + libstring_length ("\"");
			offset = libstring_search (instruction, position, "\"");

			length = offset - position;
			char * value;
			value = (char *) malloc (offset * sizeof (char));
			LIBASSERT_PTR (value);

			length = libstring_subset (instruction, position, length, value);
			position = offset + libstring_length ("\"");

			attribute_last_t->name = name;
			attribute_last_t->value = value;
			attribute_last_t->next_attribute_t = NULL;
		}
	}

	free (instruction);
	return attribute_t;
}

xml_t * libjxml_init_xml_mem (char * xml_txt)
{
	xml_t * xml_mem_t;
	xml_mem_t = (xml_t *) malloc (sizeof (xml_t));
	LIBASSERT_PTR (xml_mem_t);

	xml_mem_t->instruction_t = NULL;
	xml_mem_t->content_t = NULL;

	return xml_mem_t;
}

/*********************************************************************************
 *                                  TESTS
 *********************************************************************************/

void libjxml_test_atributes (xml_attribute_t * attribute_t)
{
	if (attribute_t != NULL)
	{
		printf ("\n+++++++++++++++");
		printf ("\nCampo: %s", attribute_t->name);
		printf ("\nValor: %s", attribute_t->value);
		
		libjxml_test_atributes (attribute_t->next_attribute_t);
	}
	else
	{
		printf("\nAtributo en blanco");
	}
}

int main()
{
	xml_t * xml_mem_t;
	long quantity;

	do
	{
		xml_mem_t = libjxml_file_to_mem (".ignore/example.xml");
	
		libjxml_mem_to_file (xml_mem_t, ".ignore/xml_name.xml", true);
	
		quantity = libjxml_free_xml_mem (xml_mem_t);

		printf ("\n\n++++++++++++++++++++++++++++++++++++++++++++++++++++ %ld\n\n", quantity);
	} while (1);

	printf("\n");
    return 0;
}
