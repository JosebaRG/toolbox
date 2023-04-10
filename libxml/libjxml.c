#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#include "libxml.h"
#include "libstring.h"

/*********************************************************************************
 *                                 DECLARATIONS
 *********************************************************************************/

FILE * libxml_create (char * xml_name);
FILE * libxml_open (char * xml_name);
FILE * libxml_close (FILE * xml_file);

long libxml_length (FILE * xml_file);
char * libxml_read (FILE * xml_file, long xml_length);
char * libxml_read_instruction (char * xml_txt);
char * libxml_read_content (char * xml_txt);

void libxml_parse_tag (char * content_txt, long position, xml_tag_t * tag_t);

/*********************************************************************************
 *                                   API
 *********************************************************************************/

int libxml_free_xml_mem (xml_t * xml_mem_t)
{
	int attributes = 0;
	int content = 0;

	if (xml_mem_t != NULL)
	{
		attributes = free_attribute (xml_mem_t->instruction_t);
		content = free_tag (xml_mem_t->content_t);
		free (xml_mem_t);
	}

	return attributes + content;
}

FILE  * libxml_mem_to_file (xml_t * xml_mem_t, char * xml_name, bool close)
{
	FILE * xml_file;

	xml_file = libxml_create (xml_name);
	libxml_write_instruction (xml_file, xml_mem_t->instruction_t);
	libxml_write_tag (xml_file, xml_mem_t->content_t, 0);

	if (close)
		libxml_close (xml_file);

	return xml_file;
}

xml_t * libxml_xml_to_mem (char * xml_txt)
{
	xml_t * xml_mem_t;

	xml_mem_t = libxml_init_xml_mem ();
	xml_mem_t->instruction_t = libxmls_parse_instruction (xml_txt);
	xml_mem_t->content_t = libxml_parse_content (xml_txt);

	return xml_mem_t;
}

xml_t * libxml_file_to_mem (char * xml_name)
{
	char * xml_txt;
	FILE * xml_file;
	xml_t * xml_mem_t;

	xml_file = libxml_open (xml_name);
	xml_txt = libxml_read (xml_file, -1);
	xml_file = libxml_close (xml_file);
	
	xml_mem_t = libxml_xml_to_mem (xml_txt);

	free (xml_txt);
	return xml_mem_t;
}

/*********************************************************************************
 *                                  FREE MEMORY
 *********************************************************************************/

int free_attribute (xml_attribute_t * attribute_t)
{
	int quantity = 0;

	if (attribute_t != NULL)
	{
		free (attribute_t->name);
		free (attribute_t->value);
		quantity = free_attribute (attribute_t->next_attribute_t);
		
		free (attribute_t);
		quantity++;
	}

	return quantity;
}

int free_tag (xml_tag_t * tag_t)
{
	int quantity = 0
	int attributes;
	int tags;

	if (tag_t != NULL)
	{
		free (tag_t->name);
		free (tag_t->value);

		attributes = free_attribute (tag_t->attribute_t);
		tags = free_tag (tag_t->nested_tag_t);
		quantity = attributes + tags;
		tags = free_tag (tag_t->sibling_tag_t);
		quantity = quantity + tags;
		
		free (tag_t);
		quantity++;
	}

	return quantity;
}

/*********************************************************************************
 *                             MEM TO FILE FUNCTIONS
 *********************************************************************************/

void libxml_write_attribute (FILE * xml_file, xml_attribute_t * attribute_t)
{
	if (attribute_t != NULL)
	{
		fprintf (xml_file, " %s=\"%s\"", attribute_t->name, attribute_t->value);
		
		libxml_write_attribute (xml_file, attribute_t->next_attribute_t);
	}
}

void libxml_write_instruction (FILE * xml_file, xml_attribute_t * instruction_t)
{
	fprintf (xml_file, "<?xml");

	libxml_write_attribute (xml_file, instruction_t);

	fprintf (xml_file, "?>");
}

