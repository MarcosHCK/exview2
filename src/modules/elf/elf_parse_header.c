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
_elf_parser_parse_header(EvElfParser* self,
                         EvViewContext* view_ctx,
                         GInputStream* stream,
                         GCancellable* cancellable,
                         GError** error)
{
  page_builder_new();
  failable_begin();

  page_builder_begin(view_ctx, "Header", FALSE);
  elf_header32_t* header = header32;

  page_builder_printp("e_ident[EI_CLASS]");
  page_builder_printv("%i bits (0x%02x)", 32 << (header->bitlen - 1), header->bitlen);
  page_builder_setrow(FALSE);

  page_builder_printp("e_ident[EI_DATA]");
  page_builder_printv("%s (0x%02x)", (header->endianness == 1) ? "Little endian" : "Big endian", header->endianness);
  page_builder_setrow(FALSE);

  page_builder_printp("e_ident[EI_VERSION]");
  page_builder_printv("%i", header->tiny_ver);
  page_builder_setrow(FALSE);

  page_builder_printp("e_ident[EI_OSABI]");
  page_builder_printv("%s (0x%02x)", _s_elfabi(header->abi), header->abi);
  page_builder_setrow(FALSE);

  page_builder_printp("e_ident[EI_ABIVERSION]");
  page_builder_printv("%i", header->abiver);
  page_builder_setrow(FALSE);

  page_builder_printp("e_type");
  page_builder_printv("%s (0x%02x)", _s_elftype(header->e_type), header->e_type);
  page_builder_setrow(FALSE);

  page_builder_printp("e_machine");
  page_builder_printv("%s (0x%04x)", _s_elfmach(header->e_machine), header->e_machine);
  page_builder_setrow(FALSE);

  page_builder_printp("e_version");
  page_builder_printv("0x%08x", header->e_version);
  page_builder_setrow(FALSE);

  switch(header->bitlen)
  {
  case elf_bitlen_32:
    page_builder_printp("e_entry");
    page_builder_printv("0x%08x", header32->e_entry);
    page_builder_setrow(FALSE);

    page_builder_printp("e_phoff");
    page_builder_printv("0x%08x", header32->e_phoff);
    page_builder_setrow(FALSE);

    page_builder_printp("e_shoff");
    page_builder_printv("0x%08x", header32->e_shoff);
    page_builder_setrow(FALSE);
    break;
  case elf_bitlen_64:
    page_builder_printp("e_entry");
    page_builder_printv("0x%016" PRIx64, header64->e_entry);
    page_builder_setrow(FALSE);

    page_builder_printp("e_phoff");
    page_builder_printv("0x%016" PRIx64, header64->e_phoff);
    page_builder_setrow(FALSE);

    page_builder_printp("e_shoff");
    page_builder_printv("0x%016" PRIx64, header64->e_shoff);
    page_builder_setrow(FALSE);

    header = (elf_header32_t*)( ((gpointer)header) + 12 );
    break;
  }

  page_builder_printp("e_flags");
  page_builder_printv("0x%08x", header->e_flags);
  page_builder_setrow(FALSE);

  page_builder_printp("e_ehsize");
  page_builder_printv("0x%04x", header->e_ehsize);
  page_builder_setrow(FALSE);

  page_builder_printp("e_phentsize");
  page_builder_printv("0x%04x", header->e_phentsize);
  page_builder_setrow(FALSE);

  page_builder_printp("e_phnum");
  page_builder_printv("0x%04x", header->e_phnum);
  page_builder_setrow(FALSE);

  page_builder_printp("e_shentsize");
  page_builder_printv("0x%04x", header->e_shentsize);
  page_builder_setrow(FALSE);

  page_builder_printp("e_shnum");
  page_builder_printv("0x%04x", header->e_shnum);
  page_builder_setrow(FALSE);

  page_builder_printp("e_shstrndx");
  page_builder_printv("0x%04x", header->e_shstrndx);
  page_builder_setrow(FALSE);

error_tag()
  page_builder_end();
return success;
}
