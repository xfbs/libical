/*======================================================================
 FILE: icalduration.h
 CREATOR: eric 26 Jan 2001

 (C) COPYRIGHT 2000, Eric Busboom <eric@softwarestudio.org>
     http://www.softwarestudio.org

 This library is free software; you can redistribute it and/or modify
 it under the terms of either:

    The LGPL as published by the Free Software Foundation, version
    2.1, available at: http://www.gnu.org/licenses/lgpl-2.1.html

 Or:

    The Mozilla Public License Version 2.0. You may obtain a copy of
    the License at http://www.mozilla.org/MPL/

 The Original Code is eric. The Initial Developer of the Original
 Code is Eric Busboom
======================================================================*/

#ifndef ICALDURATION_H
#define ICALDURATION_H

/*
 * @file icalduration.h
 * @brief Methods for working with durations in iCal
 */

#include "libical_ical_export.h"
#include "icaltime.h"

/**
 * @brief A struct representing a duration
 */
struct icaldurationtype
{
    int is_neg;
    unsigned int days;
    unsigned int weeks;
    unsigned int hours;
    unsigned int minutes;
    unsigned int seconds;
};

/**
 * @brief Creates a new ::icaldurationtype from a duration in seconds.
 * @param t The duration in seconds
 * @return An ::icaldurationtype representing the duration @a t in seconds
 *
 * ### Example
 * ```c
 * // create a new icaldurationtype with a duration of 60 seconds
 * struct icaldurationtype duration;
 * duration = icaldurationtype_from_int(60);
 *
 * // verify that the duration is one minute
 * assert(duration.minutes == 1);
 * ```
 */
LIBICAL_ICAL_EXPORT struct icaldurationtype icaldurationtype_from_int(int t);

/**
 * @brief Creates a new ::icaldurationtype from a duration given as a string.
 * @param dur The duration as a string
 * @return An ::icaldurationtype representing the duration @a dur
 *
 * @par Error handling
 * When given bad input, it sets ::icalerrno to ::ICAL_MALFORMEDDATA_ERROR
 * and returnes icaldurationtype_bad_duration().
 *
 * ### Usage
 * ```c
 * // create a new icaldurationtype
 * struct icaldurationtype duration;
 * duration = icaldurationtype_from_string("+PT05M");
 *
 * // verify that it's 5 minutes
 * assert(duration.minutes == 5);
 * ```
 */
LIBICAL_ICAL_EXPORT struct icaldurationtype icaldurationtype_from_string(const char *dur);

/**
 * @brief Convert an ::icaldurationtype into the duration in seconds as `int`.
 * @param duration The duration to convert to seconds
 * @return An `int` representing the duration in seconds
 *
 * ### Usage
 * ```c
 * // create icaldurationtype with given duration
 * struct icaldurationtype duration;
 * duration = icaldurationtype_from_int(3532342);
 *
 * // get the duration in seconds and verify it
 * assert(icaldurationtype_as_int(duration) == 3532342);
 * ```
 */
LIBICAL_ICAL_EXPORT int icaldurationtype_as_int(struct icaldurationtype duration);

/**
 * @brief Convert an ::icaldurationtype into the iCal format as string.
 * @param The ::icaldurationtype to convert to iCal format
 * @return A string representing @a d in iCal format
 * @sa icaldurationtype_as_ical_string_r()
 *
 * @par Ownership
 * The string returned by this function is owned by the caller and needs to be
 * released with `free()` after it's no longer needed.
 *
 * ### Usage
 * ```c
 * // create new duration
 * struct icaldurationtype duration;
 * duration = icaldurationtype_from_int(3424224);
 *
 * // print as ical-formatted string
 * char *ical = icaldurationtype_as_ical_string(duration);
 * printf("%s\n", ical);
 *
 * // release string
 * free(ical);
 * ```
 */
LIBICAL_ICAL_EXPORT char *icaldurationtype_as_ical_string(struct icaldurationtype d);

/**
 * @brief Convert an ::icaldurationtype into the iCal format as string.
 * @param The ::icaldurationtype to convert to iCal format
 * @return A string representing @a d in iCal format
 * @sa icaldurationtype_as_ical_string()
 *
 * @par Ownership
 * The string returned by this function is owned by libical and must not be
 * released by the caller of the function.
 *
 * ### Usage
 * ```c
 * // create new duration
 * struct icaldurationtype duration;
 * duration = icaldurationtype_from_int(3424224);
 *
 * // print as ical-formatted string
 * printf("%s\n", icaldurationtype_as_ical_string(duration));
 * ```
 */
LIBICAL_ICAL_EXPORT char *icaldurationtype_as_ical_string_r(struct icaldurationtype d);
LIBICAL_ICAL_EXPORT struct icaldurationtype icaldurationtype_null_duration(void);
LIBICAL_ICAL_EXPORT struct icaldurationtype icaldurationtype_bad_duration(void);
LIBICAL_ICAL_EXPORT int icaldurationtype_is_null_duration(struct icaldurationtype d);
LIBICAL_ICAL_EXPORT int icaldurationtype_is_bad_duration(struct icaldurationtype d);

LIBICAL_ICAL_EXPORT struct icaltimetype icaltime_add(struct icaltimetype t,
                                                     struct icaldurationtype d);

LIBICAL_ICAL_EXPORT struct icaldurationtype icaltime_subtract(struct icaltimetype t1,
                                                              struct icaltimetype t2);

#endif /* !ICALDURATION_H */