void libxml_write_tag (FILE * xml_file, xml_tag_t * tag_t, int indent)
{
	fprintf (xml_file, "\n");

	int i;
	for (i=0; i<indent; i++)
		fprintf (xml_file, "\t");

	fprintf (xml_file, "<%s", tag_t->name);

	libxml_write_attribute (xml_file, tag_t->attribute_t);

	if ((tag_t->value == NULL) && (tag_t->nested_tag_t == NULL))
		fprintf (xml_file, "/>");
	else
		fprintf (xml_file, ">");

	if ((tag_t->value == NULL) && (tag_t->nested_tag_t != NULL))
	{
		libxml_write_tag (xml_file, tag_t->nested_tag_t, indent + 1);
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
		libxml_write_tag (xml_file, tag_t->sibling_tag_t, indent);
}


/*********************************************************************************
 *                                 FILE FUNCTIONS
 *********************************************************************************/

FILE * libxml_create (char * xml_name)
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

FILE * libxml_open (char * xml_name)
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

FILE * libxml_close (FILE * xml_file)
{
	fclose (xml_file);

	return xml_file;
}

long libxml_length (FILE * xml_file)
{
	long xml_length;

    fseek (xml_file, 0, SEEK_END);
    xml_length = ftell(xml_file);

    fseek (xml_file, 0, SEEK_SET);

	return xml_length;
}

char * libxml_read (FILE * xml_file, long xml_length)
{
	long read_len;
		
	if (xml_length < 0)
	{
		xml_length = libxml_length (xml_file);
	}

	char * xml = (char *) malloc (xml_length * sizeof (char));

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

char * libxml_check_empty (char * element)
{
	long length;
	length = libstring_length (element);

	while (length > 0)
	{
		if ((element [length - 1] != ' ')
			&& (element [length - 1] != '\n')
			&& (element [length - 1] != '\t'))
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

char * libxml_read_instruction (char * xml_txt)
{
	char * instruction;

	long inst_start;
	long inst_end;
	long inst_length;
	
	inst_start  = libstring_search (xml_txt, 0, "<?");
	inst_end    = libstring_search (xml_txt, 0, "?>");

	int bracket_len;
	bracket_len = libstring_length ("?>");
	inst_length = inst_end - inst_start + bracket_len; // two characters extra for "?>"

	if (inst_end <= inst_start)
		return NULL;
	
	instruction = (char *) malloc (inst_length * sizeof (char));
	libstring_subset (xml_txt, inst_start, inst_length, instruction);
	
	return instruction;
}

char * libxml_read_content (char * xml_txt)
{
	long next_inst;
	long inst_end;
	long main_start;

	inst_end = libstring_search (xml_txt, 0, "?>");
	int bracket_len;
	bracket_len = libstring_length ("?>");
	inst_end = inst_end + bracket_len; // two characters extra for "?>"

	main_start = libstring_search (xml_txt, inst_end, "<");
	next_inst  = libstring_search (xml_txt, inst_end, "<?");

	if (next_inst > 0)
	{
		printf ("\nLibXML: Error reading content. Instruction detected.");
		return NULL;
	}
	return xml_txt + inst_end + main_start - 1;
}

long libxml_find_next_tag (char * content_txt, long position)
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

char * libxml_create_close_tag (char * name)
{
	long name_length;
	name_length = libstring_length (name);
	int bracket_len;
	bracket_len = libstring_length ("</>");
	close = (char *) malloc ((name_length + bracket_len) * sizeof (char)); // "</ >" three characters

	close[0] = '<';
	close[1] = '/';
	close[2] = '\0';
	libstring_concat (close, name);
	libstring_concat (close, ">");

	return close;
}

char * libxml_parse_tag_name (char * content_txt, long position)
{
	char * name = NULL;

	long blank;
	long bracket;
	long slash;
	long small = LONG_MAX;  // Maximum possible value for long

	blank   = libstring_search (content_txt, position + 1, " ");
	bracket = libstring_search (content_txt, position + 1, ">");
	slash   = libstring_search (content_txt, position + 1, "/>");

	if ((blank > 0) && (blank < small))
        small = blank;
    
    if ((bracket > 0) && (bracket < small))
    	small = bracket;

	if ((slash > 0) && (slash < small))
        small = slash;

	if ((small > 0) && (small < LONG_MAX))
	{
		name = (char *) malloc (small * sizeof (char));
		libstring_subset (content_txt, position + 1, small, name);
	}
	else
		printf ("\nLIBXML: Error parsing tag name");

	return name;
}

char * libxml_parse_tag_value (char * content_txt, long position, char * name)
{
	char * value = NULL;
	
	long bracket;
	long slash;

	bracket = libstring_search (content_txt, position, ">");
	slash   = libstring_search (content_txt, position, "/>");

	if ((bracket < slash) || (slash < 0))
	{
		char * close;
		close = libxml_create_close_tag (name);
		
		long close_pos;
		long next_tag;
		long start_pos;

		start_pos = position + bracket;

		close_pos = libstring_search (content_txt, start_pos, close);
		next_tag = libxml_find_next_tag (content_txt, start_pos);

		long length;
		length = next_tag - start_pos;

		if (close_pos == length)
		{
			if (length > 0)
			{
				value = (char *) malloc (length * sizeof (char));
				libstring_subset (content_txt, start_pos, length, value);
				value = libxml_check_empty (value);
				printf ("\n>>>%s has tag value: -%s-", name, value);
			}
			else
			{
				printf ("\nLibXML: Error reading tag value.");
			}
		}
		else
		{
			printf ("\n>>>%s has no tag values (2)", name);
		}

		free (close);
	}
	else
	{
		printf ("\n>>>%s has no tag values (1)", name);
	}

	return value;
}

xml_tag_t * libxml_parse_tag_nested (char * content_txt, long position, char * name)
{
	xml_tag_t * tag_t = NULL;

	long bracket;
	long slash;

	bracket = libstring_search (content_txt, position, ">");
	slash   = libstring_search (content_txt, position, "/>");

	if ((bracket < slash) || (slash < 0))
	{
		char * close;
		close = libxml_create_close_tag (name);

		long close_pos;
		long next_tag;
		long start_pos;

		start_pos = position + libstring_length (name);

		close_pos = libstring_search (content_txt, start_pos, close);
		next_tag = libxml_find_next_tag (content_txt, start_pos);

		if (close_pos > (next_tag - start_pos))
		{
			long length;
			length = position + next_tag;
			printf ("\n%s has nested on %ld", name, length);
			tag_t = (xml_tag_t *) malloc (sizeof (xml_tag_t));

			libxml_parse_tag (content_txt, next_tag, tag_t);
		}
		else
		{
			printf ("\n%s has no nested (2)", name);
		}
		
		free (close);
	}
	else
	{
		printf ("\n%s has no nested (1)", name);
	}

	return tag_t;
}

xml_tag_t * libxml_parse_tag_sibling (char * content_txt, long position, char * name)
{
	xml_tag_t * tag_t = NULL;

	long bracket;
	long slash;
	long sibling_pos = 0;

	bracket = libstring_search (content_txt, position, ">");
	slash   = libstring_search (content_txt, position, "/>");

	if ((slash > 0) && (slash <= bracket))
	{
		sibling_pos = position + slash + libstring_length ("/>");
	}
	else if ((bracket < slash) || (slash < 0))
	{
		char * close;
		close = libxml_create_close_tag (name);

		long close_pos;
		close_pos = libstring_search (content_txt + position, close);
		sibling_pos = position + close_pos + name_length + 3;

		free (close);
	}

	if (sibling_pos > 0)
	{
		long next_tag;
		long next_close;
		next_tag = libxml_find_next_tag (content_txt, sibling_pos);
		next_close = libstring_search (content_txt + sibling_pos, "</");

		if (next_tag < 0)
		{
			printf ("\n%s has no sibling (3)", name);
			return tag_t;
		}
			
		if (next_close > 0)
			next_close = next_close + sibling_pos + 1;

		if (next_tag < next_close)
		{
			tag_t = (xml_tag_t *) malloc (sizeof (xml_tag_t));
			printf ("\n%s has sibling on %ld", name, next_tag);
			libxml_parse_tag (content_txt, next_tag, tag_t);
		}
		else
		{
			printf ("\n%s has no sibling (2)", name);
		}
	}
	else
	{
		printf ("\n%s has no sibling (1)", name);
	}
	return tag_t;
}

xml_attribute_t * libxml_parse_tag_attribute (char * content_txt, long position, char * name)
{
	xml_attribute_t * attribute_t = NULL;
	xml_attribute_t * attribute_last_t = NULL;

	long offset;
	long offset_aux;
	long length;

	position = position + libstring_length (name);

	while (1)
	{
		while (content_txt [position] == ' ') 	
			position++;

		offset = libstring_search (content_txt + position, "=");
		offset_aux = libstring_search (content_txt + position, ">");
		
		if ((offset_aux <= offset) || (offset < 0))
		{
			return attribute_t;
		}
		else
		{
			xml_attribute_t * attribute_aux_t;
			attribute_aux_t = (xml_attribute_t  *) malloc (sizeof (xml_attribute_t));
			
			if (attribute_t == NULL)
				attribute_t = attribute_aux_t;

			if (attribute_last_t == NULL)
				attribute_last_t = attribute_aux_t;
			else
			{
				attribute_last_t->next_attribute_t = attribute_aux_t;
				attribute_last_t = attribute_last_t->next_attribute_t;
			}
				
			char * name = (char *) malloc (offset * sizeof (char));
			if (name == NULL)
			{
				printf ("\nLibXML: Error malloc");
				return attribute_t;
			}
			length = libstring_subset (content_txt, position, offset, name);

			position = position + length + 1;
			offset = libstring_search (content_txt + position, "\"");
			position = position + offset + 1;
			offset = libstring_search (content_txt + position, "\"");

			char * value = (char *) malloc (offset * sizeof (char));
			length = libstring_subset (content_txt, position, offset, value);
			position = position + offset + 1;

			attribute_last_t->name = name;
			attribute_last_t->value = value;
			attribute_last_t->next_attribute_t = NULL;
		}
	}
	return attribute_t;
}

void libxml_parse_tag (char * content_txt, long position, xml_tag_t * tag_t)
{
	tag_t->name = libxml_parse_tag_name (content_txt, position);
	tag_t->value = libxml_parse_tag_value (content_txt, position, tag_t->name);
	tag_t->attribute_t = libxml_parse_tag_attribute (content_txt, position, tag_t->name);
	tag_t->nested_tag_t = libxml_parse_tag_nested (content_txt, position, tag_t->name);
	tag_t->sibling_tag_t = libxml_parse_tag_sibling (content_txt, position, tag_t->name);
}

xml_tag_t * libxml_parse_content (char * xml_txt)
{
	xml_tag_t * content_t; 
	char * content_txt;
	long position = 0;

	content_txt = libxml_read_content (xml_txt);
	position = libxml_find_next_tag (content_txt, position);
	content_t = (xml_tag_t *) malloc (sizeof (xml_tag_t));

	libxml_parse_tag (content_txt, position, content_t);
	
	return content_t;
}

xml_attribute_t * libxmls_parse_instruction (char * xml_txt)
{
	xml_attribute_t * attribute_t = NULL;
	xml_attribute_t * attribute_last_t = NULL;

	long offset;
	long offset_aux;
	long length;
	long position = 0;

	char * instruction;
	instruction = libxml_read_instruction (xml_txt);

	offset = libstring_search (instruction, position, "<?");
	position = position + libstring_length ("<?");
	
	offset = libstring_search (instruction, position, " ");
	offset_aux = libstring_search (instruction, position, "?>");
	if (offset_aux <= offset)
	{
		printf ("\nLibXML: Error parsing instruction. No instructions");
		return NULL;
	}

	offset = libstring_search (instruction, position, "xml");
	position = position + libstring_length ("xml");
	if (offset != 0)
	{
		printf ("\nLibXML: Error parsing instruction. No xml begining");
		return NULL;
	}

	while (1)
	{
		while (instruction [position] == ' ') 	
			position++;

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
			
			if (attribute_t == NULL)
				attribute_t = attribute_aux_t;

			if (attribute_last_t == NULL)
				attribute_last_t = attribute_aux_t;
			else
			{
				attribute_last_t->next_attribute_t = attribute_aux_t;
				attribute_last_t = attribute_last_t->next_attribute_t;
			}			
			
			char * name = (char *) malloc (offset * sizeof (char));
// cambiar esto por un assert
			if (name == NULL) 
			{
				printf ("\nLibXML: Error malloc");
				free (instruction);
				return attribute_t;
			}

			length = libstring_subset (instruction, position, offset, name);

// sobran los "+1"???????????
			position = position + length + 1;
			offset = libstring_search (instruction, position, "\"");
			position = position + offset + 1;
			offset = libstring_search (instruction, position, "\"");

			char * value = (char *) malloc (offset * sizeof (char));
			length = libstring_subset (instruction, position, offset, value);
			position = position + offset + 1;

			attribute_last_t->name = name;
			attribute_last_t->value = value;
			attribute_last_t->next_attribute_t = NULL;
		}
	}

	free (instruction);
	return attribute_t;
}

xml_t * libxml_init_xml_mem (char * xml_txt)
{
	xml_t * xml_mem_t;
	xml_mem_t = (xml_t *) malloc (sizeof (xml_t));

	xml_mem_t->instruction_t = NULL;
	xml_mem_t->content_t = NULL;

	return xml_mem_t;
}



/*********************************************************************************
 *                                  TESTS
 *********************************************************************************/

void libxml_test_atributes (xml_attribute_t * attribute_t)
{
	if (attribute_t != NULL)
	{
		printf ("\n+++++++++++++++");
		printf ("\nCampo: %s", attribute_t->name);
		printf ("\nValor: %s", attribute_t->value);
		
		libxml_test_atributes (attribute_t->next_attribute_t);
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

	xml_mem_t = libxml_file_to_mem (".ignore/example.xml");
	
	libxml_mem_to_file (xml_mem_t, ".ignore/xml_name.xml", true);
	
	quantity = libxml_free_xml_mem (xml_mem_t);

	printf ("\n\n++++++++++++++++++++++++++++++++++++++++++++++++++++ %ld\n\n", quantity);

	printf("\n");
    return 0;
}