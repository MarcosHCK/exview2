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
#include <ev2_settings.h>
#include <ev2_settings_connector.h>
#include <gtk/gtk.h>

static
void gtk_buildable_iface_init(GtkBuildableIface* iface);

typedef struct _ParserData ParserData;

G_DEFINE_QUARK(ev-settings-connector-builder-data-list-quark,
               ev_settings_connector_builder_data_list);

G_DEFINE_QUARK(ev-settings-connector-error-quark,
               ev_settings_connector_error);

/*
 * Object definition
 *
 */

struct _EvSettingsConnector
{
  GObject parent_instance;

  /*<private>*/
  EvSettings* source;
};

enum {
  prop_0,
  prop_source,
  prop_number,
};

static
GParamSpec* properties[prop_number] = {0};

G_DEFINE_TYPE_WITH_CODE
(EvSettingsConnector,
 ev_settings_connector,
 G_TYPE_OBJECT,
 G_IMPLEMENT_INTERFACE
 (GTK_TYPE_BUILDABLE,
  gtk_buildable_iface_init));

struct _ParserData
{
  gchar* schema;
  gchar* key;
  gchar* object;
  gchar* property;
  gint line;
  gint col;
};

#define goto_error() \
G_STMT_START { \
  success = FALSE; \
  goto _error_; \
} G_STMT_END

static
void parser_data_free(gpointer pdata) {
  ParserData* data = pdata;
  if G_LIKELY(pdata != NULL)
  {
    g_free(data->schema);
    g_free(data->key);
    g_free(data->object);
    g_free(data->property);

    g_slice_free(ParserData, pdata);
  }
}

static
void parser_data_list_free(gpointer plist) {
  GSList* list = plist;
  if G_LIKELY(plist != NULL)
  {
    g_slist_free_full(list, parser_data_free);
  }
}

static void
connection_parser_start(GMarkupParseContext* context,
                        const gchar* element_name,
                        const gchar** attributes_names,
                        const gchar** attributes_values,
                        gpointer user_data,
                        GError** error)
{
  if G_LIKELY(!strcmp(element_name, "connection"))
  {
    GError* tmp_err = NULL;
    gboolean success = TRUE;
    ParserData* data = user_data;

    success =
    g_markup_collect_attributes(element_name,
                                attributes_names,
                                attributes_values,
                                &tmp_err,
                                G_MARKUP_COLLECT_STRING, "schema", &(data->schema),
                                G_MARKUP_COLLECT_STRING, "key", &(data->key),
                                G_MARKUP_COLLECT_STRING, "object", &(data->object),
                                G_MARKUP_COLLECT_STRING, "property", &(data->property),
                                G_MARKUP_COLLECT_INVALID, NULL);

    if G_UNLIKELY(tmp_err != NULL)
    {
      g_propagate_error(error, tmp_err);
      memset(data, 0, sizeof(ParserData));
      return;
    }
    else
    {
      data->schema = g_strdup(data->schema);
      data->key = g_strdup(data->key);
      data->object = g_strdup(data->object);
      data->property = g_strdup(data->property);

      g_markup_parse_context_get_position
      (context,
       &(data->line),
       &(data->col));
    }
  }
  else
  {
    g_set_error
    (error,
     G_MARKUP_ERROR,
     G_MARKUP_ERROR_UNKNOWN_ELEMENT,
     "invalid element '%s'\r\n",
     element_name);
  }
}

static
const GMarkupParser connection_parser = {
  connection_parser_start,
};

static gboolean
gtk_buildable_iface_custom_tag_start(GtkBuildable* buildable,
                                     GtkBuilder* builder,
                                     GObject* child,
                                     const gchar* tagname,
                                     GMarkupParser* parser,
                                     gpointer* parser_data)
{
  if G_LIKELY(!strcmp(tagname, "connection"))
  {
    ParserData* data =
    g_slice_new(ParserData);

    GSList* list = (gpointer)
    g_object_steal_qdata(G_OBJECT(builder),
                         ev_settings_connector_builder_data_list_quark());

    list = g_slist_prepend(list, data);

    g_object_set_qdata_full
    (G_OBJECT(builder),
     ev_settings_connector_builder_data_list_quark(),
     list,
     parser_data_list_free);

    *parser_data = data;
    *parser = connection_parser;
    return TRUE;
  }
return FALSE;
}

/*
 * GTK Builder error
 *
 */

G_DEFINE_QUARK(lookup-error,
               gtk_builder_throw);

void _gtk_builder_throw_error(GtkBuilder* builder, GError* tmp_err) {
  g_object_set_qdata_full
  (G_OBJECT(builder),
   gtk_builder_throw_quark(),
   tmp_err,
   (GDestroyNotify)
   g_error_free);
}

