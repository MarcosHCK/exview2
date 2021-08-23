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

typedef union {
  elf_dyn32_t _32;
  elf_dyn64_t _64;
} elf_dyn_t;

gboolean
_elf_parser_parse_dyninfo(EvElfParser* self,
                          EvViewContext* view_ctx,
                          GInputStream* stream,
                          GCancellable* cancellable,
                          GError** error)
{
  page_builder_new();
  failable_begin();

  gsize entries; take_var_p(header, e_phnum, &entries, gsize);
  if G_UNLIKELY(entries == 0)
    return TRUE;

  goffset base; take_var_p(header, e_phoff, &base, goffset);
  gsize entsz; take_var_p(header, e_phentsize, &entsz, gsize);
  GString* title = NULL;
  gchar* strtab = NULL;
  gsize i, j;

  union {
    elf_phent32_t _32;
    elf_phent64_t _64;
  } ents;
  union {
    elf_dyn32_t _32;
    elf_dyn64_t _64;
  } dyn;

  gsize infosz;
  switch(header32->bitlen)
  {
  case elf_bitlen_32:
    infosz = sizeof(elf_dyn32_t);
    break;
  case elf_bitlen_64:
    infosz = sizeof(elf_dyn64_t);
    break;
  }

  union {
    GArray array_;
    struct {
      elf_dyn_t *ents;
      guint len;
    };
  } *dyns = NULL;

  dyns = (gpointer)
  g_array_sized_new(0, 1, infosz, 0);

  for(i = 0;i < entries;i++)
  {
    get_a(&ents, entsz, base + (i * entsz), G_SEEK_SET);

    if(ents._32.p_type == elf_phtype_dynamic)
    {
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

      seek_a(base, G_SEEK_SET);

      for(j = 0;(j * infosz) < sz;j++)
      {
        get_s(&dyn, infosz);
        gboolean void_ = FALSE;
        switch(header32->bitlen)
        {
        case elf_bitlen_32:
#define dyn (&(dyn._32))
          void_ = dyn->d_tag == elf_dyn_null
                   && dyn->d_val == 0;
#undef dyn
          break;
        case elf_bitlen_64:
#define dyn (&(dyn._64))
          void_ = dyn->d_tag == elf_dyn_null
                   && dyn->d_val == 0;
#undef dyn
          break;
        }

        if(void_ == TRUE)
        {
          break;
        }

        g_array_append_vals(&(dyns->array_), &dyn, 1);
      }
    }
  }

  if G_LIKELY(dyns->len > 0)
  {
    goffset strtab_ = -1;
    gsize strsz = 0;

    for(i = 0;i < dyns->len;i++)
    {
      gint64 d_tag;
      guint64 d_ptr;

      switch(header32->bitlen)
      {
      case elf_bitlen_32:
#define dyn (&g_array_index(&(dyns->array_), elf_dyn32_t, i))
        d_tag = dyn->d_tag;
        d_ptr = dyn->d_ptr;
#undef dyn
        break;
      case elf_bitlen_64:
#define dyn (&g_array_index(&(dyns->array_), elf_dyn64_t, i))
        d_tag = dyn->d_tag;
        d_ptr = dyn->d_ptr;
#undef dyn
        break;
      }

      switch(d_tag)
      {
      case elf_dyn_strtab:
        strtab_ = d_ptr;
        break;
      case elf_dyn_strsz:
        strsz = d_ptr;
        break;
      }
    }

#define strtab strtab_
    goto_if_failed(strtab > 0 && strsz != 0);
#undef strtab
    strtab = g_malloc(strsz);
    get_a(strtab, strsz, strtab_, G_SEEK_SET);

    title = g_string_new(NULL);
    g_string_printf(title, "Dynamic info (%i)", dyns->len);
    page_builder_begin(view_ctx, title->str, TRUE);
    for(i = 0;i < dyns->len;i++)
    {
      g_string_printf(title, "%i", i);
      page_builder_begin(view_ctx, title->str, FALSE);

      guint32 type;
      guint64 val;

      switch(header32->bitlen)
      {
      case elf_bitlen_32:
#define dyn (&(dyns->ents[i]._32))
        page_builder_printp("d_tag");
        page_builder_printv("%s (%i)", _s_elfdynt(dyn->d_tag), dyn->d_tag);
        page_builder_setrow(FALSE);
        type = (guint32)dyn->d_tag;
        val = (guint64)dyn->d_ptr;

        page_builder_printp("d_val");
        page_builder_printv("0x%08" PRIx32, dyn->d_val);
        page_builder_setrow(FALSE);

        page_builder_printp("d_ptr");
        page_builder_printv("0x%08" PRIx32, dyn->d_ptr);
        page_builder_setrow(FALSE);
#undef dyn
        break;
      case elf_bitlen_64:
#define dyn (&(dyns->ents[i]._64))
        page_builder_printp("d_tag");
        page_builder_printv("%s (%i)", _s_elfdynt(dyn->d_tag), dyn->d_tag);
        page_builder_setrow(FALSE);
        type = (guint32)dyn->d_tag;
        val = (guint64)dyn->d_ptr;

        page_builder_printp("d_val");
        page_builder_printv("0x%016" PRIx64, dyn->d_val);
        page_builder_setrow(FALSE);

        page_builder_printp("d_ptr");
        page_builder_printv("0x%016" PRIx64, dyn->d_ptr);
        page_builder_setrow(FALSE);
#undef dyn
        break;
      }

      switch(type)
      {
      case elf_dyn_needed:
        goto_if_failed(strsz > val);
        page_builder_printp("Contents");
        page_builder_setrow(TRUE);

        g_string_insert(page_p, 0,
                        g_utf8_make_valid(strtab + val, -1));

        page_builder_setrow(FALSE);
        page_builder_closerow();
        break;
      case elf_dyn_soname:
        goto_if_failed(strsz > val);
        page_builder_printp("Contents");
        page_builder_setrow(TRUE);

        g_string_insert(page_p, 0,
                        g_utf8_make_valid(strtab + val, -1));

        page_builder_setrow(FALSE);
        page_builder_closerow();
        break;
      }
    }

    ev_view_context_ascend(view_ctx);
  }

error_tag()
  g_free(strtab);
  if G_LIKELY(dyns)
  {
    g_array_unref(&(dyns->array_));
  }
  if G_LIKELY(title)
  {
    g_string_free(title, TRUE);
  }

  page_builder_end();
return success;
}
