#include "libxml.h"
#include <stdbool.h>
#include <limits.h>

/*********************************************************************************
 *                                 DECLARATIONS
 *********************************************************************************/

FILE * libxml_open (char * xml_name);
FILE * libxml_close (FILE * xml_file);

long libxml_length (FILE * xml_file);
char * libxml_read (FILE * xml_file, long xml_length);
char * libxml_read_instruction (char * xml_txt);
char * libxml_read_content (char * xml_txt);

void libxml_parse_tag (char * content_txt, long position, xml_tag_t * tag_t);

/*********************************************************************************
 *                                 FILE FUNCTIONS
 *********************************************************************************/

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

char * libxml_read_instruction (char * xml_txt)
{
	long inst_start;
	long inst_end;
	long inst_length;
	
	inst_start = libstring_search (xml_txt, "<?");
	inst_end   = libstring_search (xml_txt, "?>");
	// two characters extra for "?>"
	inst_length = inst_end - inst_start + 2;

	if (inst_end <= inst_start)
		return NULL;

	char * instruction = (char *) malloc (inst_length * sizeof (char));

	libstring_subset (xml_txt, inst_start, inst_length, instruction);
	
	return instruction;
}

char * libxml_read_content (char * xml_txt)
{
	long next_inst;
	long inst_end;
	long main_start;

	inst_end = libstring_search (xml_txt, "?>");
	// two characters extra for "?>"
	inst_end = inst_end + 2;

	main_start = libstring_search (xml_txt + inst_end, "<");
	next_inst  = libstring_search (xml_txt + inst_end, "<?");

	if (next_inst > 0)
	{
		printf ("\nLibXML: Error reading content. Instruction detected.");
		return NULL;
	}
	return xml_txt + inst_end + main_start;
}

long libxml_find_next_tag (char * content_txt, long position)
{
	long content_length;
	content_length = libstring_length (content_txt);
	printf("\nLargo: %ld >> posicion: %ld", content_length, position);
	
	while ((content_txt [position] != '<') && (position < content_length))
			position++;

	if ((position >= 0) && (position < content_length))
		return position + 1;
	else
		return -1;
}


char * libxml_parse_tag_name (char * content_txt, long position)
{
	char * name = NULL;

	long blank;
	long bracket;
	long slash;
	long small = LONG_MAX;  // Maximum possible value for long

	blank   = libstring_search (content_txt + position, " ");
	bracket = libstring_search (content_txt + position, ">");
	slash   = libstring_search (content_txt + position, "/>");
/*
	char subset [501];
	libstring_subset (content_txt, position, 500, subset);
	printf("\n\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
	printf("\n%s", subset);
	printf("\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
*/
	if ((blank > 0) && (blank < small))
        small = blank;
    
    if ((bracket > 0) && (bracket < small))
    	small = bracket;

	if ((slash > 0) && (slash < small))
        small = slash;

	if ((small > 0) && (small < LONG_MAX))
	{
		name = (char *) malloc (small * sizeof (char));
		libstring_subset (content_txt, position, small, name);
	}
	else
		printf ("LIBXML: Error parsing tag name");

	if (name != NULL)
		printf ("\n1+++++++ %s ++++++++++ %ld", name, small);

	return name;
}

char * libxml_parse_tag_value (char * content_txt, long position, char * name)
{
	char * value = NULL;
	
	long bracket;
	long slash;

	bracket = libstring_search (content_txt + position, ">");
	slash   = libstring_search (content_txt + position, "/>");

	if ((bracket < slash) || (slash < 0))
	{
		char * close;
		close = (char *) malloc ((bracket + 3) * sizeof (char));

		close[0] = '<';
		close[1] = '/';
		libstring_concat (close, name);
		libstring_concat (close, ">");
		
		long pos = 0;
		long next_tag;
		pos = libstring_search (content_txt + position, close);
		next_tag = libxml_find_next_tag (content_txt, position);

		if (pos = next_tag)
		{
			value = (char *) malloc ((pos - position) * sizeof (char));
			libstring_subset (content_txt, position + bracket + 1, pos - bracket, value);
		}

		free (close);
		
		if (value != NULL)
			printf ("\n2+++++++ %s pos: %ld", value, pos);
	}

	return value;
}

xml_tag_t * libxml_parse_tag_nested (char * content_txt, long position, char * name)
{
	xml_tag_t * tag_t = NULL;

	long bracket;
	long slash;

	bracket = libstring_search (content_txt + position, ">");
	slash   = libstring_search (content_txt + position, "/>");

	if ((bracket < slash) || (slash < 0))
	{
		char * close;
		close = (char *) malloc ((bracket + 3) * sizeof (char));

		close[0] = '<';
		close[1] = '/';
		libstring_concat (close, name);
		libstring_concat (close, ">");
		
		long pos = 0;
		long next_tag;
		pos = libstring_search (content_txt + position, close);
		next_tag = libxml_find_next_tag (content_txt, position);

		if (pos > next_tag)
			libxml_parse_tag (content_txt, next_tag, tag_t);

		free (close);
		
		if (close != NULL)
			printf ("\n3+++++++ %s pos: %ld", close, pos);
	}

	return tag_t;
}

