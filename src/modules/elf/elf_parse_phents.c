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

static gboolean
phtype_interp(EvElfParser* self,
             EvViewContext* page_r_,
             goffset base,
             gsize size,
             GInputStream* stream,
             GCancellable* cancellable,
             GError** error)
{
  page_builder_new();
  failable_begin();
  page_r = g_object_ref(page_r_);

  page_builder_printp("Interpreter");
  page_builder_setrow(TRUE);

  gchar* buff = g_malloc(size + 1);
  get_a(buff, size, base, G_SEEK_SET);
  buff[size] = '\0';

  const gchar* end = NULL;
  if G_LIKELY(g_utf8_validate(buff, -1, &end))
  {
    page_builder_printp("%.*s", (gint)(end - buff), buff);
    page_builder_setrow(FALSE);
  }

  page_builder_closerow();
error_tag()
  page_builder_end();
return success;
}

static gboolean
phnote_gnu(EvElfParser* self,
           EvViewContext* page_r_,
           gpointer buffer,
           gsize size,
           guint32 type,
           GInputStream* stream,
           GCancellable* cancellable,
           GError** error)
{
  page_builder_new();
  failable_begin();
  page_r = g_object_ref(page_r_);

  switch(type)
  {
  case 0x01:
    {
      if G_UNLIKELY(size != 16)
      {
        page_builder_printp("GNU tag type 0x%08" PRIx32 " malformed", type);
        page_builder_setrow(FALSE);
        goto _error_;
      }

      guint32* vers =
      (guint32*)buffer;
      const gchar* os =
      "unknown";

      switch(vers[0])
      {
      case 0:
        os = "Linux";
        break;
      case 1:
        os = "Hurd";
        break;
      case 2:
        os = "Solaris";
        break;
      }

      page_builder_printp("%s %i.%i.%i", os, vers[1], vers[2], vers[3]);
      page_builder_setrow(FALSE);
    }
    break;
  case 0x03:
    {
      guchar* bytes = buffer;
      g_string_set_size(page_p, (size << 1) + 1);
      g_string_truncate(page_p, 0);

      for(;size > 0;size--)
      {
        g_string_append_printf(page_p, "%02x", *bytes++);
      }

      page_builder_setrow(FALSE);
    }
    break;
  default:
    page_builder_printp("GNU tag type 0x%08" PRIx32 " unsupported", type);
    page_builder_setrow(FALSE);
    break;
  }

error_tag()
  page_builder_end();
return success;
}

static gboolean
phnote_pax(EvElfParser* self,
           EvViewContext* page_r_,
           gpointer buffer,
           gsize size,
           guint32 type,
           GInputStream* stream,
           GCancellable* cancellable,
           GError** error)
{
  page_builder_new();
  failable_begin();
  page_r = g_object_ref(page_r_);

  switch(type)
  {
  case 0x03:
    {
      if G_UNLIKELY(size != 4)
      {
        page_builder_printp("PaX tag type 0x%08" PRIx32 " malformed", type);
        page_builder_setrow(FALSE);
        goto _error_;
      }

      guint32 flags =
      ((guint32*)buffer)[0];
      g_string_truncate(page_p, 0);
#define flag(n,v) \
  if(flags & (n) == (n)) \
  { \
    g_string_append(page_p, v "\n"); \
  }

      flag(0x01, "enable Mprotect");
      flag(0x02, "disable Mprotect");
      flag(0x04, "enable Segvguard");
      flag(0x08, "disable Segvguard");
      flag(0x10, "enable ASLR");
      flag(0x20, "disable ASLR");
#undef flag
      page_builder_setrow(FALSE);
    }
    break;
  default:
    page_builder_printp("PaX tag type 0x%08" PRIx32 " unsupported", type);
    page_builder_setrow(FALSE);
    break;
  }
error_tag()
  page_builder_end();
return success;
}

static gboolean
phtype_note(EvElfParser* self,
            EvViewContext* page_r_,
            goffset base,
            gsize size,
            GInputStream* stream,
            GCancellable* cancellable,
            GError** error)
{
  page_builder_new();
  failable_begin();
  page_r = g_object_ref(page_r_);

  page_builder_printp("Notes");
  page_builder_setrow(TRUE);
  seek_a(base, G_SEEK_SET);

  gchar* buf = NULL;
  guint32 fields[3];
#define namesz  fields[0]
#define descsz  fields[1]
#define type    fields[2]

  do
  {
    get_s(fields, sizeof(fields));
    if G_LIKELY(namesz > 0)
    {
      gsize sz_1 = (fields[0] & 3) ? fields[0] + (4 - (fields[0] & 3)) : fields[0];
      gsize sz_2 = (fields[1] & 3) ? fields[1] + (4 - (fields[1] & 3)) : fields[1];
      gsize sz = sz_1 + sz_2;

      buf = g_realloc(buf, sz);
      get_s(buf, sz);

      const gchar* end = NULL;
      if G_UNLIKELY(!g_utf8_validate(buf, namesz - 1, &end))
        continue;
      else
      {
        page_builder_printp("%.*s", (gint)(end - buf), buf);
        page_builder_setrow(TRUE);
      }

      gpointer buffer = (buf + sz_1);
      if(!strncmp(buf, "GNU", (end - buf)))
      {
        success =
        phnote_gnu(self, page_r, buffer, descsz, type, stream, cancellable, &tmp_err);
        if G_UNLIKELY(tmp_err != NULL)
        {
          g_propagate_error(error, tmp_err);
          goto_error();
        }
      } else
      if(!strncmp(buf, "PaX", (end - buf)))
      {
        success =
        phnote_pax(self, page_r, buffer, descsz, type, stream, cancellable, &tmp_err);
        if G_UNLIKELY(tmp_err != NULL)
        {
          g_propagate_error(error, tmp_err);
          goto_error();
        }
      } else
      {
        page_builder_printp("unknown contents");
        page_builder_setrow(FALSE);
      }

      page_builder_closerow();
    }
  }
  while(g_seekable_tell(G_SEEKABLE(stream)) - base < size);

#undef namesz
#undef descsz
#undef type
error_tag()
  g_free(buf);
  page_builder_end();
return success;
}

