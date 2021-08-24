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
#include <pe_parser.h>

gboolean
_pe_parser_parse_sections(EvPeParser* self,
                          EvViewContext* view_ctx,
                          GInputStream* stream,
                          GCancellable* cancellable,
                          GError** error)
{
  page_builder_new();
  failable_begin();

  pe_file_header_t* pe = &(self->pe.header);

  if G_UNLIKELY(pe->NumberOfSections == 0)
    return TRUE;

  pe_section_header_t header;
  gint i, j;

  gchar* title = g_strdup_printf("Sections (%i)", pe->NumberOfSections);
  page_builder_begin(view_ctx, title, TRUE);
  g_free(title);

  seek_a(self->sections, G_SEEK_SET);
  for(i = 0;i < pe->NumberOfSections;i++)
  {
    get_s(&header, sizeof(header));

    g_string_truncate(page_p, 0);
    for(j = 0;j < G_N_ELEMENTS(header.Name);j++)
    if(header.Name == 0)
      break;
    else
      g_string_append_c(page_p, header.Name[j]);
    page_builder_begin(view_ctx, page_p->str, FALSE);

    page_builder_printp("PhysicalAddress");
    page_builder_printv("0x%08" PRIx32, header.Misc.PhysicalAddress);
    page_builder_setrow(FALSE);

    page_builder_printp("VirtualSize");
    page_builder_printv("0x%08" PRIx32, header.Misc.VirtualSize);
    page_builder_setrow(FALSE);

    page_builder_printp("VirtualAddress");
    page_builder_printv("0x%08" PRIx32, header.VirtualAddress);
    page_builder_setrow(FALSE);

    page_builder_printp("SizeOfRawData");
    page_builder_printv("0x%08" PRIx32, header.SizeOfRawData);
    page_builder_setrow(FALSE);

    page_builder_printp("PointerToRawData");
    page_builder_printv("0x%08" PRIx32, header.PointerToRawData);
    page_builder_setrow(FALSE);

    page_builder_printp("PointerToRelocations");
    page_builder_printv("0x%08" PRIx32, header.PointerToRelocations);
    page_builder_setrow(FALSE);

    page_builder_printp("PointerToLinenumbers");
    page_builder_printv("0x%08" PRIx32, header.PointerToLinenumbers);
    page_builder_setrow(FALSE);

    page_builder_printp("NumberOfRelocations");
    page_builder_printv("0x%08" PRIx32, header.NumberOfRelocations);
    page_builder_setrow(FALSE);

    page_builder_printp("NumberOfLinenumbers");
    page_builder_printv("0x%08" PRIx32, header.NumberOfLinenumbers);
    page_builder_setrow(FALSE);

    page_builder_printp("Characteristics");
    page_builder_printv("0x%08" PRIx32, header.Characteristics);
    page_builder_setrow(TRUE);

      const gchar* chars = _s_pesectchars(header.Characteristics);
      g_string_insert(page_p, 0, chars);
      page_builder_setrow(FALSE);

    page_builder_closerow();
  }

error_tag()
  ev_view_context_ascend(view_ctx);
  page_builder_end();
return success;
}
