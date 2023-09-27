 /**
 * @file libjlog.h
 *
 * @brief Library to have a log.
 *
 * A fixed number of elements are defined to form a circular buffer where
 * a finite amount of log elements are stored. Only the latest log elements
 * are retained, equal to the number of buffer elements defined. This avoids
 * dynamic memory allocation.
 *
 * @author Joseba R.G.
 *         joseba.rg@protonmail.com
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _LIBJLOG_H
#define _LIBJLOG_H

#include <stdio.h>
#include <stdarg.h>

#include "libjtypes.h"

/*********************************************************************************
 *                                 DEFINITIONS
 *********************************************************************************/

/**
 * @def LIBJLOG_MAX_LOGS
 *
 * @brief Maximum number of logs supported in the log system.
 *
 * This macro defines the maximum number of logs that can be stored in
 * the log system. Adjusting this value allows controlling the capacity
 * of the log system.
 *
 * @note Ensure that the value of `LIBJLOG_MAX_LOGS` is set appropriately
 * to meet the application requirements and available resources.
 */
#define LIBJLOG_MAX_LOGS 10

/**
 * @def LIBJLOG_LENGTH
 *
 * @brief Maximum length of an individual log message in the log system.
 *
 * This macro defines the maximum length (in characters) of an individual
 * log message that can be stored in the log system. It represents the
 * buffer size allocated for each log message.
 *
 * @note Ensure that the value of `LIBJLOG_LENGTH` is set appropriately to
 * accommodate the expected length of log messages in the application.
 */
#define LIBJLOG_LENGTH 128

/**
 * @brief Defines the length of the date string in the log.
 *
 * This macro specifies the maximum length of the date string in the log.
 * The date string represents the timestamp associated with each log message.
 *
 * By defining the length using this macro, it ensures that sufficient space
 * is allocated for the date string.
 */
#define LIBJLOG_DATE_LEN 23

/**
 * @brief Defines the length of the location string in the log.
 *
 * This macro specifies the maximum length of the location string in the log.
 * The location string represents the source file and line number from where
 * each log message is created.
 *
 * By defining the length using this macro, it ensures that sufficient space is
 * allocated for the location string.
 */
#define LIBJLOG_LOCATION_LEN 20

/**
 * @brief Total length of the log message, including date, location
 * and additional characters.
 * 
 * This macro defines the total length of the log message, taking into account
 * the length of the log message itself, the date, the location, and any additional
 * characters or separators.
 *
 * The total length is calculated by summing the individual lengths and adding 6 to
 * account for additional characters of the two " - " separators.
 */
#define LIBJLOG_TOTAL_LENGTH (LIBJLOG_LENGTH + LIBJLOG_DATE_LEN + LIBJLOG_LOCATION_LEN + 6)

/*********************************************************************************
 *                                   MACROS
 *********************************************************************************/

/**
 * @brief Macro to print an log message.
 *
 * The macro expands to a call to the `libjlog()` function, providing the
 * specified parameters. It allows convenient logging of messages with the
 * current date, location, and line number.
 *
 * @param ... The variadic arguments containing the log message format and
 * optional arguments.
 */
#define PRINT_LIBJLOG(...) libjlog(__FILE__, __LINE__, __VA_ARGS__);

/*********************************************************************************
 *                                 DECLARATIONS
 *********************************************************************************/

/**
 * @brief Initializes the log system.
 *
 * This function initializes the circular buffer for storing log elements
 * in the log system. It sets up the circular buffer with a fixed number of
 * elements defined by `LIBJLOG_MAX_LOGS`.
 */
void libjlog_init ();

/**
 * @brief Retrieves the last log message from the log system.
 *
 * This function retrieves the last log message stored in the log system
 * and copies it into the provided `libjlog_msg` buffer. The `libjlog_write`
 * pointer is used to access the last log element in the circular buffer.
 *
 * @param[out] libjlog_msg The buffer where the last log message will be copied.
 */
void libjlog_get_last (char libjlog_msg [LIBJLOG_TOTAL_LENGTH]);

/**
 * @brief Checks if any log elements have been lost in the log system.
 *
 * This function checks the `libjlog_lost` flag to determine if any log 
 * elements have been lost in the log system.
 *
 * @return @c true if log elements have been lost, @c false otherwise.
 */
bool libjlog_lost_elements ();

/**
 * @brief Clears the flag indicating lost log elements in the log system.
 *
 * This function sets the `libjlog_lost` flag to `false`, indicating that no
 * log elements have been lost, in order to detect new log message losses.
 */
void libjlog_clear_lost_elements ();

/**
 * @brief Checks if the log system is empty.
 *
 * This function checks if the log system is empty.
 *
 * @return @c true if the log system is empty, @c false otherwise.
 */
bool libjlog_is_empty ();

/**
 * @brief Checks if the log system is full.
 *
 * This function checks if the log system is full.
 *
 * @return @c true if the log system is full, @c false otherwise.
 */
bool libjlog_is_full ();

/**
 * @brief Retrieves the count of log elements in the log system.
 *
 * This function counts the number of log elements in the log system.
 *
 * @return The count of log elements in the log system.
 */
uint32_t libjlog_count_elements ();

/**
 * @brief Reads a log element from the log system.
 *
 * This function reads the log message from the current log element pointed
 * to by the `libjlog_read` pointer. It copies the log message to the specified
 * `libjlog_msg` buffer and clears the log message in the current log element
 * by setting it to the null character '\0'.
 *
 * @param[out] libjlog_msg The buffer to store the read log message.
 */
void libjlog_read_element (char libjlog_msg [LIBJLOG_TOTAL_LENGTH]);

/**
 * @brief Writes a formatted log message to the log system.
 *
 * This function formats a log message using the specified date, location,
 * line number, and format string. The formatted log message is then written
 * to the log system by calling the `libjlog_write_element` function.
 *
 * @param[in] date The date string associated with the log message.
 * @param[in] location The source file location where the log message is created.
 * @param[in] line The line number in the source file where the log message is created. 
 * @param[in] format The format string for the log message, followed by optional arguments.
 *
 * @note This function must only be called from the "PRINT_LIBJLOG" macro.
 */
void libjlog (const char full_location [LIBJLOG_LOCATION_LEN], uint32_t line, const char* format, ...);

/**
 * @brief Writes a log element to the log system.
 *
 * This function writes a log element with the specified log message to 
 * the log system. It updates the `libjlog_write` pointer to the next log
 * element in the circular buffer.
 *
 * If the `libjlog_write` pointer reaches the `libjlog_read` pointer and the
 * log system is not empty, it sets the `libjlog_lost` flag to true and updates
 * the `libjlog_read` pointer to the next log element.
 *
 * @param[in] libjlog_msg The log message to be written to the log element.
 */
void libjlog_write_element (char libjlog_msg [LIBJLOG_LENGTH]);

#endif //_LIBJLOG_H

#ifdef __cplusplus
}
#endif
