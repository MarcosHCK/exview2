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
  elf_sym32_t _32;
  elf_sym64_t _64;
} elf_sym_t;

typedef union {
  GArray array_;
  struct {
    struct _sym_ent {
      gchar* name;
      gchar* section;
      guint sh_link;
      elf_sym_t o;
    } *ents;
    guint len;
  };
} sym_array_t;

static void
ent_free(gpointer pent)
{
  struct _sym_ent* ent = pent;
  g_free(ent->name);
  g_free(ent->section);
}

static gpointer
ent_copy(gpointer pdst,
         gpointer psrc)
{
  struct _sym_ent* dst = pdst;
  struct _sym_ent* src = psrc;

  memcpy(dst, src, sizeof(*src));

  dst->name = g_strdup(src->name);
  dst->section = g_strdup(src->section);
}

typedef gboolean (*symbol_sort_func) (EvElfParser* self, elf_sym_t* sym, gpointer user_data);

static gchar*
symbol_name(EvElfParser* self,
            guint32 st_name,
            guint32 sh_link,
            GInputStream* stream,
            GCancellable* cancellable,
            GError** error)
{
  failable_begin();
  gchar* name = NULL;

  gchar* quark_string = g_strdup_printf("elf_section%i_strings_quark", sh_link);
  GQuark quark = g_quark_from_string(quark_string);
  g_free(quark_string);

  union {
    elf_shent32_t _32;
    elf_shent64_t _64;
  } ents;

  success =
  _elf_parser_get_section_entry(self, sh_link, &(ents._64), stream, cancellable, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    goto_error();
  }

  gsize size; take_var_s(ents._, sh_size, &size, gsize);
  goto_if_failed(size > st_name);

  const gchar* strings =
  g_object_get_qdata(G_OBJECT(self), quark);
  if G_LIKELY(strings != NULL)
  {
    name = g_utf8_make_valid(strings + st_name, -1);
  }
  else
  {
    goffset base; take_var_s(ents._, sh_offset, &base, goffset);
    gchar* buffer = g_malloc(size);

    get_a(buffer, size, base, G_SEEK_SET);
    g_object_set_qdata_full
    (G_OBJECT(self),
     quark,
     buffer,
     g_free);

    name = symbol_name(self, st_name, sh_link, stream, cancellable, &tmp_err);
    if G_UNLIKELY(tmp_err != NULL)
    {
      g_propagate_error(error, tmp_err);
      goto_error();
    }
  }

error_tag()
  if G_LIKELY(success == FALSE)
  {
    g_clear_pointer(&name, g_free);
  }
return name;
}

static gchar*
section_name(EvElfParser* self,
             guint16 sectidx,
             GInputStream* stream,
             GCancellable* cancellable,
             GError** error)
{
  failable_begin();
  gchar* name;

  union {
    elf_shent32_t _32;
    elf_shent64_t _64;
  } ents;

  success =
  _elf_parser_get_section_entry(self, sectidx, &(ents._64), stream, cancellable, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    goto_error();
  }

  guint32 sh_name;
  take_var_s(ents._, sh_name, &sh_name, guint32);

  name =
  _elf_parser_get_string(self, sh_name, stream, cancellable, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    goto_error();
  }

error_tag()
  if G_UNLIKELY(success == FALSE)
  {
    g_clear_pointer(&name, g_free);
  }
return name;
}

static sym_array_t*
list_all_symbols(EvElfParser* self,
                 GInputStream* stream,
                 GCancellable* cancellable,
                 GError** error)
{
  failable_begin();
  sym_array_t* syms = (gpointer)
  g_array_new(0, 1, sizeof(struct _sym_ent));

  gsize entries; take_var_p(header, e_shnum, &entries, gsize);
  if G_UNLIKELY(entries == 0)
    return NULL;

  gsize i, j;

  union {
    elf_shent32_t _32;
    elf_shent64_t _64;
  } ents;

  for(i = 0;i < entries;i++)
  {
    success =
    _elf_parser_get_section_entry(self, i, &(ents._64), stream, cancellable, &tmp_err);
    if G_UNLIKELY(tmp_err != NULL)
    {
      g_propagate_error(error, tmp_err);
      goto_error();
    }

    if(ents._32.sh_type == elf_shtype_dynsym)
    {
      goffset base; take_var_s(ents._, sh_offset, &base, goffset);
      gsize sz; take_var_s(ents._, sh_size, &sz, gsize);
      guint sh_link; take_var_s(ents._, sh_link, &sh_link, guint);

      gsize entsz;
      switch(header32->bitlen)
      {
      case elf_bitlen_32:
        entsz = sizeof(elf_sym32_t);
        break;
      case elf_bitlen_64:
        entsz = sizeof(elf_sym64_t);
        break;
      }

      seek_a(base + entsz, G_SEEK_SET);
      for(j = 1;(j * entsz) < sz;j++)
      {
        guint cur = syms->len;
        g_array_set_size(&(syms->array_), cur + 1);

        get_s(&(syms->ents[cur].o), entsz);
        syms->ents[cur].sh_link = sh_link;
      }
    }
  }

error_tag()
  if G_UNLIKELY(success == FALSE)
  {
    g_array_unref(&(syms->array_));
    syms = NULL;
  }
return syms;
}

