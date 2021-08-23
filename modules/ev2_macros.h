/*  Copyright 2021-2022 MarcosHCK
 *  This file is part of exview2.
 *
 *  exview2 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  exview2 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with exview2. If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef __EV2_MACROS_INCLUDED__
#define __EV2_MACROS_INCLUDED__ 1
#define __GLIB_H_INSIDE__
#include <glib/gmacros.h>

#define header32 ((elf_header32_t*)&(self->header32_))
#define header64 ((elf_header64_t*)&(self->header64_))

#define failable_begin() \
GError* tmp_err = NULL; \
gboolean success = TRUE;

#define error_tag() \
_error_:
#define goto_error() \
G_STMT_START { \
  g_warn_message(G_LOG_DOMAIN, __FILE__, __LINE__, G_STRFUNC, "goto_error"); \
  success = FALSE; \
  goto _error_; \
} G_STMT_END

#define goto_if_failed(expr) \
G_STMT_START { \
  if G_LIKELY(expr) \
    {} \
  else \
  { \
    g_set_error(error, \
                EV_PARSER_ERROR, \
                EV_PARSER_ERROR_FAILED, \
                "check " #expr " failed!"); \
    goto_error(); \
  } \
} G_STMT_END


#define take_var(preffix, as, varname, where, T) \
G_STMT_START { \
  switch(header32->bitlen) \
  { \
  case elf_bitlen_32: \
    *((where)) = (T)\
    preffix##32 as varname; \
    break; \
  case elf_bitlen_64: \
    *((where)) = (T)\
    preffix##64 as varname; \
    break; \
  } \
} G_STMT_END
#define take_var_p(preffix, varname, where, T) take_var(preffix, ->, varname, where, T)
#define take_var_s(preffix, varname, where, T) take_var(preffix, ., varname, where, T)

#define get_s(b,c) \
G_STMT_START { \
  gsize read = 0; \
  gsize expected = (c); \
  g_clear_error(&tmp_err); \
  success = \
  g_input_stream_read_all(stream, (b), expected, &read, cancellable, &tmp_err); \
  if(success == FALSE) \
  { \
    g_propagate_error(error, tmp_err); \
    goto_error(); \
  } \
  if(read != expected) \
  { \
    g_set_error(error, \
                G_IO_ERROR, \
                G_IO_ERROR_FAILED, \
                "reached EOF while pending %i bytes to read\r\n", \
                expected - read); \
    goto_error(); \
  } \
} G_STMT_END
#define seek_a(o, t) \
G_STMT_START { \
  g_clear_error(&tmp_err); \
  success = \
  g_seekable_seek(G_SEEKABLE(stream), (o), (t), cancellable, &tmp_err); \
  if(success == FALSE) \
  { \
    g_propagate_error(error, tmp_err); \
    goto_error(); \
  } \
} G_STMT_END
#define get_a(b, c, o, t) \
G_STMT_START { \
  seek_a((o), (t)); \
  get_s((b), (c)); \
} G_STMT_END

#undef __GLIB_H_INSIDE__
#endif
