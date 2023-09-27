/**
 * @file libjlog.c
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

#include <string.h>
#include <time.h>

#include "libjlog.h"

/*********************************************************************************
 *                                    STRUCTS
 *********************************************************************************/

/**
 * @brief Structure representing a log entry in the log.
 *
 * The `libjlog_t` structure defines a log entry in the log, consisting of
 * a log message and a pointer to the next log entry.
 */
typedef struct libjlog_t
{
    char libjlog_msg [LIBJLOG_TOTAL_LENGTH];
    struct libjlog_t * libjlog_next;
} libjlog_t;

/*********************************************************************************
 *                               PRIVATE GLOBALS
 *********************************************************************************/

/**
 * @brief Array of log entries in the log.
 *
 * The `libjlog_array` is an array of `libjlog_t` structures representing
 * log entries in the log. It provides storage for a maximum number of log
 * entries defined by `LIBJLOG_MAX_LOGS`.
 */
libjlog_t libjlog_array [LIBJLOG_MAX_LOGS];

/**
 * @brief Pointer to the current write position in the log.
 *
 * The `libjlog_write` pointer points to the current write position in the log.
 * New log messages are written at this position in the log array.
 */
libjlog_t * libjlog_write;

/**
 * @brief Pointer to the current read position in the log.
 *
 * The `libjlog_read` pointer points to the current read position in the log.
 * Log messages can be read from this position in the log array.
 */
libjlog_t * libjlog_read;

/**
 * @brief Flag indicating if log messages have been lost.
 *
 * The `libjlog_lost` flag is a boolean value indicating whether any log messages
 * have been lost. It is set to true if log messages have been overwritten due to
 * a full log buffer.
 */
bool libjlog_lost;

/*********************************************************************************
 *                                   API
 *********************************************************************************/

void libjlog_init ()
{
    for (int i = 0; i < (LIBJLOG_MAX_LOGS - 1); i++)
	{
        libjlog_array [i].libjlog_next = &libjlog_array [i + 1];
		strcpy(libjlog_array [i].libjlog_msg, "\0");
    }

	libjlog_array [LIBJLOG_MAX_LOGS - 1].libjlog_next = &libjlog_array [0];
	strcpy(libjlog_array [LIBJLOG_MAX_LOGS - 1].libjlog_msg, "Log initialization");

	libjlog_write = &libjlog_array [LIBJLOG_MAX_LOGS - 1];
	libjlog_read  = &libjlog_array [LIBJLOG_MAX_LOGS - 1];

	libjlog_lost = false;
}

void libjlog_get_last (char libjlog_msg [LIBJLOG_LENGTH])
{
	strcpy (libjlog_msg, libjlog_write->libjlog_msg);
}

bool libjlog_lost_elements ()
{
	return libjlog_lost;
}

void libjlog_clear_lost_elements ()
{
	libjlog_lost = false;
}

bool libjlog_is_empty ()
{
	if ((libjlog_read == libjlog_write) && (libjlog_write->libjlog_msg [0] == '\0'))
		return true;
	else
		return false;
}

bool libjlog_is_full ()
{
	if ((libjlog_write == libjlog_read) && !libjlog_is_empty())
		return true;
	else
		return false;
}

uint32_t libjlog_count_elements ()
{
	uint32_t quantity = 0;

	if (!libjlog_is_empty())
	{
		libjlog_t * libjlog_aux;
		libjlog_aux = libjlog_read;

		if (libjlog_read->libjlog_msg [0] != '\0')
			quantity = 1;		

		while (libjlog_aux == libjlog_write)
		{
			quantity++;
			libjlog_aux = libjlog_aux->libjlog_next;
		}
	}

	return quantity;
}

void libjlog_read_element (char libjlog_msg [LIBJLOG_TOTAL_LENGTH])
{
	strcpy (libjlog_msg, libjlog_read->libjlog_msg);
	strcpy (libjlog_read->libjlog_msg, "\0");

	if (libjlog_read != libjlog_write)
		libjlog_read = libjlog_read->libjlog_next;
}

void libjlog (const char full_location [LIBJLOG_LOCATION_LEN], uint32_t line, const char* format, ...)
{
	va_list args;
	va_start (args, format);
	int size = vsnprintf (NULL, 0, format, args);
	va_end (args);

	char libjlog_msg_1 [LIBJLOG_LENGTH];
	char libjlog_msg_2 [LIBJLOG_TOTAL_LENGTH];

	va_start (args, format);
	vsnprintf (libjlog_msg_1, size + 1, format, args);
	va_end (args);

	uint32_t line_len;
	int16_t whitespaces;

	char short_location [LIBJLOG_LOCATION_LEN] = "\0";
	strcpy (short_location, strrchr(full_location, '/') + 1);

	if (strlen (short_location) <= 0)
		strcpy (short_location, full_location);

	line_len = snprintf(NULL, 0, "%ld", line);
	whitespaces = LIBJLOG_LOCATION_LEN - strlen (short_location) - line_len - strlen ("()");

	char datetime [LIBJLOG_DATE_LEN] = "UNINITIALIZED DATE";

	time_t currentTime;
	struct tm* dateInfo;
	time(&currentTime);
	dateInfo = localtime(&currentTime);

	sprintf (datetime,
			 "%d/%02d/%02d %02d:%02d:%02d", 
			 dateInfo.tm_year,
			 dateInfo.tm_mon,
			 dateInfo.tm_mday,
			 dateInfo.tm_hour,
			 dateInfo.tm_min,
			 dateInfo.tm_sec);
	
	sprintf (libjlog_msg_2,
			 "%s - %s(%ld)%*s - %s",
			 datetime,
			 short_location,
			 line,
			 whitespaces,
			 "",
			 libjlog_msg_1);

	libjlog_write_element (libjlog_msg_2);
}

void libjlog_write_element (char libjlog_msg [LIBJLOG_TOTAL_LENGTH])
{
	if (!libjlog_is_empty())
		libjlog_write = libjlog_write->libjlog_next;

	if ((libjlog_write == libjlog_read) && !libjlog_is_empty())
	{
		libjlog_lost = true;
		libjlog_read = libjlog_read->libjlog_next;
	}

	strcpy(libjlog_write->libjlog_msg, libjlog_msg);
}
