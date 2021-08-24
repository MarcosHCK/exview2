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

gboolean
_pe_parser_parse_pe_header(EvPeParser* self,
                           EvViewContext* view_ctx,
                           GInputStream* stream,
                           GCancellable* cancellable,
                           GError** error)
{
  page_builder_new();
  failable_begin();

  pe_file_header_t* pe = &(self->pe.header);

  page_builder_begin(view_ctx, "PE header", FALSE);

  page_builder_printp("Arch");
  page_builder_printv("%s (0x%04" PRIx16 ")", _s_pearch(pe->Arch), pe->Arch);
  page_builder_setrow(FALSE);

  page_builder_printp("NumberOfSections");
  page_builder_printv("0x%04" PRIx16, pe->NumberOfSections);
  page_builder_setrow(FALSE);

  page_builder_printp("TimeDateStamp");
  page_builder_printv("%s (0x%08" PRIx32 ")", _s_petimestamp(pe->TimeDateStamp), pe->TimeDateStamp);
  page_builder_setrow(FALSE);

  page_builder_printp("PointerToSymbolTable");
  page_builder_printv("0x%08" PRIx32, pe->PointerToSymbolTable);
  page_builder_setrow(FALSE);

  page_builder_printp("NumberOfSymbols");
  page_builder_printv("0x%08" PRIx32, pe->NumberOfSymbols);
  page_builder_setrow(FALSE);

  page_builder_printp("SizeOfOptionalHeader");
  page_builder_printv("0x%04" PRIx16, pe->SizeOfOptionalHeader);
  page_builder_setrow(FALSE);

  page_builder_printp("Characteristics");
  page_builder_printv("0x%04" PRIx16, pe->Characteristics);
  page_builder_setrow(TRUE);

  g_string_truncate(page_p, 0);
  gchar* chars = _s_pechars(pe->Characteristics);
  g_string_insert(page_p, 0, chars);
  g_free(chars);
  page_builder_setrow(FALSE);

  page_builder_closerow();

error_tag()
  page_builder_end();
return success;
}