gboolean
_elf_parser_parse_note_section(EvElfParser* self,
                               EvViewContext* page_r_,
                               goffset base,
                               gsize size,
                               GInputStream* stream,
                               GCancellable* cancellable,
                               GError** error)
{
  return phtype_note(self, page_r_, base, size, stream, cancellable, error);
}

gboolean
_elf_parser_parse_phents(EvElfParser* self,
                         EvViewContext* view_ctx,
                         GInputStream* stream,
                         GCancellable* cancellable,
                         GError** error)
{
  page_builder_new();
  failable_begin();

  gsize entries; take_var_p(header, e_phnum, &entries, gsize);
  if(entries == 0) return TRUE;
  goffset base; take_var_p(header, e_phoff, &base, goffset);
  gsize entsz; take_var_p(header, e_phentsize, &entsz, gsize);
  gsize i;

  gchar* title = g_strdup_printf("Program headers (%i)", entries);
  page_builder_begin(view_ctx, title, TRUE);
  g_free(title);

  union {
    elf_phent32_t _32;
    elf_phent64_t _64;
  } ents;

  for(i = 0;i < entries;i++)
  {

    get_a(&ents, entsz, base + (i * entsz), G_SEEK_SET);

    gchar* title = g_strdup_printf("(%i) %s", (gint)i, _s_elfphtype(ents._32.p_type));
    page_builder_begin(view_ctx, title, FALSE);
    g_free(title);

    switch(header32->bitlen)
    {
    case elf_bitlen_32:
#define entry (&(ents._32))
      page_builder_printp("p_type");
      page_builder_printv("%s (0x%x)", _s_elfphtype(entry->p_type), entry->p_type);
      page_builder_setrow(FALSE);

      page_builder_printp("p_offset");
      page_builder_printv("0x%08" PRIx32, entry->p_offset);
      page_builder_setrow(FALSE);

      page_builder_printp("p_vaddr");
      page_builder_printv("0x%08" PRIx32, entry->p_vaddr);
      page_builder_setrow(FALSE);

      page_builder_printp("p_paddr");
      page_builder_printv("0x%08" PRIx32, entry->p_paddr);
      page_builder_setrow(FALSE);

      page_builder_printp("p_filesz");
      page_builder_printv("0x%08" PRIx32, entry->p_filesz);
      page_builder_setrow(FALSE);

      page_builder_printp("p_memsz");
      page_builder_printv("0x%08" PRIx32, entry->p_memsz);
      page_builder_setrow(FALSE);

      page_builder_printp("p_align");
      page_builder_printv("0x%08" PRIx32, entry->p_align);
      page_builder_setrow(FALSE);

      page_builder_printp("p_flags");
      page_builder_printv("0x%08" PRIx32, entry->p_flags);
      page_builder_setrow(FALSE);
#undef entry
      break;
    case elf_bitlen_64:
#define entry (&(ents._64))
      page_builder_printp("p_type");
      page_builder_printv("%s (0x%x)", _s_elfphtype(entry->p_type), entry->p_type);
      page_builder_setrow(FALSE);

      page_builder_printp("p_offset");
      page_builder_printv("0x%016" PRIx64, entry->p_offset);
      page_builder_setrow(FALSE);

      page_builder_printp("p_vaddr");
      page_builder_printv("0x%016" PRIx64, entry->p_vaddr);
      page_builder_setrow(FALSE);

      page_builder_printp("p_paddr");
      page_builder_printv("0x%016" PRIx64, entry->p_paddr);
      page_builder_setrow(FALSE);

      page_builder_printp("p_filesz");
      page_builder_printv("0x%016" PRIx64, entry->p_filesz);
      page_builder_setrow(FALSE);

      page_builder_printp("p_memsz");
      page_builder_printv("0x%016" PRIx64, entry->p_memsz);
      page_builder_setrow(FALSE);

      page_builder_printp("p_align");
      page_builder_printv("0x%016" PRIx64, entry->p_align);
      page_builder_setrow(FALSE);

      page_builder_printp("p_flags");
      page_builder_printv("0x%08" PRIx32, entry->p_flags);
      page_builder_setrow(FALSE);
#undef entry
      break;
    }

    goffset base;
    gsize sz;

    switch(header32->bitlen)
    {
    case elf_bitlen_32:
      base = ((elf_phent32_t*)&ents)->p_offset;
      sz = ((elf_phent32_t*)&ents)->p_filesz;
      break;
    case elf_bitlen_64:
      base = ((elf_phent64_t*)&ents)->p_offset;
      sz = ((elf_phent64_t*)&ents)->p_filesz;
      break;
    }

    switch(ents._32.p_type)
    {
    case elf_phtype_interp:
      success =
      phtype_interp(self, page_r, base, sz, stream, cancellable, &tmp_err);
      break;
    case elf_phtype_note:
      success =
      phtype_note(self, page_r, base, sz, stream, cancellable, &tmp_err);
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
