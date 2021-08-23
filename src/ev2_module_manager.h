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
#ifndef __EV2_MODULE_MANAGER_INCLUDED__
#define __EV2_MODULE_MANAGER_INCLUDED__
#include <gio/gio.h>

/**
 * EV_MODULE_MANAGER_ERROR:
 *
 * Error domain for EV_MODULE_MANAGER. Errors in this domain will be from the #EvModuleManagerError enumeration.
 * See #GError for more information on error domains.
 */
#define EV_MODULE_MANAGER_ERROR (ev_module_manager_error_quark())

/**
 * EvModuleManagerError:
 * @EV_MODULE_MANAGER_ERROR_FAILED: generic error condition.
 * @EV_MODULE_MANAGER_ERROR_OPEN_ERROR: error occurred at module
 * load, specifically at #g_module_open() function.
 * @EV_MODULE_MANAGER_ERROR_UNDEFINED_INIT: initialization function
 * is not defined by loaded module.
 * @EV_MODULE_MANAGER_ERROR_UNKNOWN_FORMAT: there are not modules capable
 * of parse given input.
 *
 * Error code returned by #EvModuleManager API functions.
 * Note that %EV_MODULE_MANAGER_ERROR_FAILED is here only for compatibility with
 * error domain definition paradigm as defined on GLib documentation.
 */
typedef enum {
  EV_MODULE_MANAGER_ERROR_FAILED,
  EV_MODULE_MANAGER_ERROR_OPEN_ERROR,
  EV_MODULE_MANAGER_ERROR_UNDEFINED_INIT,
  EV_MODULE_MANAGER_ERROR_UNKNOWN_FORMAT,
} EvModuleManagerError;

#define EV_TYPE_MODULE_MANAGER            (ev_module_manager_get_type())
#define EV_MODULE_MANAGER(object)         (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_MODULE_MANAGER, EvModuleManager))
#define EV_MODULE_MANAGER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_MODULE_MANAGER, EvModuleManagerClass))
#define EV_IS_MODULE_MANAGER(object)      (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_MODULE_MANAGER))
#define EV_IS_MODULE_MANAGER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), EV_TYPE_MODULE_MANAGER))
#define EV_MODULE_MANAGER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), EV_TYPE_MODULE_MANAGER, EvModuleManagerClass))

typedef struct _EvModuleManager       EvModuleManager;
typedef struct _EvModuleManagerClass  EvModuleManagerClass;

#if __cplusplus
extern "C" {
#endif // __cplusplus

GType
ev_module_manager_get_type();
GQuark
ev_module_manager_error_quark();

struct _EvModuleManagerClass
{
  GObjectClass parent_class;
};

EvModuleManager*
ev_module_manager_new(GCancellable* cancellable,
                      GError** error);
void
ev_module_manager_load(EvModuleManager* manager,
                       GFile* filename,
                       GCancellable* cancellable,
                       GError** error);
void
ev_module_manager_load_all(EvModuleManager* manager,
                           GCancellable* cancellable,
                           GError** error);
GBytes**
ev_module_manager_list_snippets(EvModuleManager* manager,
                                gint* n_snippets);

#if __cplusplus
}
#endif // __cplusplus

#endif // __EV2_MODULE_MANAGER_INCLUDED__
