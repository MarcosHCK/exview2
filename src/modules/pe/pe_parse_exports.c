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

typedef union  _EvExportArray EvExportArray;
typedef struct _EvExportEntry EvExportEntry;

union _EvExportArray
{
  GArray array_;
  struct
  {
    struct _EvExportEntry
    {
      gchar* name;
      guint32 name_;
      gchar* from;
      vaddr_t addr;
      guint16 ordl;
    } *exports;

    guint len;
  };
};

#define _g_array_unref0(var) ((var == NULL) ? NULL : (var = (g_array_unref (var), NULL)))
#define _g_free0(var) ((var == NULL) ? NULL : (var = (g_free (var), NULL)))

static void
export_array_free(EvExportEntry* entry)
{
  _g_free0(entry->name);
  _g_free0(entry->from);
}

#define vaddr2offset(addr) \
  (G_GNUC_EXTENSION ({ \
    GError* tmp_err = NULL; \
    goffset offset = \
    _pe_parser_translate_vaddr(self, ((addr)), stream, cancellable, &tmp_err); \
    if G_UNLIKELY(tmp_err != NULL) \
    { \
      g_propagate_error(error, tmp_err); \
      goto_error(); \
    } \
    (goffset) offset; \
  }));

static
gchar* get_name_by_vaddr(EvPeParser* self,
                         vaddr_t vaddr,
                         GInputStream* stream,
                         GCancellable* cancellable,
                         GError** error)
{
  failable_begin();
  gchar* name = NULL;

  goffset name_ =
  vaddr2offset(vaddr);

  name =
  _pe_parser_load_string(self, name_, -1, NULL, stream, cancellable, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    goto_error();
  }
  else
  {
    goto_if_failed(name != NULL);
  }

error_tag()
  if G_UNLIKELY(success == FALSE)
    g_clear_pointer(&name, g_free);
return name;
}

static gboolean
load_exports(EvPeParser* self,
             GArray* exports,
             vaddr_t eat_,
             pe_export_descriptor_t *pdesc,
             GInputStream* stream,
             GCancellable* cancellable,
             GError** error)
{
  failable_begin();

/*
 * Load export
 *
 */

  pe_section_header_t* eat =
  _pe_parser_get_section_by_vaddr(self, eat_, stream, cancellable, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    goto_error();
  }
  else
  {
    goto_if_failed(eat != NULL);
  }

  goffset eat__ =
  eat->PointerToRawData
  + (eat_ - eat->VirtualAddress);
  pe_export_descriptor_t desc;

  get_a(&desc, sizeof(desc), eat__, G_SEEK_SET);
  memcpy(pdesc, &desc, sizeof(desc));

/*
 * Load forwarding parameters
 *
 */

  vaddr_t forwarded_base;
  vaddr_t forwarded_limit;
  switch(self->bitlen)
  {
  case pe_bitlen_32:
    forwarded_base = self->opt._32.DataDirectory[pe_directory_export].VirtualAddress;
    forwarded_limit = self->opt._32.DataDirectory[pe_directory_export].Size + forwarded_base;
    break;
  case pe_bitlen_64:
    forwarded_base = self->opt._64.DataDirectory[pe_directory_export].VirtualAddress;
    forwarded_limit = self->opt._64.DataDirectory[pe_directory_export].Size + forwarded_base;
    break;
  default:
    g_assert_not_reached();
    break;
  }

/*
 * Load arrays
 *
 */

  goffset addresses_ = vaddr2offset(desc.AddressOfFunctions);
  goffset names_ = vaddr2offset(desc.AddressOfNames);
  goffset ordinals_ = vaddr2offset(desc.AddressOfNameOrdinals);
  guint32* addresses = NULL;
  guint32* names = NULL;
  guint16* ordinals = NULL;
  gchar* name = NULL;
  gchar* from = NULL;
  guint i;

  addresses = g_new(guint32, desc.NumberOfFunctions);
  names = g_new(guint32, desc.NumberOfNames);
  ordinals = g_new(guint16, desc.NumberOfFunctions);

  get_a(addresses, sizeof(guint32) * desc.NumberOfFunctions, addresses_, G_SEEK_SET);
  get_a(names, sizeof(guint32) * desc.NumberOfNames, names_, G_SEEK_SET);
  get_a(ordinals, sizeof(guint16) * desc.NumberOfFunctions, ordinals_, G_SEEK_SET);

  for(i = 0;i < desc.NumberOfFunctions;i++)
  {
    if(i < desc.NumberOfNames)
    {
      name = get_name_by_vaddr(self, names[i], stream, cancellable, &tmp_err);
      if G_UNLIKELY(tmp_err != NULL)
      {
        g_propagate_error(error, tmp_err);
        goto_error();
      }
    }

    if(addresses[i] >= forwarded_base
       && forwarded_limit > addresses[i])
    {
      from = get_name_by_vaddr(self, names[i], stream, cancellable, &tmp_err);
      if G_UNLIKELY(tmp_err != NULL)
      {
        g_propagate_error(error, tmp_err);
        goto_error();
      }
    }

    EvExportEntry entry;
    entry.name = g_steal_pointer(&name);
    entry.from = g_steal_pointer(&from);
    entry.name_ = names[i];
    entry.addr = addresses[i];
    entry.ordl = desc.Base + ordinals[i];

    g_array_append_vals
    (exports,
     &entry,
     1);
  }

error_tag()
  _g_free0(addresses);
  _g_free0(names);
  _g_free0(ordinals);
  _g_free0(name);
  _g_free0(from);
return success;
}