G_DEFINE_QUARK(elf-parser-symbol-list-quark,
               elf_parser_symbol_list);

static sym_array_t*
list_symbols(EvElfParser* self,
             symbol_sort_func sort_func,
             gpointer user_data,
             GInputStream* stream,
             GCancellable* cancellable,
             GError** error)
{
  failable_begin();
  sym_array_t* syms = (gpointer)
  g_array_new(0, 1, sizeof(struct _sym_ent));
  g_array_set_clear_func(&(syms->array_), ent_free);
  guint i;

  sym_array_t* all = (gpointer)
  g_object_get_qdata(G_OBJECT(self),
                     elf_parser_symbol_list_quark());
  if G_UNLIKELY(all == NULL)
  {
    all =
    list_all_symbols(self, stream, cancellable, &tmp_err);
    if(all == NULL)
    {
      g_propagate_error(error, tmp_err);
      goto_error();
    }

    g_object_set_qdata_full
    (G_OBJECT(self),
     elf_parser_symbol_list_quark(),
     all,
     (GDestroyNotify)
     g_array_unref);
  }

  for(i = 0;i < all->len;i++)
  {
    struct _sym_ent* ent = &(all->ents[i]);
    if(sort_func(self, &(ent->o), user_data))
    {
      if G_UNLIKELY(ent->name == NULL)
      {
        guint32 st_name = 0;
        take_var_s(ent->o._, st_name, &st_name, guint32);

        if G_LIKELY(st_name > 0)
        {
          ent->name =
          symbol_name(self, st_name, all->ents[i].sh_link, stream, cancellable, &tmp_err);
          if G_UNLIKELY(tmp_err != NULL)
          {
            g_propagate_error(error, tmp_err);
            goto_error();
          }
        }
        else
        {
          ent->name = g_strdup("UND");
        }
      }

      if G_UNLIKELY(ent->section == NULL)
      {
        guint16 st_shndx = 0;
        take_var_s(ent->o._, st_shndx, &st_shndx, guint16);

        if G_LIKELY
        (st_shndx > 0
         && !(st_shndx >= 0xff00
         && st_shndx <= 0xffff))
        {
          ent->section =
          section_name(self, st_shndx, stream, cancellable, &tmp_err);
          if G_UNLIKELY(tmp_err != NULL)
          {
            g_propagate_error(error, tmp_err);
            goto_error();
          }
        }
        else
        {
          ent->section = g_strdup("UND");
        }
      }

      guint cur = syms->len;
      g_array_set_size(&(syms->array_), cur + 1);
      ent_copy(&(syms->ents[cur]), &(all->ents[i]));
    }
  }

error_tag()
  if G_UNLIKELY(success == FALSE)
  {
    g_array_unref(&(syms->array_));
    syms = NULL;
  }
return syms;
}

static void
report_symbol(EvElfParser* self,
              EvViewContext* view_ctx,
              struct _sym_ent* ent)
{
  page_builder_new();
  gchar* name = NULL;

  page_builder_begin(view_ctx, ent->name, FALSE);

#define gett(T, n) \
  T n; take_var_s(ent->o._, n, & n, T)

  gett(guint32, st_name);
  gett(guint32, st_shndx);
  gett(guint64, st_value);
  gett(guint64, st_size);
  gett(guint8, st_info);
  gett(guint8, st_other);
#undef gett

  const gchar* intfmt;
  switch(header32->bitlen)
  {
  case elf_bitlen_32:
    intfmt = "0x%08" PRIx32;
    break;
  case elf_bitlen_64:
    intfmt = "0x%016" PRIx64;
    break;
  }

  page_builder_printp("st_name");
  page_builder_printv("%s (0x%x)", ent->name, st_name);
  page_builder_setrow(FALSE);

  page_builder_printp("st_value");
  page_builder_printv(intfmt, st_value);
  page_builder_setrow(FALSE);

  page_builder_printp("st_size");
  page_builder_printv(intfmt, st_size);
  page_builder_setrow(FALSE);

  page_builder_printp("st_info");
  page_builder_printv("0x%02" PRIx8, st_info);
  page_builder_setrow(TRUE);

