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
#include <elf_parser.h>
#include <ev2_parser.h>
#include <ev2_view_context.h>

static
void ev_parser_iface_init(EvParserIface* iface);

/*
 * Object definition
 *
 */

G_DEFINE_TYPE_WITH_CODE
(EvElfParser,
 ev_elf_parser,
 G_TYPE_OBJECT,
 G_IMPLEMENT_INTERFACE
 (EV_TYPE_PARSER,
  ev_parser_iface_init));

static gboolean
ev_parser_iface_parse(EvParser* pself,
                      EvViewContext* view_ctx,
                      GInputStream* stream,
                      GCancellable* cancellable,
                      GError** error)
{
  EvElfParser* self = EV_ELF_PARSER(pself);
  gboolean success = TRUE;
  GError* tmp_err = NULL;

  char ident[4] = {0};
  guchar bitlen = 0;

  get_s(ident, sizeof(ident));
  if G_UNLIKELY(!memcmp(ident, "\x7f" "ELF", sizeof(ident)))
  {
    get_s(&bitlen, 1);
    goto_if_failed(bitlen == 1 || bitlen == 2);

    switch(bitlen)
    {
    case elf_bitlen_32:
      get_a(header32, sizeof(elf_header32_t), 0, G_SEEK_SET);
      goto_if_failed(header32->e_shnum > header32->e_shstrndx || header32->e_shnum == 0);
      break;
    case elf_bitlen_64:
      get_a(header64, sizeof(elf_header64_t), 0, G_SEEK_SET);
      goto_if_failed(header64->e_shnum > header64->e_shstrndx || header64->e_shnum == 0);
      break;
    }
  }
  else
  {
    g_set_error
    (error,
     EV_PARSER_ERROR,
     EV_PARSER_ERROR_UNPARSEABLE,
     "invalid ELF header\r\n");
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
void ev_elf_parser_class_init(EvElfParserClass* klass) {
}

static
void ev_elf_parser_init(EvElfParser* self) {
}
