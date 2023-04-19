/**
 * @file libjxml.h
 *
 * @brief Library to work with xml files.
 *
 * A structure tree is defined to store an XML file in memory. This library provides
 * functions to convert the XML file into the tree structure and vice versa.
 *
 * @author Joseba R.G.
 *         joseba.rg@protonmail.com
 */

#ifndef _LIBJXLM_H
#define _LIBJXLM_H

#include <stdio.h>
#include <stdbool.h>

/*********************************************************************************
 *                                    STRUCTS
 *********************************************************************************/

/**
 * @brief The main XML structure representing an entire XML document.
 */
typedef struct xml_t
{
    struct xml_attribute_t * instruction_t; /**< Pointer to the XML instruction */
    struct xml_tag_t       * content_t;     /**< Pointer to the first XML tag in the list */
} xml_t;

/**
 * @brief The structure representing a single XML attribute.
 */
typedef struct xml_attribute_t
{
    char * name;                                /**< The name of the attribute */
    char * value;                               /**< The value of the attribute */
    struct xml_attribute_t * next_attribute_t;  /**< Pointer to the next attribute in the list */
} xml_attribute_t;

/**
 * @brief The structure representing an XML tag.
 */
typedef struct xml_tag_t
{
    char * name;                            /**< The name of the tag */
    char * value;                           /**< The value of the tag */
    struct xml_attribute_t * attribute_t;   /**< Pointer to the first attribute in the list */
    struct xml_tag_t       * nested_tag_t;  /**< Pointer to the first nested tag in the list */
    struct xml_tag_t       * sibling_tag_t; /**< Pointer to the next sibling tag */
} xml_tag_t;

/*********************************************************************************
 *                                      API
 *********************************************************************************/

/**
 * @brief Free memory allocated to an xml_t structure.
 *
 * This function frees the memory allocated to an xml_t structure, 
 * including the memory allocated to its attributes and content.
 *
 * @param xml_mem_t Pointer to the xml_t structure whose memory will be freed.
 *
 * @return The total number of attributes and content that have been freed.
 */
int libjxml_free_xml_mem (xml_t * xml_mem_t);

/**
 * @brief Convert an XML string into an xml_t structure.
 *
 * This function takes an XML string as input and returns an xml_t structure 
 * that represents the XML document. The xml_t structure contains pointers to 
 * the parsed instruction and content of the XML document.
 *
 * @param[in] xml_txt The XML string to be converted.
 * @return A pointer to the xml_t structure that represents the XML document.
 *
 * @note The returned xml_t structure must be freed using libjxml_free_xml_mem() 
 * when it is no longer needed.
 */
xml_t * libjxml_xml_to_mem  (char * xml_txt);

/**
 * @brief Read an XML file and convert it to an xml_t structure.
 *
 * This function opens the specified XML file, reads its contents, and converts it
 * to an xml_t structure using libjxml_xml_to_mem(). The resulting xml_t * structure
 * contains pointers to the parsed instruction and content of the XML document.
 *
 * @param[in] xml_name The name of the XML file to be read.
 * @return A pointer to the xml_t structure that represents the XML document.
 *
 * @note The returned xml_t structure must be freed using libjxml_free_xml_mem() when
 * it is no longer needed.
 */
xml_t * libjxml_file_to_mem (char * xml_name);

/**
 * @brief Write an xml_t structure to an XML file.
 *
 * This function takes an xml_t structure as input, writes its instruction and
 * content to an XML file using libjxml_write_instruction() and libjxml_write_tag(),
 * respectively, and returns a pointer to the opened file. If the 'close' parameter
 * is set to true, the file will be closed after writing is complete.
 *
 * @param[in] xml_mem_t A pointer to the xml_t structure to be written.
 * @param[in] xml_name The name of the XML file to be created.
 * @param[in] close A boolean value indicating whether to close the file after
 * writing (true) or keep it open (false).
 * @return A pointer to the opened XML file.
 *
 * @note The file returned by this function must be closed using fclose() when it 
 * is no longer needed.
 */
FILE  * libjxml_mem_to_file (xml_t * xml_mem_t, 
							 char * xml_name,
							 bool close);

#endif //_LIBJXLM_H
