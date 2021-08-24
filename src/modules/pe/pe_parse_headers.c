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
_pe_parser_parse_dos_header(EvPeParser* self,
                            EvViewContext* view_ctx,
                            GInputStream* stream,
                            GCancellable* cancellable,
                            GError** error)
{
  page_builder_new();
  failable_begin();

  pe_dos_header_t* dos = &(self->dos);
  guint i;

  page_builder_begin(view_ctx, "DOS header", FALSE);

  page_builder_printp("e_magic");
  page_builder_printv("0x%04" PRIx16, dos->e_magic);
  page_builder_setrow(FALSE);

  page_builder_printp("e_cblp");
  page_builder_printv("0x%04" PRIx16, dos->e_cblp);
  page_builder_setrow(FALSE);

  page_builder_printp("e_cp");
  page_builder_printv("0x%04" PRIx16, dos->e_cp);
  page_builder_setrow(FALSE);

  page_builder_printp("e_crlc");
  page_builder_printv("0x%04" PRIx16, dos->e_crlc);
  page_builder_setrow(FALSE);

  page_builder_printp("e_cparhdr");
  page_builder_printv("0x%04" PRIx16, dos->e_cparhdr);
  page_builder_setrow(FALSE);

  page_builder_printp("e_minalloc");
  page_builder_printv("0x%04" PRIx16, dos->e_minalloc);
  page_builder_setrow(FALSE);

  page_builder_printp("e_maxalloc");
  page_builder_printv("0x%04" PRIx16, dos->e_maxalloc);
  page_builder_setrow(FALSE);

  page_builder_printp("e_ss");
  page_builder_printv("0x%04" PRIx16, dos->e_ss);
  page_builder_setrow(FALSE);

  page_builder_printp("e_sp");
  page_builder_printv("0x%04" PRIx16, dos->e_sp);
  page_builder_setrow(FALSE);

  page_builder_printp("e_csum");
  page_builder_printv("0x%04" PRIx16, dos->e_csum);
  page_builder_setrow(FALSE);

  page_builder_printp("e_ip");
  page_builder_printv("0x%04" PRIx16, dos->e_ip);
  page_builder_setrow(FALSE);

  page_builder_printp("e_cs");
  page_builder_printv("0x%04" PRIx16, dos->e_cs);
  page_builder_setrow(FALSE);

  page_builder_printp("e_lfarlc");
  page_builder_printv("0x%04" PRIx16, dos->e_lfarlc);
  page_builder_setrow(FALSE);

  page_builder_printp("e_ovno");
  page_builder_printv("0x%04" PRIx16, dos->e_ovno);
  page_builder_setrow(FALSE);

  page_builder_printp("e_res");
  page_builder_setrow(TRUE);

  for(i = 0;i < G_N_ELEMENTS(dos->e_res);i++)
  {
    page_builder_printp("e_res[%i]", i);
    page_builder_printv("0x%04" PRIx16, dos->e_res[i]);
    page_builder_setrow(FALSE);
  }

  page_builder_closerow();

  page_builder_printp("e_oemid");
  page_builder_printv("0x%04" PRIx16, dos->e_oemid);
  page_builder_setrow(FALSE);

  page_builder_printp("e_oeminfo");
  page_builder_printv("0x%04" PRIx16, dos->e_oeminfo);
  page_builder_setrow(FALSE);

  page_builder_printp("e_res2");
  page_builder_setrow(TRUE);

  for(i = 0;i < G_N_ELEMENTS(dos->e_res2);i++)
  {
    page_builder_printp("e_res2[%i]", i);
    page_builder_printv("0x%04" PRIx16, dos->e_res2[i]);
    page_builder_setrow(FALSE);
  }

  page_builder_closerow();

  page_builder_printp("e_lfanew");
  page_builder_printv("0x%04" PRIx16, dos->e_lfanew);
  page_builder_setrow(FALSE);

error_tag()
  page_builder_end();
return success;
}