xml_tag_t * libxml_parse_tag_sibling (char * content_txt, long position, char * name)
{
	xml_tag_t * tag_t = NULL;

	long bracket;
	long slash;
	long sibling_pos = 0;

	bracket = libstring_search (content_txt + position, ">");
	slash   = libstring_search (content_txt + position, "/>");

	if ((slash > 0) && (slash < bracket))
	{
		sibling_pos = slash + 2;
	}
	else if ((bracket < slash) || (slash < 0))
	{
		char * close;
		close = (char *) malloc ((bracket + 3) * sizeof (char));

		close[0] = '<';
		close[1] = '/';
		libstring_concat (close, name);
		libstring_concat (close, ">");
		
		long pos = 0;
		long next_tag;
		pos = libstring_search (content_txt + position, close);

		sibling_pos = position + pos + bracket + 3;
		free (close);
	}

	if (sibling_pos > 0)
	{
		tag_t = (xml_tag_t *) malloc (sizeof (xml_tag_t));
		libxml_parse_tag (content_txt, sibling_pos, tag_t);
	}

	return tag_t;
}

xml_attribute_t * libxml_parse_tag_attribute (char * content_txt, long position, char * name)
{
	xml_attribute_t * attribute_t = NULL;

	long offset;
	long offset_aux;
	long length;

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
			xml_attribute_t * attribute_last_t = NULL;
			attribute_last_t = (xml_attribute_t  *) malloc (sizeof (xml_attribute_t));

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

			if (attribute_t == NULL)
				attribute_t = attribute_last_t;
			else
				attribute_t->next_attribute_t = attribute_last_t;
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
	xml_tag_t * content; 
	char * content_txt;
	long position = 0;

	content_txt = libxml_read_content (xml_txt);
	position = libxml_find_next_tag (content_txt, position);

	content = (xml_tag_t *) malloc (sizeof (xml_tag_t));

	libxml_parse_tag (content_txt, position, content);

	
	return content;
}

xml_attribute_t * libxmls_parse_instruction (char * instruction)
{
	xml_attribute_t * attribute_t = NULL;
	long offset, offset_aux, length;
	long position = 0;

printf("\n%s", instruction);

	offset = libstring_search (instruction + position, "<?");
	position = position + libstring_length ("<?");
	
	offset = libstring_search (instruction + position, " ");
	offset_aux = libstring_search (instruction + position, "?>");
	if (offset_aux <= offset)
	{
		printf ("\nLibXML: Error parsing instruction. No instructions");
		return NULL;
	}

	offset = libstring_search (instruction + position, "xml");
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

		offset = libstring_search (instruction + position, "=");
		offset_aux = libstring_search (instruction + position, "?>");
		
		if ((offset_aux <= offset) || (offset < 0))
		{
			return attribute_t;
		}
		else
		{
			xml_attribute_t * attribute_last_t = NULL;
			attribute_last_t = (xml_attribute_t  *) malloc (sizeof (xml_attribute_t));

			char * name = (char *) malloc (offset * sizeof (char));
			if (name == NULL)
			{
				printf ("\nLibXML: Error malloc");
				return attribute_t;
			}
			length = libstring_subset (instruction, position, offset, name);

			position = position + length + 1;
			offset = libstring_search (instruction + position, "\"");
			position = position + offset + 1;
			offset = libstring_search (instruction + position, "\"");

			char * value = (char *) malloc (offset * sizeof (char));
			length = libstring_subset (instruction, position, offset, value);
			position = position + offset + 1;

			attribute_last_t->name = name;
			attribute_last_t->value = value;
			attribute_last_t->next_attribute_t = NULL;

			if (attribute_t == NULL)
				attribute_t = attribute_last_t;
			else
				attribute_t->next_attribute_t = attribute_last_t;
		}
	}
	return attribute_t;
}

xml_t * libxml_init_xml_mem (char * xml_txt)
{
	char * instruction;
	xml_t * xml_mem;

	xml_mem = (xml_t *) malloc (sizeof (xml_t));

	xml_mem->instruction_t = NULL;
	xml_mem->content_t = NULL;

	instruction = libxml_read_instruction (xml_txt);
	xml_mem->instruction_t = libxmls_parse_instruction (instruction);
	
	free (instruction);
	return xml_mem;
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

/*********************************************************************************
 *                                   API
 *********************************************************************************/

xml_t * libxml_xml_to_mem (char * xml_txt)
{
	xml_tag_t * content; 
	xml_t * xml_mem;
	xml_mem = libxml_init_xml_mem (xml_txt);

	content = libxml_parse_content (xml_txt);

	return xml_mem;
}

xml_t * libxml_file_to_mem (char * xml_name)
{
	char * xml_txt;
	FILE * xml_file;
	xml_t * xml_mem;

	xml_file = libxml_open (xml_name);
	xml_txt = libxml_read (xml_file, -1);
	xml_file = libxml_close (xml_file);
	
	xml_mem = libxml_xml_to_mem (xml_txt);
	libxml_test_atributes (xml_mem->instruction_t);
	free (xml_txt);
	return xml_mem;
}



/*****************************************************************************/


int main()
{
//	FILE * xml_file;
//	long xml_length;
//	char * xml_txt;
	xml_t * xml_mem;


	xml_mem = libxml_file_to_mem (".ignore/example.xml");
	free (xml_mem);

	printf("\n");

    return 0;
}


/*
#ifdef BASURA
// xml_file = libxml_open (".ignore/fichero.txt");
	xml_file = libxml_open (".ignore/example.xml");

// xml_length = libxml_length (xml_file);

	printf ("\nEl archivo tiene un tamaño de %ld bytes", xml_length);

	xml_txt = libxml_read (xml_file, -1);
// printf ("\nFICHERO:\n%s", xml_txt);
	xml_file = libxml_close (xml_file);

char * aux;

	aux = libxml_read_instruction (xml_txt);
printf ("\nInstructions: %s", aux);
	free (aux);
	free (xml_txt);

	aux = libxml_read_content (xml_txt);
printf ("\nCONTENT:\n%s", aux);

#endif
*/

