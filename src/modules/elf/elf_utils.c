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

gboolean
_elf_parser_get_segment_entry(EvElfParser* self,
                              guint32 idx,
                              elf_phent64_t* ent,
                              GInputStream* stream,
                              GCancellable* cancellable,
                              GError** error)
{
  gboolean success = TRUE;
  GError* tmp_err = NULL;

  switch(header32->bitlen)
  {
  case elf_bitlen_32:
    goto_if_failed(header32->e_phnum > idx);
    get_a(ent, header32->e_phentsize, header32->e_phoff + (idx * header32->e_phentsize), G_SEEK_SET);
    break;
  case elf_bitlen_64:
    goto_if_failed(header64->e_phnum > idx);
    get_a(ent, header64->e_phentsize, header64->e_phoff + (idx * header64->e_phentsize), G_SEEK_SET);
    break;
  }

error_tag()
return success;
}

gboolean
_elf_parser_get_section_entry(EvElfParser* self,
                              guint32 idx,
                              elf_shent64_t* ent,
                              GInputStream* stream,
                              GCancellable* cancellable,
                              GError** error)
{
  gboolean success = TRUE;
  GError* tmp_err = NULL;

  switch(header32->bitlen)
  {
  case elf_bitlen_32:
    goto_if_failed(header32->e_shnum > idx);
    get_a(ent, header32->e_shentsize, header32->e_shoff + (idx * header32->e_shentsize), G_SEEK_SET);
    break;
  case elf_bitlen_64:
    goto_if_failed(header64->e_shnum > idx);
    get_a(ent, header64->e_shentsize, header64->e_shoff + (idx * header64->e_shentsize), G_SEEK_SET);
    break;
  }

error_tag()
return success;
}

G_DEFINE_QUARK(elf-parser-cached-string-table-quark,
               elf_parser_cached_string_table);

static gboolean
load_table(EvElfParser* self,
           GInputStream* stream,
           GCancellable* cancellable,
           GError** error)
{
  gboolean success = TRUE;
  GError* tmp_err = NULL;

  union {
    elf_shent32_t _32;
    elf_shent64_t _64;
  } ents;
  gchar* buff;
  gsize size;

  switch(header32->bitlen)
  {
  case elf_bitlen_32:
    success =
    _elf_parser_get_section_entry(self, header32->e_shstrndx, &(ents._64), stream, cancellable, &tmp_err);
    if G_UNLIKELY(tmp_err != NULL)
    {
      g_propagate_error(error, tmp_err);
      goto_error();
    }

    size = ents._32.sh_size;
    buff = g_malloc(size);
    get_a(buff, size, ents._32.sh_offset, G_SEEK_SET);
    break;
  case elf_bitlen_64:
    success =
    _elf_parser_get_section_entry(self, header64->e_shstrndx, &(ents._64), stream, cancellable, &tmp_err);
    if G_UNLIKELY(tmp_err != NULL)
    {
      g_propagate_error(error, tmp_err);
      goto_error();
    }

    size = ents._64.sh_size;
    buff = g_malloc(size);
    get_a(buff, size, ents._64.sh_offset, G_SEEK_SET);
    break;
  }

  g_object_set_qdata_full(G_OBJECT(self),
                          elf_parser_cached_string_table_quark(),
                          g_bytes_new_take(buff, size),
                          (GDestroyNotify)g_bytes_unref);
error_tag()
return success;
}

gchar*
_elf_parser_get_string(EvElfParser* self,
                       guint32 sh_name,
                       GInputStream* stream,
                       GCancellable* cancellable,
                       GError** error)
{
  failable_begin();
  gchar* return_ = NULL;

  GBytes* table =
  g_object_get_qdata(G_OBJECT(self), elf_parser_cached_string_table_quark());
  if(table == NULL)
  {
    success = load_table(self, stream, cancellable, &tmp_err);
    if G_UNLIKELY(tmp_err != NULL)
    {
      g_propagate_error(error, tmp_err);
      goto_error();
    }

    table = g_object_get_qdata(G_OBJECT(self), elf_parser_cached_string_table_quark());
    g_assert(table != NULL);
  }

  if(sh_name > g_bytes_get_size(table))
  {
    g_set_error(error,
                EV_PARSER_ERROR,
                EV_PARSER_ERROR_FAILED,
                "String offset out of bounds\r\n");
    goto_error();
  }

  const gchar* strings =
  g_bytes_get_data(table, NULL);
  return_ = g_utf8_make_valid(strings + sh_name, -1);

error_tag()
return return_;
}

goffset
_elf_parser_translate_addr(EvElfParser* self,
                           vaddr_t addr,
                           GInputStream* stream,
                           GCancellable* cancellable,
                           GError** error)
{
  failable_begin();
  goffset return_ = -1;
  vaddr_t base, limit;
  goffset start;
  gint i, t;

  union {
    elf_phent32_t _32;
    elf_phent64_t _64;
  } ents;

  switch(header32->bitlen)
  {
  case elf_bitlen_32:
    t = header32->e_phnum;
    break;
  case elf_bitlen_64:
    t = header64->e_phnum;
    break;
  }

  for(i = 0;i < t;i++)
  {
    switch(header32->bitlen)
    {
    case elf_bitlen_32:
      success =
      _elf_parser_get_segment_entry(self, i, &(ents._64), stream, cancellable, &tmp_err);
      if G_UNLIKELY(tmp_err != NULL)
      {
        g_propagate_error(error, tmp_err);
        goto_error();
      }

      base = (vaddr_t)(ents._32.p_vaddr);
      limit = base + ents._32.p_memsz;
      start = ents._32.p_offset;
      break;
    case elf_bitlen_64:
      success =
      _elf_parser_get_segment_entry(self, i, &(ents._64), stream, cancellable, &tmp_err);
      if G_UNLIKELY(tmp_err != NULL)
      {
        g_propagate_error(error, tmp_err);
        goto_error();
      }

      base = (vaddr_t)(ents._64.p_vaddr);
      limit = base + ents._64.p_memsz;
      start = ents._64.p_offset;
      break;
    }

    if(base >= addr && addr < limit)
    {
      vaddr_t offset = addr - base;
      return_ = start + offset;
    }
  }

error_tag()
return return_;
}
