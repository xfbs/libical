/*======================================================================
 FILE: icalmemory.h
 CREATOR: eric 30 June 1999

 (C) COPYRIGHT 2000, Eric Busboom <eric@softwarestudio.org>
     http://www.softwarestudio.org

 This library is free software; you can redistribute it and/or modify
 it under the terms of either:

    The LGPL as published by the Free Software Foundation, version
    2.1, available at: http://www.gnu.org/licenses/lgpl-2.1.html

 Or:

    The Mozilla Public License Version 2.0. You may obtain a copy of
    the License at http://www.mozilla.org/MPL/

 The Initial Developer of the Original Code is Eric Busboom
======================================================================*/

#ifndef ICALMEMORY_H
#define ICALMEMORY_H

/**
 * @file icalmemory.h
 * @brief Memory management from libical.
 *
 * Libical manages it's own memory with these functions. These enable temporary
 * buffers to be returned by library calls without requiring the user to
 * manually deallocate them.  Temporary memory is kept in a ring buffer and
 * managed library-internally.
 */

#include "libical_ical_export.h"

/**
 * @brief Creates a new temporary buffer on the ring and returns it.
 * @param size How big (in bytes) the buffer should be
 * @return A pointer to the newly created buffer on the ring
 *
 * Creates a temporary buffer on the ring. Regardless of what @a size you
 * specify, the buffer will always be at least ::MIN_BUFFER_SIZE big, and it
 * will be zeroed out.
 *
 * @par Error handling
 * If there is a problem allocating memory for the buffer, it sets ::icalerrno
 * to ::ICAL_NEWFAILED_ERROR and returns `NULL`.
 *
 * @par Ownership
 * The returned buffer is owned by icalmemory. It must not be freed by the
 * caller and the returned memory will be automatically reclaimed as more items
 * are added to the ring buffer.
 *
 * ### Usage
 * ```c
 * char *str = icalmemory_tmp_buffer(256);
 * strcpy(str, "some data");
 *
 * // use str
 * ```
 */
LIBICAL_ICAL_EXPORT void *icalmemory_tmp_buffer(size_t size);

/**
 * @brief Creates a copy of the given string, stored on the ring buffer, and
 *  returns it.
 * @param str The string to copy
 * @return A copy of @a str, which has been placed on the ring buffer for
 *  automatic reclamation.
 *
 * @par Error handling
 * The passed string @a str must not be `NULL`, otherwise a segfault might
 * ensue, since the routine calls `strlen()` on it.
 *
 * @par Ownership
 * The returned string is owned by icalmemory. It must not be freed by the
 * caller, and it will be automatically reclaimed as more items are added to the
 * buffer.
 *
 * ### Usage
 * ```c
 * const char *str = "Example string";
 * char *tmp_copy = icalmemory_tmp_copy(str);
 * ```
 */
LIBICAL_ICAL_EXPORT char *icalmemory_tmp_copy(const char *str);

/**
 * @brief Add an externally allocated buffer to the ring.
 * @param buf The externally allocated buffer to add to the ring
 *
 * Adds an externally allocated buffer to the ring. This ensures that libical
 * will `free()` the buffer automatically, either after ::BUFFER_RING_SIZE other
 * buffers have been created or added, or after ::icalmemory_free_ring() has
 * been called.
 *
 * @par Error handling
 * No error is raised if @a buf is `NULL`.
 *
 * @par Ownership
 * After adding @a buf to the ring, it becomes owned by icalmemory and must not
 * be `free()`d manually anymore, it leads to a double-`free()` when icalmemory
 * reclaims the memory.
 *
 * ### Usage
 * ```c
 * char *buf = calloc(256, sizeof(char));
 *
 * icalmemory_add_tmp_buffer(buf);
 * ```
 */
LIBICAL_ICAL_EXPORT void icalmemory_add_tmp_buffer(void *buf);

/**
 * @brief Free all memory used in the ring
 *
 * Frees all memory used in the ring. Depending on if ::HAVE_PTHREAD is set or
 * not, the ring buffer is allocated on a per-thread basis, meaning that if all
 * rings are to be released, it must be called once in every thread.
 *
 * ### Usage
 * ``` c
 * void *buf = icalmemory_tmp_buffer(256);
 *
 * // use buf
 *
 * // release buf and all other memory in the ring buffer
 * icalmemory_free_ring();
 * ```
 */
LIBICAL_ICAL_EXPORT void icalmemory_free_ring(void);

/* Non-tmp buffers must be freed. These are mostly wrappers around
 * malloc, etc, but are used so the caller can change the memory
 * allocators in a future version of the library */

LIBICAL_ICAL_EXPORT void *icalmemory_new_buffer(size_t size);
LIBICAL_ICAL_EXPORT void *icalmemory_resize_buffer(void *buf, size_t size);
LIBICAL_ICAL_EXPORT void icalmemory_free_buffer(void *buf);

/**
   icalmemory_append_string will copy the string 'string' to the
   buffer 'buf' starting at position 'pos', reallocing 'buf' if it is
   too small. 'buf_size' is the size of 'buf' and will be changed if
   'buf' is reallocated. 'pos' will point to the last byte of the new
   string in 'buf', usually a '\0' */

/* THESE ROUTINES CAN NOT BE USED ON TMP BUFFERS. Only use them on
   normally allocated memory, or on buffers created from
   icalmemory_new_buffer, never with buffers created by
   icalmemory_tmp_buffer. If icalmemory_append_string has to resize a
   buffer on the ring, the ring will loose track of it an you will
   have memory problems. */

LIBICAL_ICAL_EXPORT void icalmemory_append_string(char **buf, char **pos, size_t *buf_size,
                                                  const char *string);

/**  icalmemory_append_char is similar, but is appends a character instead of a string */
LIBICAL_ICAL_EXPORT void icalmemory_append_char(char **buf, char **pos, size_t *buf_size, char ch);

/** A wrapper around strdup. Partly to trap calls to strdup, partly
    because in -ansi, gcc on Red Hat claims that strdup is undeclared */
LIBICAL_ICAL_EXPORT char *icalmemory_strdup(const char *s);

#endif /* !ICALMEMORY_H */
