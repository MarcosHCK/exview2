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
 *  along with exview2.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <config.h>
#include <ev2_module.h>
#include <ev2_parser.h>

typedef union _GTypesArray GTypesArray;

G_DEFINE_QUARK(ev-module-error-quark,
               ev_module_error);

static
void ev_parser_iface_init(EvParserIface* iface);

/**
 * SECTION: ev2_module
 * @Short_description: an storage endpoint for modules metadata.
 * @Title: EvModule
 *
 * The #EvModule is an storage object for various metadata provided
 * by Exview2 modules. Also processes many of that data and provides
 * some services an facilities related with modules handling.
 */

struct _EvModule
{
  GObject parent_instance;

  /*<private>*/
  gchar* name;
  GBytes* snippet;
  union _GTypesArray
  {
    GArray array_;
    struct
    {
      GType* g_types;
      guint len;
    };
  } *g_types;
};

struct _EvModuleClass
{
  GObjectClass parent_class;
};

enum {
  prop_0,
  prop_name,
  prop_snippet,
  prop_number,
};

static
GParamSpec* properties[prop_number] = {0};

G_DEFINE_TYPE_WITH_CODE
(EvModule,
 ev_module,
 G_TYPE_OBJECT,
 G_IMPLEMENT_INTERFACE
 (EV_TYPE_PARSER,
  ev_parser_iface_init));

static
gboolean ev_parser_iface_parse(EvParser* pself, EvViewContext* view_ctx, GInputStream* stream, GCancellable* cancellable, GError** error) {
  g_return_val_if_fail(G_IS_SEEKABLE(stream), FALSE);
  EvModule* self = EV_MODULE(pself);
  GError* tmp_err = NULL;

  goffset start =
  g_seekable_tell(G_SEEKABLE(stream));
  guint i;

  for(i = 0;i < self->g_types->len;i++)
  {
    GType g_type = self->g_types->g_types[i];
    EvParser* parser = g_object_new(g_type, NULL);

    ev_parser_parse(parser, view_ctx, stream, cancellable, &tmp_err);
    if G_LIKELY(tmp_err != NULL)
    {
      g_object_unref(parser);

      if G_LIKELY
      (tmp_err->domain != EV_PARSER_ERROR
       || tmp_err->code != EV_PARSER_ERROR_UNPARSEABLE)
      {
        g_propagate_error(error, tmp_err);
        return FALSE;
      }

      g_clear_error(&tmp_err);

      g_seekable_seek
      (G_SEEKABLE(stream),
       start,
       G_SEEK_SET,
       cancellable,
       &tmp_err);
      if G_UNLIKELY(tmp_err != NULL)
      {
        g_propagate_error(error, tmp_err);
        return FALSE;
      }
    }
    else
    {
      return TRUE;
    }
  }
return FALSE;
}

static
void ev_parser_iface_init(EvParserIface* iface) {
  iface->parse = ev_parser_iface_parse;
}

static
void ev_module_class_get_property(GObject* pself, guint prop_id, GValue* value, GParamSpec* pspec) {
  EvModule* self = EV_MODULE(pself);
  switch(prop_id)
  {
  case prop_name:
    g_value_set_string(value, self->name);
    break;
  case prop_snippet:
    g_value_set_boxed(value, self->snippet);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(pself, prop_id, pspec);
    break;
  }
}

static
void ev_module_class_set_property(GObject* pself, guint prop_id, const GValue* value, GParamSpec* pspec) {
  EvModule* self = EV_MODULE(pself);
  switch(prop_id)
  {
  case prop_name:
    ev_module_set_name(self, g_value_get_string(value));
    break;
  case prop_snippet:
    ev_module_set_snippet(self, g_value_get_boxed(value));
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(pself, prop_id, pspec);
    break;
  }
}

static
void ev_module_class_finalize(GObject* pself) {
  EvModule* self = EV_MODULE(pself);

/*
 * Finalize
 *
 */
  if G_LIKELY(self->name != NULL)
    g_free(self->name);
  if G_LIKELY(self->snippet != NULL)
    g_bytes_unref((GBytes*) self->snippet);
  if G_LIKELY(self->g_types != NULL)
    g_array_unref((GArray*) self->g_types);

/*
 * Chain-up
 *
 */
  G_OBJECT_CLASS(ev_module_parent_class)->finalize(pself);
}

