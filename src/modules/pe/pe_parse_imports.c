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

typedef union  _EvFuncArray EvFuncArray;
typedef struct _EvFuncEntry EvFuncEntry;
typedef union  _EvDllArray  EvDllArray;
typedef struct _EvDllEntry  EvDllEntry;

union _EvFuncArray
{
  GArray array_;
  struct
  {
    struct _EvFuncEntry
    {
      gchar* name;
      union
      {
        guint16 ordinal;
        guint16 hint;
      };

      vaddr_t thunk;
    } *funcs;

    guint len;
  };
};

union _EvDllArray
{
  GArray array_;
  struct
  {
    struct _EvDllEntry
    {
      gchar* name;
      EvFuncArray* funcs;
      guint32 characteristics;
      guint32 forwarder_chain;
      guint32 first_thunk;
    } *dlls;

    guint len;
  };
};

#define _g_array_unref0(var) ((var == NULL) ? NULL : (var = (g_array_unref (var), NULL)))
#define _g_free0(var) ((var == NULL) ? NULL : (var = (g_free (var), NULL)))

static
void func_array_free(EvFuncEntry* entry)
{
  _g_free0(entry->name);
}

static
void dll_array_free(EvDllEntry* entry)
{
  GArray* _tmp0_ = &(entry->funcs->array_);
  _g_array_unref0(_tmp0_);
  _g_free0(entry->name);
}

static gboolean
load_imports(EvPeParser* self,
             GArray* funcs,
             vaddr_t chain_,
             GInputStream* stream,
             GCancellable* cancellable,
             GError** error)
{
  failable_begin();
  EvFuncEntry entry = {0};
  gchar* name = NULL;
  vaddr_t thunk = 0;

  gsize thunk_sz = 0;
  vaddr_t thunk_mask = 1;
  switch(self->bitlen)
  {
  case pe_bitlen_32:
    thunk_sz = 4;
    thunk_mask <<= 31;
    break;
  case pe_bitlen_64:
    thunk_sz = 8;
    thunk_mask <<= 63;
    break;
  }

/*
 * Load section header which has thunk chain
 *
 */
  pe_section_header_t* chain =
  _pe_parser_get_section_by_vaddr(self, chain_, stream, cancellable, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    goto_error();
  }
  else
  {
    goto_if_failed(chain != NULL);
  }

  goffset read, chain__ =
  chain->PointerToRawData
  + (chain_ - chain->VirtualAddress);

  for(read = 0;TRUE;read += thunk_sz)
  {
    get_a
    (&thunk,
     thunk_sz,
      chain__ + read,
      G_SEEK_SET);

    if(thunk == 0)
      break;

/*
 * Load thunk name
 *
 */
    if((thunk & thunk_mask) == 0)
    {
      goffset name_ =
      _pe_parser_translate_vaddr(self, thunk, stream, cancellable, &tmp_err);
      if G_UNLIKELY(tmp_err != NULL)
      {
        g_propagate_error(error, tmp_err);
        goto_error();
      }

      get_a
      (&(entry.hint),
       sizeof(entry.hint),
       name_,
       G_SEEK_SET);

      name =
      _pe_parser_load_string(self, name_ + 2, -1, NULL, stream, cancellable, &tmp_err);
      if G_UNLIKELY(tmp_err != NULL)
      {
        g_propagate_error(error, tmp_err);
        goto_error();
      }

      entry.name = g_steal_pointer(&name);
    }
    else
    {
      entry.hint = thunk & (~thunk_mask);
    }

    entry.thunk = thunk;

/*
 * Finish load inserting into dll array
 *
 */
    g_array_append_vals(funcs, &entry, 1);
  }
error_tag()
  _g_free0(name);
return success;
}

static gboolean
load_libraries(EvPeParser* self,
               GArray* libraries,
               vaddr_t iat_,
               GInputStream* stream,
               GCancellable* cancellable,
               GError** error)
{
  failable_begin();
  EvDllEntry entry = {0};
  pe_import_descriptor_t desc;
  gchar* name = NULL;

/*
 * Load section header which has import table
 *
 */
  pe_section_header_t* iat =
  _pe_parser_get_section_by_vaddr(self, iat_, stream, cancellable, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    goto_error();
  }
  else
  {
    goto_if_failed(iat != NULL);
  }

  goffset read, iat__ =
  iat->PointerToRawData
  + (iat_ - iat->VirtualAddress);

  for(read = 0;TRUE;read += sizeof(desc))
  {
    get_a
    (&desc,
     sizeof(desc),
     iat__ + read,
     G_SEEK_SET);

    if(desc.Name == 0)
      break;

/*
 * Load library name
 *
 */
    goffset name_ =
    _pe_parser_translate_vaddr(self, desc.Name, stream, cancellable, &tmp_err);
    if G_UNLIKELY(tmp_err != NULL)
    {
      g_propagate_error(error, tmp_err);
      goto_error();
    }

    name =
    _pe_parser_load_string(self, name_, -1, NULL, stream, cancellable, &tmp_err);
    if G_UNLIKELY(tmp_err != NULL)
    {
      g_propagate_error(error, tmp_err);
      goto_error();
    }

    goto_if_failed(name != NULL);

/*
 * Load library metadata
 *
 */
    entry.name = g_steal_pointer(&name);
    entry.characteristics = desc.Characteristics;
    entry.forwarder_chain = desc.ForwarderChain;
    entry.first_thunk = desc.FirstThunk;

/*
 * Load funcs
 *
 */
    EvFuncArray* funcs = (EvFuncArray*)
    g_array_new(0, 0, sizeof(EvFuncEntry));
    g_array_set_clear_func
    (&(funcs->array_),
     (GDestroyNotify)
     func_array_free);
    entry.funcs = funcs;

    success =
    load_imports(self, &(funcs->array_), desc.FirstThunk, stream, cancellable, &tmp_err);
    if G_UNLIKELY(tmp_err != NULL)
    {
      g_propagate_error(error, tmp_err);
      goto_error();
    }

/*
 * Finish load inserting into dll array
 *
 */
    g_array_append_vals(libraries, &entry, 1);
  }
error_tag()
  g_free(name);
return success;
}

