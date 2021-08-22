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
#ifndef __EV2_MODULE_INCLUDED__
#define __EV2_MODULE_INCLUDED__ 1
#include <gio/gio.h>
#include <gmodule.h>

/**
 * EV_MODULE_INIT_FUNCTION:
 *
 * Module entry point
 */
#define EV_MODULE_INIT_FUNCTION ev_module_entry

/**
 * EV_MODULE_ERROR:
 *
 * Error domain for EV_MODULE. Errors in this domain will be from the #EvModuleError enumeration.
 * See #GError for more information on error domains.
 */
#define EV_MODULE_ERROR (ev_module_error_quark())

/**
 * EvModuleError:
 * @EV_MODULE_ERROR_FAILED: generic error condition.
 * @EV_MODULE_ERROR_INCOMPATIBLE_VERSION: module requested version is
 * totally incompatible with exview2 version, which means any attempt to use
 * exported function from either side will lead to undefined behavior (very possibly
 * program crash).
 * @EV_MODULE_ERROR_VERSION_MISMATCH: module requested version mismatches with exview2
 * version, but is expected that they may work together without any accident. (Note that
 * #ev_module_check_version() will return FALSE in this case, so module must decide whether ignore
 * this error or not).
 *
 * Error code returned by #ev_module_check_version() function.
 * Note that %EV_MODULE_ERROR_FAILED is here only for compatibility with
 * error domain definition paradigm as defined on GLib documentation.
 */
typedef enum {
  EV_MODULE_ERROR_FAILED,
  EV_MODULE_ERROR_INCOMPATIBLE_VERSION,
  EV_MODULE_ERROR_VERSION_MISMATCH,
} EvModuleError;

#define EV_TYPE_MODULE            (ev_module_get_type())
#define EV_MODULE(object)         (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_MODULE, EvModule))
#define EV_MODULE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_MODULE, EvModuleClass))
#define EV_IS_MODULE(object)      (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_MODULE))
#define EV_IS_MODULE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), EV_TYPE_MODULE))
#define EV_MODULE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), EV_TYPE_MODULE, EvModuleClass))

typedef struct _EvModule      EvModule;
typedef struct _EvModuleClass EvModuleClass;

typedef gboolean (*EvModuleInitFunc) (EvModule* module, GCancellable* cancellable, GError** error);

#if __cplusplus
extern "C" {
#endif // __cplusplus

GType
ev_module_get_type();
GQuark
ev_module_error_quark();

void
ev_module_set_name(EvModule* module,
                   const gchar* name);
void
ev_module_set_snippet(EvModule* module,
                      GBytes* snippet);
void
ev_module_add_parser(EvModule* module,
                     GType g_type);
gboolean
ev_module_check_version(EvModule* module,
                        const gchar* expected,
                        GError** error);

/*
 * Initialization function
 *
 */
G_MODULE_EXPORT
gboolean EV_MODULE_INIT_FUNCTION (EvModule       *module,
                                  GCancellable   *cancellable,
                                  GError        **error);

#if __cplusplus
}
#endif // __cplusplus

#endif // __EV2_MODULE_INCLUDED__