static
void ev_module_class_init(EvModuleClass* klass) {
  GObjectClass* oclass = G_OBJECT_CLASS(klass);

/*
 * vtable
 *
 */
  oclass->get_property = ev_module_class_get_property;
  oclass->set_property = ev_module_class_set_property;
  oclass->finalize = ev_module_class_finalize;

/*
 * Properties
 *
 */
  properties[prop_name] =
    g_param_spec_string("name",
                        "Name",
                        "Module name",
                        "unset",
                        G_PARAM_READWRITE
                        | G_PARAM_CONSTRUCT
                        | G_PARAM_STATIC_STRINGS);

  properties[prop_snippet] =
    g_param_spec_boxed("snippet",
                       "Snippet",
                       "Preferences snippet",
                       G_TYPE_BYTES,
                       G_PARAM_READWRITE
                       | G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties
  (oclass,
   prop_number,
   properties);
}

static
void ev_module_init(EvModule* self) {
  self->g_types = (GTypesArray*)
  g_array_sized_new(0, 1, sizeof(GType), 1);
}

/*
 * Object methods
 *
 */

EvModule*
_ev_module_new() {
return g_object_new(EV_TYPE_MODULE, NULL);
}

void
ev_module_set_name(EvModule* module,
                   const gchar* name)
{
  g_return_if_fail(EV_IS_MODULE(module));
  g_return_if_fail(name != NULL);

  if G_LIKELY(g_strcmp0(module->name, name) != 0)
  {
    g_clear_pointer(&(module->name), g_free);
    module->name = g_strdup(name);

    g_object_notify_by_pspec
    (G_OBJECT(module),
     properties[prop_name]);
  }
}

void
ev_module_set_snippet(EvModule* module,
                      GBytes* snippet)
{
  g_return_if_fail(EV_IS_MODULE(module));
  g_return_if_fail(snippet != NULL);

  if G_LIKELY
  (module->snippet != snippet
   && g_bytes_compare(module->snippet, snippet) != 0)
  {
    g_clear_pointer(&(module->snippet), g_bytes_unref);
    module->snippet = g_bytes_ref(snippet);

    g_object_notify_by_pspec
    (G_OBJECT(module),
     properties[prop_snippet]);
  }
}

static
gboolean g_type_compare(GType* g_type1, GType* g_type2) {
return (*g_type1) == (*g_type2);
}

void
ev_module_add_parser(EvModule* module,
                     GType g_type)
{
  g_return_if_fail(EV_IS_MODULE(module));
  g_return_if_fail(g_type_is_a(g_type, G_TYPE_OBJECT));

  if G_LIKELY
  (g_array_binary_search
   (&(module->g_types->array_),
    &g_type,
    (GCompareFunc)
    g_type_compare,
    NULL) == FALSE)
  {
    g_array_append_vals
    (&(module->g_types->array_),
     &g_type, 1);
  }
}

/*
 * Version check
 *
 */

#define BIT_MAYOR 0
#define BIT_MINOR 1
#define BIT_MICRO 2
#define BIT_BUILD 3

static
guint cur[4] = {0};

#define PRIiBITS "%i.%i.%i.%i"
#define EXPAND_BITS(b) b[0],b[1],b[2],b[3]

gboolean
ev_module_check_version(EvModule* module,
                        const gchar* expected,
                        GError** error)
{
  g_return_val_if_fail(EV_IS_MODULE(module), FALSE);
  g_return_val_if_fail(expected != NULL, FALSE);
  g_return_val_if_fail(error == NULL || *error == NULL, FALSE);

  guint bits[4] = {0}, n_bit = 0, i;
  const gchar* bit = expected;

/*
 * Load version bits
 *
 */
  static gint cur[4] = {0};
  if G_UNLIKELY
  (cur[0] == 0
   && cur[1] == 0
   && cur[2] == 0
   && cur[3] == 0)
  {
    cur[0] = (gint)g_strtod(PACKAGE_VERSION_MAYOR, NULL);
    cur[1] = (gint)g_strtod(PACKAGE_VERSION_MINOR, NULL);
    cur[2] = (gint)g_strtod(PACKAGE_VERSION_MICRO, NULL);
    cur[3] = (gint)g_strtod(PACKAGE_VERSION_BUILD, NULL);
  }

  do
  {
    g_return_val_if_fail(n_bit < 4, FALSE);
    bits[n_bit++] = (gint)g_strtod(bit, NULL);
    bit = g_strstr_len(bit, -1, ".");
  }
  while(bit && ++bit);

/*
 * Check if current binary is
 * older than module
 *
 */
  for(i = 0;i < G_N_ELEMENTS(bits);i++)
  {
    if G_UNLIKELY(bits[i] > cur[i])
    {
      g_set_error
      (error,
       EV_MODULE_ERROR,
       EV_MODULE_ERROR_INCOMPATIBLE_VERSION,
       "Exview2 expected version '" PRIiBITS "' is greater than this build '" PRIiBITS "'\r\n",
       EXPAND_BITS(bits), EXPAND_BITS(cur));
      return FALSE;
    }
  }

/*
 * Mayor build are expected to
 * be ABI-incompatible
 *
 */
  if G_UNLIKELY(bits[BIT_MAYOR] != cur[BIT_MAYOR])
  {
    g_set_error
    (error,
     EV_MODULE_ERROR,
     EV_MODULE_ERROR_INCOMPATIBLE_VERSION,
     "Exview2 expected version '" PRIiBITS "' doesn't match this build '" PRIiBITS "'\r\n",
     EXPAND_BITS(bits), EXPAND_BITS(cur));
    return FALSE;
  }

/*
 * If version mismatches
 * but reaches this point
 * means module should work
 * correctly, although I don't
 * recommend it so emit a little
 * warning
 *
 */
  for(i = 0;i < G_N_ELEMENTS(bits);i++)
  {
    if G_UNLIKELY(bits[i] > cur[i])
    {
      g_set_error
      (error,
       EV_MODULE_ERROR,
       EV_MODULE_ERROR_VERSION_MISMATCH,
       "Exview expected version '" PRIiBITS "' doesn't match this build '" PRIiBITS "'\r\n",
       EXPAND_BITS(bits), EXPAND_BITS(cur));
      return FALSE;
    }
  }
return TRUE;
}
