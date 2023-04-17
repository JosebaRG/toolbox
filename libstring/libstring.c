#include "libstring.h"

long libstring_length (char * text)
{
	long length = 0;

	while (text [length] != '\0')
		length++;
		
	return length;
}

long libstring_concat (char * text_a, char * text_b)
{
	long length_a = libstring_length(text_a);
	long length_b = libstring_length(text_b);
	long pos = 0;

	while (length_b >= 0)
	{
		text_a[length_a] = text_b[pos];
		length_a++;
		length_b--;
		pos++;
	}

	return pos-1;
}

long libstring_subset (char * text, long offset, long length, char * subset)
{
	long copied = 0;

	while (length > copied)
	{
		subset [copied] = text [offset + copied];
		copied++;
	}

	subset [copied] = '\0';

	return copied;
}

long libstring_replace (char * text, long offset, char * replace)
{
	long replaced = 0;
	long length = libstring_length(replace);

	if (length <= 0)
		return 0;

	while (length > replaced)
	{
		text [offset + replaced] = replace [replaced];
		replaced++;
	}

	return replaced;
}

int libstring_compare (char * a, char * b)
{
	long length_a = libstring_length(a);
	long length_b = libstring_length(b);

	if (length_a != length_b)
		return -1;

	for (long i=0; i<length_a; i++)
		if (a [i] != b [i])
			return -1;

	return 0;
}

long libstring_search (char * text, long offset, char * searched)
{
	long length_t = libstring_length(text + offset);
	long length_s = libstring_length(searched);

	if (length_s > length_t)
		return -1;

	char * ptr_aux = NULL;
	ptr_aux = (char *) malloc ((length_s) * sizeof (char));

	if (ptr_aux == NULL)
	{
		printf ("\nLIBSTRING: Error on malloc from search");
	}

	long position = 0;

//printf ("\n//////////\n\tBuscado: +>.%s.<+\nTexto: +>.%s.<+", searched, text);	
	while (text [offset + position] != '\0')
	{
		libstring_subset(text, offset + position, length_s, ptr_aux);
//printf ("\n++ ->.%s.<- ->.%s.<-", ptr_aux, searched);
		if (strcmp(ptr_aux, searched) == 0)
		{
			free (ptr_aux);
//printf ("\n\tFOUND\n//////////");
			return offset + position;
		}
		position ++;
	}

	free (ptr_aux);
//printf ("\n\tNOT FOUND\n//////////");
	return -1;
}

match_pattern_t libstring_match_pattern (char * string, char * match)
{
	match_pattern_t matched_t;
	
	long length_s = libstring_length(string);
	long length_m = libstring_length(match);

	/*if (length_s > length_t)
		return -1;*/

	char aux [length_m];
	strcpy(aux, "\0");
	char * ptr_aux = aux;

	long position = 0;
	
	while (string [position] != '\0')
	{
		libstring_subset(string, position, length_m, ptr_aux);
		//printf("\n++ %s - %s", ptr_aux, searched);
		if (strcmp(ptr_aux, match) == 0)
			break;
			//return position;
		position ++;
	}

	matched_t.before = (char *) malloc (position * sizeof (char));
	matched_t.matched = (char *) malloc (length_m * sizeof (char));
	matched_t.after = (char *) malloc ((length_s - (position + length_m)) * sizeof (char));

	libstring_subset(string, 0, position , matched_t.before);
	libstring_subset(string, position, length_m, matched_t.matched);
	libstring_subset(string, position + length_m, length_s - (position + length_m), matched_t.after);
	matched_t.offset = position;

	return matched_t;
}

match_pattern_t libstring_free_matched (match_pattern_t matched_t)
{
	free(matched_t.before);
	free(matched_t.matched);
	free(matched_t.after);

	return matched_t;
}

#ifdef BASURA
void main()
{
	char texto [12];
	char * ptr;
	long largo = 0;

	strcpy(texto,"hola mundo!");

	ptr = texto;

	printf("\n%s", texto);
	
	largo = libstring_length(ptr);
	printf("\nLength --> %s - %ld", ptr, largo);

	///////////////////////////////
	
	char A [20];
	char * p_A = A;
	strcpy(p_A,"Hola ");

	char B [12];
	char * p_B = B;
	strcpy(p_B,"mundo!");

	long cantidad = 0;

	cantidad = libstring_concat(p_A, p_B);

	printf("\nConcat --> %s - %ld", p_A, cantidad);

	///////////////////////////////
	
	strcpy(ptr,"hola mundo!");

	char subseted [20];
	char * p_s = subseted;
	strcpy(p_s,"Me gustan las patatas.");

	long ret_sub = 0;

	ret_sub = libstring_subset(ptr,3,5,p_s);

	printf("\nSubset --> %s - %ld", p_s, ret_sub);

	///////////////////////////////
	
	char Ar [20];
	char * pr_A = Ar;
	strcpy(pr_A,"AAAAAAAAAAAAAAAAAAAAAA");

	char Br [12];
	char * pr_B = Br;
	strcpy(pr_B,"BB");

	cantidad = 0;

	cantidad = libstring_replace(pr_A, pr_B, 3);

	printf("\nReplace -> %s - %ld", pr_A, cantidad);

	///////////////////////////////
	
	long searched_pos = 0;

	searched_pos = ibstring_search(pr_A, pr_B);

	printf("\nSearch --> %ld", searched_pos);

	///////////////////////////////

	match_pattern_t matched_t;

	strcpy(pr_A,"12345678");
	strcpy(pr_B,"34");

	matched_t = libstring_match_pattern(pr_A, pr_B);

	printf("\nMatch p -> %s", matched_t.before);
	printf("\nMatch p -> %s", matched_t.matched);
	printf("\nMatch p -> %s", matched_t.after);
	printf("\nMatch p -> %ld", matched_t.offset);
}

#endif //BASURA
