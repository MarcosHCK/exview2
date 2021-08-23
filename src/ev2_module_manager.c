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
#include <ev2_module_manager.h>
#include <ev2_parser.h>
#include <gmodule.h>

G_DEFINE_QUARK(ev-module-manager-error-quark,
               ev_module_manager_error);

typedef union  _EvPathArray   EvPathArray;
typedef struct _EvModuleEntry EvModuleEntry;
typedef union  _EvModuleArray EvModuleArray;

static
void g_initable_iface_init(GInitableIface* iface);
static
void ev_parser_iface_init(EvParserIface* iface);
static
void _ev_module_manager_load(EvModuleManager* manager,
                             const gchar* filename,
                             GCancellable* cancellable,
                             GError** error);

/*
 * Object definition
 *
 */

struct _EvModuleManager
{
  GObject parent_instance;

  /*<private>*/
  union _EvPathArray
  {
    GPtrArray array_;
    struct
    {
      GFile** paths;
      guint len;
    };
  } *paths;
  union _EvModuleArray
  {
    GArray array_;
    struct
    {
      struct _EvModuleEntry
      {
        union
        {
          EvModule* module;
          EvParser* parser;
        };

        GModule* handle;
      } *modules;
      guint len;
    };
  } *modules;
};

G_DEFINE_TYPE_WITH_CODE
(EvModuleManager,
 ev_module_manager,
 G_TYPE_OBJECT,
 G_IMPLEMENT_INTERFACE
 (G_TYPE_INITABLE,
  g_initable_iface_init)
 G_IMPLEMENT_INTERFACE
 (EV_TYPE_PARSER,
  ev_parser_iface_init));

static
gboolean g_initable_iface_init_sync(GInitable* pself, GCancellable* cancellable, GError** error) {
  GError* tmp_err = NULL;
  _ev_module_manager_load(EV_MODULE_MANAGER(pself), NULL, cancellable, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    if G_UNLIKELY
    (tmp_err->domain != EV_MODULE_MANAGER_ERROR
     || tmp_err->code != EV_MODULE_MANAGER_ERROR_UNDEFINED_INIT)
    {
      g_propagate_error(error, tmp_err);
      return FALSE;
    }

    g_error_free(tmp_err);
  }
return TRUE;
}

static
void g_initable_iface_init(GInitableIface* iface) {
  iface->init = g_initable_iface_init_sync;
}

