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
#include <config.h>
#include <pe_parser.h>

static
void ev_parser_iface_init(EvParserIface* iface);

/*
 * Object definition
 *
 */

G_DEFINE_TYPE_WITH_CODE
(EvPeParser,
 ev_pe_parser,
 G_TYPE_OBJECT,
 G_IMPLEMENT_INTERFACE
 (EV_TYPE_PARSER,
  ev_parser_iface_init));

#define parse_call(func, ...) \
G_STMT_START { \
  success = \
  (func) (__VA_ARGS__, &tmp_err); \
  if(success == FALSE) \
  { \
    g_propagate_error(error, tmp_err); \
    goto_error(); \
  } \
} G_STMT_END

static gchar*
dumphex(gconstpointer buffer_,
        gsize size)
{
  static const gint bytes_per_line = 16;
  static const gchar* chars = "0123456789abcdef";
  guchar c, *buffer = (guchar*) buffer_;

  GString* string =
  g_string_sized_new((size * 3) + (2 * (size / bytes_per_line)));

  do
  {
    gsize i, take = MIN(bytes_per_line, size);
    size -= take;

    for(i = 0;i < take;i++, buffer++)
    {
      c = *buffer;
      g_string_append_c(string, chars[(c >> 4) & 0xf]);
      g_string_append_c(string, chars[       c & 0xf]);
      g_string_append_c(string, ' ');
    }

    g_string_append(string, "\r\n");
  }
  while(size > 0);
return g_string_free(string, FALSE);
}

static gboolean
ev_parser_iface_parse(EvParser* pself,
                      EvViewContext* view_ctx,
                      GInputStream* stream,
                      GCancellable* cancellable,
                      GError** error)
{
  EvPeParser* self = EV_PE_PARSER(pself);
  gboolean success = TRUE;
  GError* tmp_err = NULL;

  char mz[2] = {0};
  get_s(mz, sizeof(mz));
  if G_UNLIKELY(!memcmp(mz, "MZ", sizeof(mz)))
  {
    get_a(&(self->dos), sizeof(pe_dos_header_t), 0, G_SEEK_SET);

    pe_sign_t sign;
    get_a(&sign, sizeof(sign), self->dos.e_lfanew, G_SEEK_SET);
    if G_UNLIKELY(!memcmp(&(sign.Magic), "PE", sizeof(sign.Magic)))
    {
      get_s(&(self->pe.header), sizeof(self->pe.header));
      memcpy(&(self->pe.sign), &sign, sizeof(sign));

      if G_LIKELY(self->pe.header.SizeOfOptionalHeader > 0)
      {
        get_s(&(self->opt), self->pe.header.SizeOfOptionalHeader);
        if(self->opt._32.Magic == 0x10b)
          self->pe.header.Characteristics |= pe_flag_32bit_machine;
      }

      self->sections = g_seekable_tell(G_SEEKABLE(stream));
      self->bitlen = (self->pe.header.Characteristics & pe_flag_32bit_machine) ? pe_bitlen_32 : pe_bitlen_64;

      parse_call(_pe_parser_parse_dos_header, self, view_ctx, stream, cancellable);
      parse_call( _pe_parser_parse_pe_header, self, view_ctx, stream, cancellable);
      parse_call(_pe_parser_parse_opt_header, self, view_ctx, stream, cancellable);
      parse_call(  _pe_parser_parse_sections, self, view_ctx, stream, cancellable);
      parse_call(   _pe_parser_parse_imports, self, view_ctx, stream, cancellable);
      parse_call(   _pe_parser_parse_exports, self, view_ctx, stream, cancellable);
    }
    else
    {
      g_set_error
      (error,
       EV_PARSER_ERROR,
       EV_PARSER_ERROR_UNPARSEABLE,
       "invalid PE header\r\n");
      goto_error();
    }
  }
  else
  {
    g_set_error
    (error,
     EV_PARSER_ERROR,
     EV_PARSER_ERROR_UNPARSEABLE,
     "invalid DOS header\r\n");
    goto_error();
  }

error_tag()
return success;
}

static
void ev_parser_iface_init(EvParserIface* iface) {
  iface->parse = ev_parser_iface_parse;
}

static
void ev_pe_parser_class_init(EvPeParserClass* klass) {
}

static
void ev_pe_parser_init(EvPeParser* self) {
}
