#include <stdlib.h>
#include <limits.h>

#include "libxml.h"
#include "libstring.h"

//borrame
#include <unistd.h>

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
 *                                  FREE MEMORY
 *********************************************************************************/

long free_attribute (xml_attribute_t * attribute_t)
{
	long quantity;

	if (attribute_t != NULL)
	{
		free (attribute_t->name);
		free (attribute_t->value);

		quantity = free_attribute (attribute_t->next_attribute_t);
		quantity++;
	}
	else
	{
		quantity = 0;
	}

	return quantity;
}

long free_tag (xml_tag_t * tag_t)
{
	long quantity;
	long aux;

	if (tag_t != NULL)
	{
		free (tag_t->name);
		free (tag_t->value);

		quantity = free_attribute (tag_t->attribute_t);
		aux = free_tag (tag_t->nested_tag_t);
		quantity = quantity + aux;
		aux = free_tag (tag_t->sibling_tag_t);
		quantity = quantity + aux;
		quantity++;
	}
	else
	{
		quantity = 0;
	}

	return quantity;
}

/*********************************************************************************
 *                             MEM TO FILE FUNCTIONS
 *********************************************************************************/




/*********************************************************************************
 *                             FILE TO MEM FUNCTIONS
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
	char * instruction;

	long inst_start;
	long inst_end;
	long inst_length;
	
	inst_start = libstring_search (xml_txt, "<?");
	inst_end   = libstring_search (xml_txt, "?>");
	inst_length = inst_end - inst_start + 2; // two characters extra for "?>"

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
//	printf("\nLargo: %ld >> posicion: %ld", content_length, position);
	
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
printf ("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> %s", name);
	}
	else
		printf ("\nLIBXML: Error parsing tag name");

	//if (name != NULL)
		//printf ("\n1+++++++ >%s< ++++++++++ %ld", name, small);

	return name;
}

char * libxml_parse_tag_value (char * content_txt, long position, char * name)
{
	char * value = NULL;
	
	long bracket;
	long slash;

	bracket = libstring_search (content_txt + position, ">");
	slash   = libstring_search (content_txt + position, "/>");

//////////////printf ("\n2+++++++ bracket: %ld slash: %ld", bracket, slash);

	if ((bracket < slash) || (slash < 0))
	{
		char * close;
		long name_length;
		name_length = libstring_length (name);
		close = (char *) malloc ((name_length + 3) * sizeof (char)); // "</ >" three characters

		close[0] = '<';
		close[1] = '/';
		close[2] = '\0';
		libstring_concat (close, name);
		libstring_concat (close, ">");
		
		long close_pos;
		long next_tag;
		long start_pos;

		//start_pos = position + libstring_length (name) + 1;
		start_pos = position + bracket + 1;

		close_pos = libstring_search (content_txt + start_pos, close);
		next_tag = libxml_find_next_tag (content_txt, start_pos);
//printf ("\n:::::::::::: close_pos: %ld next_tag: %ld position: %ld", close_pos, next_tag, position);
/*
char texto [10];
libstring_subset (content_txt, start_pos, close_pos, texto);
printf ("\nclose: -%s-", texto);
libstring_subset (content_txt, start_pos, next_tag - start_pos - 1, texto);
printf ("\nclose: -%s-", texto);
printf ("\nclose: -%ld-", next_tag - start_pos - 1);
*/
		long length;
		length = next_tag - start_pos - 1;

		if (close_pos == length)
		{
//printf ("\n::::: >%s< ::::::: %ld", name, length);
			if (length > 0)
			{
				value = (char *) malloc (length * sizeof (char));
				libstring_subset (content_txt, start_pos, length, value);
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

		//if (value != NULL)
//printf ("\n2+++++++ >%s< position: %ld bracket: %ld pos: %ld", value, position, bracket, close_pos);
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

	bracket = libstring_search (content_txt + position, ">");
	slash   = libstring_search (content_txt + position, "/>");

	if ((bracket < slash) || (slash < 0))
	{
		char * close;
		long name_length;
		name_length = libstring_length (name);
		close = (char *) malloc ((name_length + 3) * sizeof (char)); // "</ >" three characters
		close[0] = '<';
		close[1] = '/';
		close[2] = '\0';
		libstring_concat (close, name);
		libstring_concat (close, ">");
//printf ("\n3------- +%s+", close);
/*		
		long close_pos;
		long next_tag;
		close_pos = libstring_search (content_txt + position, close);
		next_tag = libxml_find_next_tag (content_txt, position);*/
	
		long close_pos;
		long next_tag;
		long start_pos;

		start_pos = position + libstring_length (name) +1 ;

		close_pos = libstring_search (content_txt + start_pos, close);
		next_tag = libxml_find_next_tag (content_txt, start_pos);
//printf ("\n3+++++++ +%s+ pos: %ld next: %ld", close, close_pos, next_tag);

		if (close_pos > (next_tag - start_pos - 1))
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

	bracket = libstring_search (content_txt + position, ">");
	slash   = libstring_search (content_txt + position, "/>");

	if ((slash > 0) && (slash <= bracket))
	{
		sibling_pos = position + slash + 2; // "/>" two characters
	}
	else if ((bracket < slash) || (slash < 0))
	{
		char * close;
		long name_length;
		name_length = libstring_length (name);
		close = (char *) malloc ((name_length + 3) * sizeof (char)); // "</ >" three characters

		close[0] = '<';
		close[1] = '/';
		close[2] = '\0';
		libstring_concat (close, name);
		libstring_concat (close, ">");

		long close_pos;
		close_pos = libstring_search (content_txt + position, close);
//printf ("\n4------- name: +%s+ close: +%s+ pos: %ld", name, close, close_pos);

		sibling_pos = position + close_pos + name_length + 3;

//printf ("\n4------- %s", content_txt + sibling_pos);
		free (close);
	}
/*
char aux [20];
libstring_subset (content_txt, sibling_pos, 20, aux);
printf ("\n****%s****", aux);
*/
//printf ("\n4++++++ position: %ld bracket: %ld slash: %ld sibling: %ld", position, bracket, slash, sibling_pos);

	if (sibling_pos > 0)
	{
		long next_tag;
		long next_close;
		next_tag = libxml_find_next_tag (content_txt, sibling_pos);
		next_close = libstring_search (content_txt + sibling_pos, "</");
//printf ("\n4------- next_tag: %ld next_close: %ld", next_tag, next_close);

		if (next_tag < 0)
		{
			printf ("\n%s has no sibling (3)", name);
			return tag_t;
		}
			

		if (next_close > 0)
			next_close = next_close + sibling_pos + 1;

//printf ("\n4------- next_tag: %ld next_close: %ld", next_tag, next_close);

//printf ("\n4--++--- %s", content_txt + next_tag);
		
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
			xml_attribute_t * attribute_last_t;
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

//printf ("\n5----- name: >%s< value: >%s<", name, value);

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
sleep (0);

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

xml_attribute_t * libxmls_parse_instruction (char * instruction)
{
	xml_attribute_t * attribute_t = NULL;
	long offset;
	long offset_aux;
	long length;
	long position = 0;

//printf("\n%s", instruction);

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
	xml_t * xml_mem_t;

	xml_mem_t = (xml_t *) malloc (sizeof (xml_t));

	xml_mem_t->instruction_t = NULL;
	xml_mem_t->content_t = NULL;

	instruction = libxml_read_instruction (xml_txt);
	xml_mem_t->instruction_t = libxmls_parse_instruction (instruction);
	
	free (instruction);
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

/*********************************************************************************
 *                                   API
 *********************************************************************************/

xml_t * libxml_xml_to_mem (char * xml_txt)
{
	xml_t * xml_mem_t;

	xml_mem_t = libxml_init_xml_mem (xml_txt);
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

long free_xml_mem (xml_t * xml_mem_t)
{
	long quantity;
	long aux;

	if (xml_mem_t != NULL)
	{
		quantity = free_attribute (xml_mem_t->instruction_t);
		aux = free_tag (xml_mem_t->content_t);
		quantity = quantity + aux;
	}
	else
	{
		quantity = 0;
	}

	return quantity;
}


/*****************************************************************************/


int main()
{
//	FILE * xml_file;
//	long xml_length;
//	char * xml_txt;
	xml_t * xml_mem_t;
	long quantity;

while (1)
{
	xml_mem_t = libxml_file_to_mem (".ignore/example.xml");
//	libxml_test_atributes (xml_mem_t->instruction_t);
	quantity = free_xml_mem (xml_mem_t);

	printf ("\n\n++++++++++++++++++++++++++++++++++++++++++++++++++++ %ld\n\n", quantity);

	printf("\n");
}
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

