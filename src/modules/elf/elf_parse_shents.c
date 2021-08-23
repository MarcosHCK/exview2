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
#include <inttypes.h>
#include <elf_parser.h>

gboolean
_elf_parser_parse_note_section(EvElfParser* self,
                               EvViewContext* page_r_,
                               goffset base,
                               gsize size,
                               GInputStream* stream,
                               GCancellable* cancellable,
                               GError** error);

gboolean
_elf_parser_parse_shents(EvElfParser* self,
                         EvViewContext* view_ctx,
                         GInputStream* stream,
                         GCancellable* cancellable,
                         GError** error)
{
  page_builder_new();
  failable_begin();

  gsize entries; take_var_p(header, e_shnum, &entries, gsize);
  if G_UNLIKELY(entries == 0)
    return TRUE;

  goffset base; take_var_p(header, e_shoff, &base, goffset);
  gsize entsz; take_var_p(header, e_shentsize, &entsz, gsize);
  gsize i;

  gchar* title = g_strdup_printf("Section headers (%i)", entries);
  page_builder_begin(view_ctx, title, TRUE);
  g_free(title);

  union {
    elf_shent32_t _32;
    elf_shent64_t _64;
  } ents;

  for(i = 1;i < entries;i++)
  {
    get_a(&ents, entsz, base + (i * entsz), G_SEEK_SET);

    gchar* name = _elf_parser_get_string(self, ents._32.sh_name, stream, cancellable, &tmp_err);
    if G_UNLIKELY(tmp_err != NULL)
    {
      g_propagate_error(error, tmp_err);
      goto_error();
    }

    GString* title = g_string_new(NULL);
    g_string_append_printf(title, "(%i) ", i);
    g_string_append(title, name);

    page_builder_begin(view_ctx, title->str, FALSE);
    g_string_free(title, TRUE);

    switch(header32->bitlen)
    {
    case elf_bitlen_32:
#define entry (&(ents._32))
      page_builder_printp("sh_name");
      page_builder_printv("%s (0x%x)", name, entry->sh_name);
      page_builder_setrow(FALSE);

      page_builder_printp("sh_type");
      page_builder_printv("%s (0x%x)", _s_elfshtype(entry->sh_type), entry->sh_type);
      page_builder_setrow(FALSE);

      page_builder_printp("sh_flags");
      page_builder_printv("0x%08" PRIx32, entry->sh_flags);
      page_builder_setrow(FALSE);

      page_builder_printp("sh_addr");
      page_builder_printv("0x%08" PRIx32, entry->sh_addr);
      page_builder_setrow(FALSE);

      page_builder_printp("sh_offset");
      page_builder_printv("0x%08" PRIx32, entry->sh_offset);
      page_builder_setrow(FALSE);

      page_builder_printp("sh_size");
      page_builder_printv("0x%08" PRIx32, entry->sh_size);
      page_builder_setrow(FALSE);

      page_builder_printp("sh_link");
      page_builder_printv("0x%08" PRIx32, entry->sh_link);
      page_builder_setrow(FALSE);

      page_builder_printp("sh_info");
      page_builder_printv("0x%08" PRIx32, entry->sh_info);
      page_builder_setrow(FALSE);

      page_builder_printp("sh_addralign");
      page_builder_printv("0x%08" PRIx32, entry->sh_addralign);
      page_builder_setrow(FALSE);

      page_builder_printp("sh_entsize");
      page_builder_printv("0x%08" PRIx32, entry->sh_entsize);
      page_builder_setrow(FALSE);
#undef entry
      break;
    case elf_bitlen_64:
#define entry (&(ents._64))
      page_builder_printp("sh_name");
      page_builder_printv("%s (0x%x)", name, entry->sh_name);
      page_builder_setrow(FALSE);

      page_builder_printp("sh_type");
      page_builder_printv("%s (0x%x)", _s_elfshtype(entry->sh_type), entry->sh_type);
      page_builder_setrow(FALSE);

      page_builder_printp("sh_flags");
      page_builder_printv("0x%016" PRIx64, entry->sh_flags);
      page_builder_setrow(FALSE);

      page_builder_printp("sh_addr");
      page_builder_printv("0x%016" PRIx64, entry->sh_addr);
      page_builder_setrow(FALSE);

      page_builder_printp("sh_offset");
      page_builder_printv("0x%016" PRIx64, entry->sh_offset);
      page_builder_setrow(FALSE);

      page_builder_printp("sh_size");
      page_builder_printv("0x%016" PRIx64, entry->sh_size);
      page_builder_setrow(FALSE);

      page_builder_printp("sh_link");
      page_builder_printv("0x%08" PRIx32, entry->sh_link);
      page_builder_setrow(FALSE);

      page_builder_printp("sh_info");
      page_builder_printv("0x%08" PRIx32, entry->sh_info);
      page_builder_setrow(FALSE);

      page_builder_printp("sh_addralign");
      page_builder_printv("0x%016" PRIx64, entry->sh_addralign);
      page_builder_setrow(FALSE);

      page_builder_printp("sh_entsize");
      page_builder_printv("0x%016" PRIx64, entry->sh_entsize);
      page_builder_setrow(FALSE);
#undef entry
      break;
    }

    goffset base;
    gsize sz;

    switch(header32->bitlen)
    {
    case elf_bitlen_32:
      base = ((elf_shent32_t*)&ents)->sh_offset;
      sz = ((elf_shent32_t*)&ents)->sh_size;
      break;
    case elf_bitlen_64:
      base = ((elf_shent64_t*)&ents)->sh_offset;
      sz = ((elf_shent64_t*)&ents)->sh_size;
      break;
    }

    switch(ents._32.sh_type)
    {
    case elf_shtype_note:
      success =
      _elf_parser_parse_note_section(self, page_r, base, sz, stream, cancellable, &tmp_err);
      break;
    }

    if G_UNLIKELY(tmp_err != NULL)
    {
      g_propagate_error(error, tmp_err);
      goto_error();
    }
  }

error_tag()
  ev_view_context_ascend(view_ctx);
  page_builder_end();
return success;
}