    page_builder_printp("bind");
    page_builder_printv("%s", _s_elfsbind((st_info >> 4) & 0xf));
    page_builder_setrow(FALSE);

    page_builder_printp("type");
    page_builder_printv("%s", _s_elfstype((st_info) & 0xf));
    page_builder_setrow(FALSE);

  page_builder_closerow();

  page_builder_printp("st_other");
  page_builder_printv("0x%02" PRIx8, st_other);
  page_builder_setrow(TRUE);

    page_builder_printp("visibility");
    page_builder_printv("%s", _s_elfsvis(st_other));
    page_builder_setrow(FALSE);

  page_builder_closerow();

  page_builder_printp("st_shndx");
  page_builder_printv("%s (0x%x)", ent->section, st_shndx);
  page_builder_setrow(FALSE);

error_tag()
  g_free(name);
page_builder_end();
}

static gboolean
list_symbols_of_type(EvElfParser* self,
                     EvViewContext* view_ctx,
                     symbol_sort_func sort_func,
                     gpointer user_data,
                     const gchar* kind,
                     GInputStream* stream,
                     GCancellable* cancellable,
                     GError** error)
{
  page_builder_new();
  failable_begin();
  sym_array_t* syms = NULL;
  guint i;

  syms = list_symbols(self, sort_func, user_data, stream, cancellable, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    goto_error();
  }

  if G_LIKELY(syms->len > 0)
  {
    gchar* title = g_strdup_printf("%s symbols (%i)", kind, syms->len);
    page_builder_begin(view_ctx, title, TRUE);
    g_free(title);

    for(i = 0;i < syms->len;i++)
    {
      report_symbol(self, view_ctx, &g_array_index(&(syms->array_), struct _sym_ent, i));
    }

    ev_view_context_ascend(view_ctx);
  }
error_tag()
  if(syms)
  {
    g_array_unref(&(syms->array_));
  }
  page_builder_end();
return success;
}

static gboolean
sort_func_local(EvElfParser* self,
                elf_sym_t* sym,
                gpointer user_data)
{
  switch(header32->bitlen)
  {
  case elf_bitlen_32:
    return ((sym->_32.st_info >> 4) == elf_bind_local);
    break;
  case elf_bitlen_64:
    return ((sym->_64.st_info >> 4) == elf_bind_local);
    break;
  }

  g_assert_not_reached();
return FALSE;
}

static gboolean
sort_func_bind(EvElfParser* self,
               elf_sym_t* sym,
               gpointer user_data)
{
  gint type = GPOINTER_TO_UINT(user_data);
  switch(header32->bitlen)
  {
  case elf_bitlen_32:
    return ((sym->_32.st_info >> 4) & type);
    break;
  case elf_bitlen_64:
    return ((sym->_64.st_info >> 4) & type);
    break;
  }

  g_assert_not_reached();
}

static gboolean
sort_func_undefined(EvElfParser* self,
                    elf_sym_t* sym,
                    gpointer user_data)
{
  gint type = GPOINTER_TO_UINT(user_data);
  switch(header32->bitlen)
  {
  case elf_bitlen_32:
    return
    sort_func_bind(self, sym, GINT_TO_POINTER(elf_bind_global))
    && ((sym->_32.st_shndx > 0)
        ^ GPOINTER_TO_INT(user_data));
    break;
  case elf_bitlen_64:
    return
    sort_func_bind(self, sym, GINT_TO_POINTER(elf_bind_global))
    && ((sym->_64.st_shndx > 0)
        ^ GPOINTER_TO_INT(user_data));
    break;
  }

  g_assert_not_reached();
}

gboolean
_elf_parser_parse_symbols(EvElfParser* self,
                          EvViewContext* view_ctx,
                          GInputStream* stream,
                          GCancellable* cancellable,
                          GError** error)
{
  page_builder_new();
  failable_begin();

  success =
  list_symbols_of_type(self, view_ctx, sort_func_local, GINT_TO_POINTER(elf_bind_local), "Local", stream, cancellable, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    goto_error();
  }

  success =
  list_symbols_of_type(self, view_ctx, sort_func_bind, GINT_TO_POINTER(elf_bind_global), "Global", stream, cancellable, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    goto_error();
  }

  success =
  list_symbols_of_type(self, view_ctx, sort_func_bind, GINT_TO_POINTER(elf_bind_weak), "Weak", stream, cancellable, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    goto_error();
  }

  success =
  list_symbols_of_type(self, view_ctx, sort_func_undefined, GINT_TO_POINTER(0), "Exported", stream, cancellable, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    goto_error();
  }

  success =
  list_symbols_of_type(self, view_ctx, sort_func_undefined, GINT_TO_POINTER(1), "Imported", stream, cancellable, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    goto_error();
  }

error_tag()
  page_builder_end();
return success;
}
