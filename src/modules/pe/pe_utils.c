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

G_DEFINE_QUARK(ev-pe-parser-sections-quark,
               sections);

static pe_section_header_t*
get_sections(EvPeParser* self,
             guint* n_sections_p,
             GInputStream* stream,
             GCancellable* cancellable,
             GError** error)
{
  failable_begin();

  pe_section_header_t* sections =
  g_object_get_qdata(G_OBJECT(self),
                     sections_quark());
  guint n_sections =
  self->pe.header.NumberOfSections;

  if G_UNLIKELY(sections == NULL)
  {
    goffset current =
    g_seekable_tell(G_SEEKABLE(stream));
    seek_a(self->sections, G_SEEK_SET);

    sections = g_new(pe_section_header_t,
                     n_sections);

    get_s(sections,
          n_sections
          * sizeof(pe_section_header_t));
    seek_a(current, G_SEEK_SET);

    g_object_set_qdata_full
    (G_OBJECT(self),
     sections_quark(),
     sections,
     (GDestroyNotify)
     g_free);
  }

  if(n_sections_p)
    *n_sections_p =
    n_sections;

error_tag()
return sections;
}

pe_section_header_t*
_pe_parser_get_section_by_vaddr(EvPeParser* self,
                                vaddr_t vaddr,
                                GInputStream* stream,
                                GCancellable* cancellable,
                                GError** error)
{
  failable_begin();

  guint i, n_sections = 0;
  pe_section_header_t* header = NULL;
  pe_section_header_t* sections =
  get_sections(self, &n_sections, stream, cancellable, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    goto_error();
  }

  for(i = 0;i < n_sections;i++)
  {
    vaddr_t start = sections[i].VirtualAddress;
    vaddr_t end = sections[i].SizeOfRawData + start;

    if(start >= vaddr
       && end > vaddr)
    {
      header = &(sections[i]);
      goto_success();
    }
  }

error_tag()
return header;
}

goffset
_pe_parser_translate_vaddr(EvPeParser* self,
                           vaddr_t vaddr,
                           GInputStream* stream,
                           GCancellable* cancellable,
                           GError** error)
{
  failable_begin();
  goffset offset = -1;

  pe_section_header_t* header =
  _pe_parser_get_section_by_vaddr(self, vaddr, stream, cancellable, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    goto_error();
  }
  else
  {
    goto_if_failed(header != NULL);
  }

  offset = (vaddr - header->VirtualAddress) + header->PointerToRawData;
error_tag()
return offset;
}

gchar*
_pe_parser_load_string(EvPeParser* self,
                       goffset at_,
                       gssize size_,
                       gsize* psize,
                       GInputStream* stream,
                       GCancellable* cancellable,
                       GError** error)
{
  failable_begin();
  GString* string = g_string_new(NULL);
  gboolean zero = FALSE;
  gsize read = 0;

  const guint load_string_buflen = 16;
  gchar buf[load_string_buflen];
  guint i;

  g_seekable_seek
  (G_SEEKABLE(stream),
   at_,
   G_SEEK_SET,
   cancellable,
   &tmp_err);

  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    goto_error();
  }

  do
  {
    success =
    g_input_stream_read_all(stream, buf, sizeof(buf), &read, cancellable, &tmp_err);
    if G_UNLIKELY(tmp_err != NULL)
    {
      g_propagate_error(error, tmp_err);
      goto_error();
    }

    for(i = 0;i < sizeof(buf);i++)
    {
      if(buf[i] == '0')
      {
        zero = TRUE;
        break;
      }
      else
      {
        g_string_append_c(string, buf[i]);
        if(size_ > 0 && string->len >= size_)
        {
          zero = TRUE;
          break;
        }
      }
    }
  }
  while(read == load_string_buflen && zero == FALSE);

  if(read != sizeof(buf)
     && zero == FALSE)
  {
    g_set_error
    (error,
     EV_PARSER_ERROR,
     EV_PARSER_ERROR_FAILED,
     "reached EOF while loading an string\r\n");
    goto_error();
  }
  else
  {
    success =
    g_utf8_validate_len(string->str, string->len, NULL);
    if G_UNLIKELY(success == FALSE)
    {
      g_set_error
      (error,
       EV_PARSER_ERROR,
       EV_PARSER_ERROR_FAILED,
       "invalid string\r\n");
      goto_error();
    }
  }

error_tag()
return g_string_free(string, !success);
}