gboolean
_pe_parser_parse_exports(EvPeParser* self,
                         EvViewContext* view_ctx,
                         GInputStream* stream,
                         GCancellable* cancellable,
                         GError** error)
{
  page_builder_new();
  failable_begin();
  vaddr_t eat_ = 0;
  guint i;

/*
 * Search data directory for import table
 *
 */

  switch(self->bitlen)
  {
  case pe_bitlen_32:
    if(pe_directory_export > self->opt._32.NumberOfRvaAndSizes)
      return TRUE;
    eat_ = self->opt._32.DataDirectory[pe_directory_export].VirtualAddress;
    break;
  case pe_bitlen_64:
    if(pe_directory_export > self->opt._64.NumberOfRvaAndSizes)
      return TRUE;
    eat_ = self->opt._64.DataDirectory[pe_directory_export].VirtualAddress;
    break;
  }

  goto_if_failed(eat_ != 0);
  pe_export_descriptor_t desc;
  EvExportArray* exports = (EvExportArray*)
  g_array_new(0, 0, sizeof(EvExportEntry));
  g_array_set_clear_func
  (&(exports->array_),
   (GDestroyNotify)
   export_array_free);

/*
 * Load exports
 *
 */
  success =
  load_exports(self, &(exports->array_), eat_, &desc, stream, cancellable, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    goto_error();
  }

  if G_LIKELY(desc.NumberOfFunctions > 0)
  {
    g_string_printf(page_p, "Exports (%i)", desc.NumberOfFunctions);
    page_builder_begin(view_ctx, page_p->str, TRUE);

    page_builder_printp("Characteristics");
    page_builder_printv("0x%08" PRIx32, desc.Characteristics);
    page_builder_setrow(FALSE);

    page_builder_printp("TimeDateStamp");
    page_builder_printv("%s (0x%08" PRIx32 ")", _s_petimestamp(desc.TimeDateStamp), desc.TimeDateStamp);
    page_builder_setrow(FALSE);

    page_builder_printp("MajorVersion");
    page_builder_printv("0x%04" PRIx16, desc.MajorVersion);
    page_builder_setrow(FALSE);

    page_builder_printp("MinorVersion");
    page_builder_printv("0x%04" PRIx16, desc.MinorVersion);
    page_builder_setrow(FALSE);

    page_builder_printp("Name");
    page_builder_printv("0x%08" PRIx32, desc.Name);
    page_builder_setrow(FALSE);

    page_builder_printp("Base");
    page_builder_printv("0x%08" PRIx32, desc.Base);
    page_builder_setrow(FALSE);

    page_builder_printp("NumberOfFunctions");
    page_builder_printv("0x%08" PRIx32, desc.NumberOfFunctions);
    page_builder_setrow(FALSE);

    page_builder_printp("NumberOfNames");
    page_builder_printv("0x%08" PRIx32, desc.NumberOfNames);
    page_builder_setrow(FALSE);

    page_builder_printp("AddressOfFunctions");
    page_builder_printv("0x%08" PRIx32, desc.AddressOfFunctions);
    page_builder_setrow(FALSE);

    page_builder_printp("AddressOfNames");
    page_builder_printv("0x%08" PRIx32, desc.AddressOfNames);
    page_builder_setrow(FALSE);

    page_builder_printp("AddressOfNameOrdinals");
    page_builder_printv("0x%08" PRIx32, desc.AddressOfNameOrdinals);
    page_builder_setrow(FALSE);

    for(i = 0;i < exports->len;i++)
    {
      EvExportEntry* export_ = &(exports->exports[i]);

      if(export_->name != NULL)
      {
        g_string_truncate(page_p, 0);
        g_string_insert(page_p, 0, export_->name);
        page_builder_begin(view_ctx, page_p->str, FALSE);
      }
      else
      {
        g_string_printf(page_p, "%i", export_->ordl);
        page_builder_begin(view_ctx, page_p->str, FALSE);
      }

      if(export_->name)
      {
        page_builder_printp("Name");
        page_builder_printv("%s (%i)", export_->name, export_->name_);
        page_builder_setrow(FALSE);
      }

      page_builder_printp("Ordinal");
      g_string_printf(page_v, "%i", export_->ordl);
      page_builder_setrow(FALSE);

      if(export_->from)
      {
        page_builder_printp("Forwarded");
        g_string_truncate(page_v, 0);
        g_string_insert(page_v, 0, export_->from);
        page_builder_setrow(FALSE);
      }
      else
      {
        page_builder_printp("Address");

        switch(self->bitlen)
        {
        case pe_bitlen_32:
          g_string_printf(page_v, "0x%08" PRIx32, export_->addr);
          break;
        case pe_bitlen_64:
          g_string_printf(page_v, "0x%016" PRIx64, export_->addr);
          break;
        }

        page_builder_setrow(FALSE);
      }
    }

    ev_view_context_ascend(view_ctx);
  }
error_tag()
  g_array_unref(&(exports->array_));
  page_builder_end();
return success;
}
