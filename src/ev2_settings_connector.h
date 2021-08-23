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
#ifndef __EV2_SETTINGS_CONNECTOR_INCLUDED__
#define __EV2_SETTINGS_CONNECTOR_INCLUDED__
#include <glib-object.h>

/**
 * EV_SETTINGS_CONNECTOR_ERROR:
 *
 * Error domain for EV_SETTINGS_CONNECTOR. Errors in this domain will be from the #EvSettingsConnectorError enumeration.
 * See #GError for more information on error domains.
 */
#define EV_SETTINGS_CONNECTOR_ERROR (ev_settings_connector_error_quark())

/**
 * EvModuleError:
 * @EV_SETTINGS_CONNECTOR_ERROR_FAILED: generic error condition.
 * @EV_SETTINGS_CONNECTOR_ERROR_UNKOWN_SCHEMA: self explanatory.
 * @EV_SETTINGS_CONNECTOR_ERROR_UNKOWN_KEY: self explanatory.
 *
 * Error code returned by #EvSettingsConnector API functions.
 * Note that %EV_SETTINGS_CONNECTOR_ERROR_FAILED is here only for compatibility with
 * error domain definition paradigm as defined on GLib documentation.
 */
typedef enum {
  EV_SETTINGS_CONNECTOR_ERROR_FAILED,
  EV_SETTINGS_CONNECTOR_ERROR_UNKOWN_SCHEMA,
  EV_SETTINGS_CONNECTOR_ERROR_UNKOWN_KEY,
} EvSettingsConnectorError;

#define EV_TYPE_SETTINGS_CONNECTOR            (ev_settings_connector_get_type ())
#define EV_SETTINGS_CONNECTOR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), EV_TYPE_SETTINGS_CONNECTOR, EvSettingsConnector))
#define EV_SETTINGS_CONNECTOR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), EV_TYPE_SETTINGS_CONNECTOR, EvSettingsConnectorClass))
#define EV_IS_SETTINGS_CONNECTOR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), EV_TYPE_SETTINGS_CONNECTOR))
#define EV_IS_SETTINGS_CONNECTOR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), EV_TYPE_SETTINGS_CONNECTOR))
#define EV_SETTINGS_CONNECTOR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), EV_TYPE_SETTINGS_CONNECTOR, EvSettingsConnectorClass))

typedef struct _EvSettingsConnector      EvSettingsConnector;
typedef struct _EvSettingsConnectorClass EvSettingsConnectorClass;

#if __cplusplus
extern "C" {
#endif // __cplusplus

GType
ev_settings_connector_get_type();
GQuark
ev_settings_connector_error_quark();

struct _EvSettingsConnectorClass
{
  GObjectClass parent_class;
};

EvSettingsConnector*
ev_settings_connector_new();

#if __cplusplus
}
#endif // __cplusplus

#endif // __EV2_SETTINGS_CONNECTOR_INCLUDED__