static
gboolean ev_parser_iface_parse(EvParser* pself, EvViewContext* view_ctx, GInputStream* stream, GCancellable* cancellable, GError** error) {
  g_return_val_if_fail(G_IS_SEEKABLE(stream), FALSE);
  EvModuleManager* self = EV_MODULE_MANAGER(pself);
  GError* tmp_err = NULL;

  goffset start =
  g_seekable_tell(G_SEEKABLE(stream));
  guint i;

  for(i = 0;i < self->modules->len;i++)
  {
    EvParser* parser = self->modules->modules[i].parser;
    ev_parser_parse(parser, view_ctx, stream, cancellable, &tmp_err);
    if G_LIKELY(tmp_err != NULL)
    {
      g_object_unref(parser);

      if G_UNLIKELY
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

  g_set_error
  (error,
   EV_MODULE_MANAGER_ERROR,
   EV_MODULE_MANAGER_ERROR_UNKNOWN_FORMAT,
   "Unknown format\r\n");
return FALSE;
}

static
void ev_parser_iface_init(EvParserIface* iface) {
  iface->parse = ev_parser_iface_parse;
}


static
void ev_module_manager_class_finalize(GObject* pself) {
  EvModuleManager* self = EV_MODULE_MANAGER(pself);

/*
 * Finalize
 *
 */
  g_ptr_array_unref(&(self->paths->array_));
  g_array_unref(&(self->modules->array_));

/*
 * Chain-up
 *
 */
  G_OBJECT_CLASS(ev_module_manager_parent_class)->finalize(pself);
}

static
void ev_module_manager_class_dispose(GObject* pself) {
  EvModuleManager* self = EV_MODULE_MANAGER(pself);

/*
 * Dispose
 *
 */
  g_ptr_array_foreach
  (&(self->paths->array_),
   (GFunc)
   g_object_unref,
   NULL);

  guint i;
  for(i = 0;i < self->modules->len;i++)
  {
    g_object_unref(self->modules->modules[i].module);
    g_module_close(self->modules->modules[i].handle);
  }

/*
 * Chain-up
 *
 */
  G_OBJECT_CLASS(ev_module_manager_parent_class)->dispose(pself);
}

static
void ev_module_manager_class_init(EvModuleManagerClass* klass) {
  GObjectClass* oclass = G_OBJECT_CLASS(klass);

/*
 * vtable
 *
 */
  oclass->finalize = ev_module_manager_class_finalize;
  oclass->dispose = ev_module_manager_class_dispose;
}

static
void ev_module_manager_init(EvModuleManager* self) {
  self->paths = (EvPathArray*) g_ptr_array_new_full(1, NULL);
  self->modules = (EvModuleArray*) g_array_new(0, 1, sizeof(EvModuleEntry));
}

/*
 * Object methods
 *
 */

EvModuleManager*
ev_module_manager_new(GCancellable* cancellable,
                      GError** error)
{
  return
  g_initable_new
  (EV_TYPE_MODULE_MANAGER,
   cancellable,
   error,
   NULL);
}

void
ev_module_manager_add_path(EvModuleManager* manager,
                           const gchar* path)
{
  g_return_if_fail(EV_IS_MODULE_MANAGER(manager));
  g_return_if_fail(path != NULL);

  GFile* path_ = g_file_new_for_path(path);
  g_ptr_array_add(&(manager->paths->array_), path_);
}

EvModule* _ev_module_new();

#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
#define _g_module_close0(var) ((var == NULL) ? NULL : (var = (g_module_close (var), NULL)))

static gboolean
already_loaded(EvModuleManager* manager, GModule* handle) {
  guint i;
  for(i = 0;i < manager->modules->len;i++)
  {
    if(manager->modules->modules[i].handle == handle)
      return TRUE;
  }
return FALSE;
}

static void
_ev_module_manager_load(EvModuleManager* manager,
                        const gchar* filename,
                        GCancellable* cancellable,
                        GError** error)
{
  GModule* handle =
  g_module_open
  (filename,
   G_MODULE_BIND_LAZY);

  if G_UNLIKELY(handle == NULL)
  {
    g_set_error
    (error,
     EV_MODULE_MANAGER_ERROR,
     EV_MODULE_MANAGER_ERROR_OPEN_ERROR,
     "error at module load: %s\r\n",
     g_module_error());
    _g_module_close0(handle);
    return;
  }

  if(already_loaded(manager, handle))
  {
    _g_module_close0(handle);
    return;
  }

  EvModuleInitFunc init = NULL;
  if(g_module_symbol
     (handle,
      G_STRINGIFY(EV_MODULE_INIT_FUNCTION),
      (gpointer*) &init) == FALSE)
  {
    g_set_error
    (error,
     EV_MODULE_MANAGER_ERROR,
     EV_MODULE_MANAGER_ERROR_UNDEFINED_INIT,
     "error initializing module: %s\r\n",
     g_module_error());
    _g_module_close0(handle);
    return;
  }

  g_assert(init != NULL);

  EvModule* module = _ev_module_new();
  EvModuleEntry entry = {0};
  GError* tmp_err = NULL;

  init(module, cancellable, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    _g_object_unref0(module);
    _g_module_close0(handle);
    return;
  }

  g_module_make_resident(handle);

  entry.handle = handle;
  entry.module = module;

  g_array_append_vals
  (&(manager->modules->array_),
   &entry, 1);
}

void
ev_module_manager_load(EvModuleManager* manager,
                       GFile* filename,
                       GCancellable* cancellable,
                       GError** error)
{
  g_return_if_fail(EV_IS_MODULE_MANAGER(manager));
  g_return_if_fail(G_IS_FILE(filename));
  g_return_if_fail(error == NULL || *error == NULL);

  _ev_module_manager_load
  (manager,
   g_file_peek_path(filename),
   cancellable,
   error);
}

void
ev_module_manager_load_all(EvModuleManager* manager,
                           GCancellable* cancellable,
                           GError** error)
{
  g_return_if_fail(EV_IS_MODULE_MANAGER(manager));
  g_return_if_fail(error == NULL || *error == NULL);
  GError* tmp_err = NULL;

  guint i;
  for(i = 0;i < manager->paths->len;i++)
  {
    GFile* path = manager->paths->paths[i];

    GFileEnumerator* enumer =
    g_file_enumerate_children
    (path,
     G_FILE_ATTRIBUTE_STANDARD_TYPE,
     G_FILE_QUERY_INFO_NONE,
     cancellable,
     &tmp_err);

    if G_UNLIKELY(tmp_err != NULL)
    {
      g_propagate_error(error, tmp_err);
      _g_object_unref0(enumer);
      break;
    }

    for(;;)
    {
      GFile* child = NULL;
      GFileInfo* info = NULL;
      g_file_enumerator_iterate
      (enumer,
       &info, &child,
       cancellable,
       &tmp_err);

      if G_UNLIKELY(tmp_err != NULL)
      {
        g_propagate_error(error, tmp_err);
        _g_object_unref0(enumer);
        goto _error_;
      }

      if(child == NULL)
        break;

      GFileType type = g_file_info_get_attribute_uint32(info, G_FILE_ATTRIBUTE_STANDARD_TYPE);
      if(type == G_FILE_TYPE_REGULAR)
      {
        ev_module_manager_load(manager, child, cancellable, &tmp_err);
        if G_UNLIKELY(tmp_err != NULL)
        {
          g_propagate_error(error, tmp_err);
          _g_object_unref0(enumer);
          goto _error_;
        }
      }
    }

    g_file_enumerator_close(enumer, cancellable, &tmp_err);
    if G_UNLIKELY(tmp_err != NULL)
    {
      g_propagate_error(error, tmp_err);
      _g_object_unref0(enumer);
      break;
    }

    _g_object_unref0(enumer);
  }
_error_:
return;
}