gboolean
_pe_parser_parse_opt_header(EvPeParser* self,
                            EvViewContext* view_ctx,
                            GInputStream* stream,
                            GCancellable* cancellable,
                            GError** error)
{
  page_builder_new();
  failable_begin();

  if G_UNLIKELY(self->pe.header.SizeOfOptionalHeader == 0)
    return TRUE;

  page_builder_begin(view_ctx, "Optional header", FALSE);
  pe_data_directory_t* directories = NULL;
  gint n_directories;
  gchar* chars;

  switch(self->bitlen)
  {
  case pe_bitlen_32:
#define optional (&(self->opt._32))
    page_builder_printp("Magic");
    page_builder_printv("%s (0x%04" PRIx16 ")", _s_pever(optional->Magic), optional->Magic);
    page_builder_setrow(FALSE);

    page_builder_printp("MajorLinkerVersion");
    page_builder_printv("0x%02" PRIX8, optional->MajorLinkerVersion);
    page_builder_setrow(FALSE);

    page_builder_printp("MinorLinkerVersion");
    page_builder_printv("0x%02" PRIX8, optional->MinorLinkerVersion);
    page_builder_setrow(FALSE);

    page_builder_printp("SizeOfCode");
    page_builder_printv("0x%08" PRIx32, optional->SizeOfCode);
    page_builder_setrow(FALSE);

    page_builder_printp("SizeOfInitializedData");
    page_builder_printv("0x%08" PRIx32, optional->SizeOfInitializedData);
    page_builder_setrow(FALSE);

    page_builder_printp("SizeOfUninitializedData");
    page_builder_printv("0x%08" PRIx32, optional->SizeOfUninitializedData);
    page_builder_setrow(FALSE);

    page_builder_printp("AddressOfEntryPoint");
    page_builder_printv("0x%08" PRIx32, optional->AddressOfEntryPoint);
    page_builder_setrow(FALSE);

    page_builder_printp("BaseOfCode");
    page_builder_printv("0x%08" PRIx32, optional->BaseOfCode);
    page_builder_setrow(FALSE);

    page_builder_printp("BaseOfData");
    page_builder_printv("0x%08" PRIx32, optional->BaseOfData);
    page_builder_setrow(FALSE);

    page_builder_printp("ImageBase");
    page_builder_printv("0x%08" PRIx32, optional->ImageBase);
    page_builder_setrow(FALSE);

    page_builder_printp("SectionAlignment");
    page_builder_printv("0x%08" PRIx32, optional->SectionAlignment);
    page_builder_setrow(FALSE);

    page_builder_printp("FileAlignment");
    page_builder_printv("0x%08" PRIx32, optional->FileAlignment);
    page_builder_setrow(FALSE);

    page_builder_printp("MajorOperatingSystemVersion");
    page_builder_printv("0x%04" PRIx16, optional->MajorOperatingSystemVersion);
    page_builder_setrow(FALSE);

    page_builder_printp("MinorOperatingSystemVersion");
    page_builder_printv("0x%04" PRIx16, optional->MinorOperatingSystemVersion);
    page_builder_setrow(FALSE);

    page_builder_printp("MajorImageVersion");
    page_builder_printv("0x%04" PRIx16, optional->MajorImageVersion);
    page_builder_setrow(FALSE);

    page_builder_printp("MinorImageVersion");
    page_builder_printv("0x%04" PRIx16, optional->MinorImageVersion);
    page_builder_setrow(FALSE);

    page_builder_printp("MajorSubsystemVersion");
    page_builder_printv("0x%04" PRIx16, optional->MajorSubsystemVersion);
    page_builder_setrow(FALSE);

    page_builder_printp("MinorSubsystemVersion");
    page_builder_printv("0x%04" PRIx16, optional->MinorSubsystemVersion);
    page_builder_setrow(FALSE);

    page_builder_printp("SizeOfImage");
    page_builder_printv("0x%08" PRIx32, optional->SizeOfImage);
    page_builder_setrow(FALSE);

    page_builder_printp("SizeOfHeaders");
    page_builder_printv("0x%08" PRIx32, optional->SizeOfHeaders);
    page_builder_setrow(FALSE);

    page_builder_printp("CheckSum");
    page_builder_printv("0x%08" PRIx32, optional->CheckSum);
    page_builder_setrow(FALSE);

    page_builder_printp("Subsystem");
    page_builder_printv("%s (0x%04" PRIx16 ")", _s_pesubsystem(optional->Subsystem), optional->Subsystem);
    page_builder_setrow(FALSE);

    page_builder_printp("DllCharacteristics");
    page_builder_printv("0x%04" PRIx16, optional->DllCharacteristics);
    page_builder_setrow(TRUE);

      g_string_truncate(page_p, 0);
      chars = _s_pedllchars(optional->DllCharacteristics);
      g_string_insert(page_p, 0, chars);
      g_free(chars);
      page_builder_setrow(FALSE);

    page_builder_closerow();

    page_builder_printp("SizeOfStackReserve");
    page_builder_printv("0x%08" PRIx32, optional->SizeOfStackReserve);
    page_builder_setrow(FALSE);

    page_builder_printp("SizeOfStackCommit");
    page_builder_printv("0x%08" PRIx32, optional->SizeOfStackCommit);
    page_builder_setrow(FALSE);

    page_builder_printp("SizeOfHeapReserve");
    page_builder_printv("0x%08" PRIx32, optional->SizeOfHeapReserve);
    page_builder_setrow(FALSE);

    page_builder_printp("SizeOfHeapCommit");
    page_builder_printv("0x%08" PRIx32, optional->SizeOfHeapCommit);
    page_builder_setrow(FALSE);

    page_builder_printp("LoaderFlags");
    page_builder_printv("0x%08" PRIx32, optional->LoaderFlags);
    page_builder_setrow(FALSE);

    page_builder_printp("NumberOfRvaAndSizes");
    page_builder_printv("0x%08" PRIx32, optional->NumberOfRvaAndSizes);
    page_builder_setrow(FALSE);

    directories = &(optional->DataDirectory[0]);
    n_directories = optional->NumberOfRvaAndSizes;
#undef optional
    break;
  case pe_bitlen_64:
#define optional (&(self->opt._64))
    page_builder_printp("Magic");
    page_builder_printv("%s (0x%04" PRIx16 ")", _s_pever(optional->Magic), optional->Magic);
    page_builder_setrow(FALSE);

    page_builder_printp("MajorLinkerVersion");
    page_builder_printv("0x%02" PRIX8, optional->MajorLinkerVersion);
    page_builder_setrow(FALSE);

    page_builder_printp("MinorLinkerVersion");
    page_builder_printv("0x%02" PRIX8, optional->MinorLinkerVersion);
    page_builder_setrow(FALSE);

    page_builder_printp("SizeOfCode");
    page_builder_printv("0x%08" PRIx32, optional->SizeOfCode);
    page_builder_setrow(FALSE);

    page_builder_printp("SizeOfInitializedData");
    page_builder_printv("0x%08" PRIx32, optional->SizeOfInitializedData);
    page_builder_setrow(FALSE);

    page_builder_printp("SizeOfUninitializedData");
    page_builder_printv("0x%08" PRIx32, optional->SizeOfUninitializedData);
    page_builder_setrow(FALSE);

    page_builder_printp("AddressOfEntryPoint");
    page_builder_printv("0x%08" PRIx32, optional->AddressOfEntryPoint);
    page_builder_setrow(FALSE);

    page_builder_printp("BaseOfCode");
    page_builder_printv("0x%08" PRIx32, optional->BaseOfCode);
    page_builder_setrow(FALSE);

    page_builder_printp("ImageBase");
    page_builder_printv("0x%016" PRIx64, optional->ImageBase);
    page_builder_setrow(FALSE);

    page_builder_printp("SectionAlignment");
    page_builder_printv("0x%08" PRIx32, optional->SectionAlignment);
    page_builder_setrow(FALSE);

    page_builder_printp("FileAlignment");
    page_builder_printv("0x%08" PRIx32, optional->FileAlignment);
    page_builder_setrow(FALSE);

    page_builder_printp("MajorOperatingSystemVersion");
    page_builder_printv("0x%04" PRIx16, optional->MajorOperatingSystemVersion);
    page_builder_setrow(FALSE);

    page_builder_printp("MinorOperatingSystemVersion");
    page_builder_printv("0x%04" PRIx16, optional->MinorOperatingSystemVersion);
    page_builder_setrow(FALSE);

    page_builder_printp("MajorImageVersion");
    page_builder_printv("0x%04" PRIx16, optional->MajorImageVersion);
    page_builder_setrow(FALSE);

    page_builder_printp("MinorImageVersion");
    page_builder_printv("0x%04" PRIx16, optional->MinorImageVersion);
    page_builder_setrow(FALSE);

    page_builder_printp("MajorSubsystemVersion");
    page_builder_printv("0x%04" PRIx16, optional->MajorSubsystemVersion);
    page_builder_setrow(FALSE);

    page_builder_printp("MinorSubsystemVersion");
    page_builder_printv("0x%04" PRIx16, optional->MinorSubsystemVersion);
    page_builder_setrow(FALSE);

    page_builder_printp("SizeOfImage");
    page_builder_printv("0x%08" PRIx32, optional->SizeOfImage);
    page_builder_setrow(FALSE);

    page_builder_printp("SizeOfHeaders");
    page_builder_printv("0x%08" PRIx32, optional->SizeOfHeaders);
    page_builder_setrow(FALSE);

    page_builder_printp("CheckSum");
    page_builder_printv("0x%08" PRIx32, optional->CheckSum);
    page_builder_setrow(FALSE);

    page_builder_printp("Subsystem");
    page_builder_printv("%s (0x%04" PRIx16 ")", _s_pesubsystem(optional->Subsystem), optional->Subsystem);
    page_builder_setrow(FALSE);

    page_builder_printp("DllCharacteristics");
    page_builder_printv("0x%04" PRIx16, optional->DllCharacteristics);
    page_builder_setrow(TRUE);

      g_string_truncate(page_p, 0);
      chars = _s_pedllchars(optional->DllCharacteristics);
      g_string_insert(page_p, 0, chars);
      g_free(chars);
      page_builder_setrow(FALSE);

    page_builder_closerow();

    page_builder_printp("SizeOfStackReserve");
    page_builder_printv("0x%016" PRIx64, optional->SizeOfStackReserve);
    page_builder_setrow(FALSE);

    page_builder_printp("SizeOfStackCommit");
    page_builder_printv("0x%016" PRIx64, optional->SizeOfStackCommit);
    page_builder_setrow(FALSE);

    page_builder_printp("SizeOfHeapReserve");
    page_builder_printv("0x%016" PRIx64, optional->SizeOfHeapReserve);
    page_builder_setrow(FALSE);

    page_builder_printp("SizeOfHeapCommit");
    page_builder_printv("0x%016" PRIx64, optional->SizeOfHeapCommit);
    page_builder_setrow(FALSE);

    page_builder_printp("LoaderFlags");
    page_builder_printv("0x%08" PRIx32, optional->LoaderFlags);
    page_builder_setrow(FALSE);

    page_builder_printp("NumberOfRvaAndSizes");
    page_builder_printv("0x%08" PRIx32, optional->NumberOfRvaAndSizes);
    page_builder_setrow(FALSE);

    directories = &(optional->DataDirectory[0]);
    n_directories = optional->NumberOfRvaAndSizes;
#undef optional
    break;
  default:
    g_assert_not_reached();
    break;
  }

  goto_if_failed(pe_numberof_data_directories >= n_directories);
  g_assert(directories != NULL);

  const gchar* properties[] = {
    "Virtual Address",
    "Size",
    "Section"};
  gint i, j;

  for(i = 0;i < n_directories;i++)
  {
    page_builder_printp("DataDirectory[%i]", i);
    page_builder_printv("%s", _s_peddirname(i));
    page_builder_setrow(TRUE);

    guint32* uints = (guint32*)&(directories[i]);
    for(j = 0;j < G_N_ELEMENTS(properties);j++)
    {
      page_builder_printp("%s", properties[j]);
      page_builder_printv("0x%08" PRIx32, uints[j]);
      page_builder_setrow(FALSE);
    }

    page_builder_closerow();
  }

error_tag()
  page_builder_end();
return success;
}