static void
gtk_buildable_iface_parser_finished(GtkBuildable* buildable,
                                    GtkBuilder* builder)
{
  gboolean success = TRUE;
  GSList *ptr, *list = (gpointer)
  g_object_get_qdata
  (G_OBJECT(builder),
   ev_settings_connector_builder_data_list_quark());

  EvSettingsConnector* self = EV_SETTINGS_CONNECTOR(buildable);
  GSettings* settings = NULL;

  for(ptr = list;ptr != NULL;ptr = ptr->next)
  {
    ParserData* data = ptr->data;
    GObject* object = gtk_builder_get_object(builder, data->object);
    if G_UNLIKELY(object == NULL)
    {
      _gtk_builder_throw_error
      (builder,
       g_error_new
       (GTK_BUILDER_ERROR,
        GTK_BUILDER_ERROR_INVALID_ID,
        "Unknown object '%s'\r\n",
        data->object));
      goto_error();
    }

    success =
    ev_settings_has_key(self->source, data->schema, data->key);
    if G_LIKELY(success == TRUE)
    {
      settings = ev_settings_get_settings(self->source, data->schema);
      if G_UNLIKELY(settings == NULL)
      {
        _gtk_builder_throw_error
        (builder,
         g_error_new
         (EV_SETTINGS_CONNECTOR_ERROR,
          EV_SETTINGS_CONNECTOR_ERROR_UNKOWN_SCHEMA,
          "Unknown schema '%s'\r\n",
          data->schema));
        goto_error();
      }

      g_settings_bind
      (settings,
       data->key,
       object,
       data->property,
       G_SETTINGS_BIND_DEFAULT);
    }
    else
    {
      _gtk_builder_throw_error
      (builder,
       g_error_new
       (EV_SETTINGS_CONNECTOR_ERROR,
        EV_SETTINGS_CONNECTOR_ERROR_UNKOWN_KEY,
        "Unknown key '%s' on schema\r\n",
        data->key));
      goto_error();
    }

    g_clear_object(&settings);
  }

_error_:
  g_clear_object(&settings);
}

static
void gtk_buildable_iface_init(GtkBuildableIface* iface) {
  iface->custom_tag_start = gtk_buildable_iface_custom_tag_start;
  iface->parser_finished = gtk_buildable_iface_parser_finished;
}

static
GObject* ev_settings_connector_class_constructor(GType type, guint n_construct_params, GObjectConstructParam* construct_params) {
  static
  GObject* _g_object = NULL;

  if G_LIKELY(_g_object != NULL)
    g_object_ref(_g_object);
  else
  {
    _g_object =
    G_OBJECT_CLASS(ev_settings_connector_parent_class)->
    constructor(type, n_construct_params, construct_params);
  }
return _g_object;
}

static
void ev_settings_connector_class_set_property(GObject* pself, guint prop_id, const GValue* value, GParamSpec* pspec) {
  EvSettingsConnector* self = EV_SETTINGS_CONNECTOR(pself);
  switch(prop_id)
  {
  case prop_source:
    g_set_object(&(self->source), g_value_get_object(value));
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(pself, prop_id, pspec);
    break;
  }
}

static
void ev_settings_connector_class_dispose(GObject* pself) {
  EvSettingsConnector* self = EV_SETTINGS_CONNECTOR(pself);

/*
 * Dispose
 *
 */
  g_clear_object(&(self->source));

/*
 * Chain-up
 *
 */
  G_OBJECT_CLASS(ev_settings_connector_parent_class)->dispose(pself);
}

static
void ev_settings_connector_class_init(EvSettingsConnectorClass* klass) {
  GObjectClass* oclass = G_OBJECT_CLASS(klass);

/*
 * vtable
 *
 */
  oclass->constructor = ev_settings_connector_class_constructor;
  oclass->set_property = ev_settings_connector_class_set_property;
  oclass->dispose = ev_settings_connector_class_dispose;

/*
 * Properties
 *
 */
  properties[prop_source] =
    g_param_spec_object("settings-source",
                        "settings-source",
                        "settings-source",
                        EV_TYPE_SETTINGS,
                        G_PARAM_WRITABLE
                        | G_PARAM_STATIC_STRINGS);

  g_object_class_install_properties(oclass,
                                    prop_number,
                                    properties);
}

static
void ev_settings_connector_init(EvSettingsConnector* self) {
}

/*
 * Object methods
 *
 */

EvSettingsConnector*
ev_settings_connector_new() {
  return g_object_new(EV_TYPE_SETTINGS_CONNECTOR, NULL);
}