gboolean
_pe_parser_parse_imports(EvPeParser* self,
                         EvViewContext* view_ctx,
                         GInputStream* stream,
                         GCancellable* cancellable,
                         GError** error)
{
  page_builder_new();
  failable_begin();
  vaddr_t iat_ = 0;

/*
 * Search data directory for import table
 *
 */
  switch(self->bitlen)
  {
  case pe_bitlen_32:
    if(pe_directory_import > self->opt._32.NumberOfRvaAndSizes)
      return TRUE;
    iat_ = self->opt._32.DataDirectory[pe_directory_import].VirtualAddress;
    break;
  case pe_bitlen_64:
    if(pe_directory_import > self->opt._64.NumberOfRvaAndSizes)
      return TRUE;
    iat_ = self->opt._64.DataDirectory[pe_directory_import].VirtualAddress;
    break;
  }

  goto_if_failed(iat_ != 0);

/*
 * Load imports
 *
 */
  EvDllArray* dlls = (EvDllArray*)
  g_array_new(0, 0, sizeof(EvDllEntry));
  g_array_set_clear_func
  (&(dlls->array_),
   (GDestroyNotify)
   dll_array_free);

  success =
  load_libraries(self, &(dlls->array_), iat_, stream, cancellable, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    goto_error();
  }

/*
 * Render input
 *
 */
  if G_LIKELY(dlls->len > 0)
  {
    g_string_printf
    (page_p,
     "Imports (%i)",
     dlls->len);

    guint i, j;

    page_builder_begin(view_ctx, page_p->str, TRUE);
    for(i = 0;i < dlls->len;i++)
    {
      EvDllEntry* dll = &(dlls->dlls[i]);
      EvFuncArray* funcs = dll->funcs;

      g_string_printf
      (page_p,
       "%s (%i)",
       dll->name,
       funcs->len);

      page_builder_begin(view_ctx, page_p->str, TRUE);

      page_builder_printp("Characteristics");
      page_builder_printv("0x%08" PRIx32, dll->characteristics);
      page_builder_setrow(FALSE);

      page_builder_printp("ForwarderChain");
      page_builder_printv("0x%08" PRIx32, dll->forwarder_chain);
      page_builder_setrow(FALSE);

      page_builder_printp("FirstThunk");
      page_builder_printv("0x%08" PRIx32, dll->first_thunk);
      page_builder_setrow(FALSE);

      for(j = 0;j < funcs->len;j++)
      {
        EvFuncEntry* func = &(funcs->funcs[j]);

        if(func->name)
        {
          g_string_erase(page_p, 0, -1);
          g_string_insert(page_p, 0, func->name);
        }
        else
        {
          g_string_printf(page_p, "%i", func->ordinal);
        }

        page_builder_begin(view_ctx, page_p->str, FALSE);

        page_builder_printp("Thunk");
        switch(self->bitlen)
        {
        case pe_bitlen_32:
          page_builder_printv("0x%08" PRIx32, func->thunk & G_MAXUINT32);
          page_builder_setrow(FALSE);
          break;
        case pe_bitlen_64:
          page_builder_printv("0x%016" PRIx64, func->thunk & G_MAXUINT64);
          page_builder_setrow(FALSE);
          break;
        }

        if(func->name)
        {
          page_builder_printp("Name");
          g_string_erase(page_v, 0, -1);
          g_string_insert(page_v, 0, func->name);
          page_builder_setrow(FALSE);
        }

        page_builder_printp("Hint");
        page_builder_printv("%i", func->hint);
        page_builder_setrow(FALSE);
      }

      ev_view_context_ascend(view_ctx);
    }

    ev_view_context_ascend(view_ctx);
  }

error_tag()
  g_array_unref(&(dlls->array_));
  page_builder_end();
return success;
}
