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
#ifndef __EV2_SETTINGS_INCLUDED__
#define __EV2_SETTINGS_INCLUDED__
#include <glib-object.h>

#define EV_TYPE_SETTINGS            (ev_settings_get_type ())
#define EV_SETTINGS(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), EV_TYPE_SETTINGS, EvSettings))
#define EV_SETTINGS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), EV_TYPE_SETTINGS, EvSettingsClass))
#define EV_IS_SETTINGS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), EV_TYPE_SETTINGS))
#define EV_IS_SETTINGS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), EV_TYPE_SETTINGS))
#define EV_SETTINGS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), EV_TYPE_SETTINGS, EvSettingsClass))

typedef struct _EvSettings      EvSettings;
typedef struct _EvSettingsClass EvSettingsClass;

#if __cplusplus
extern "C" {
#endif // __cplusplus

GType
ev_settings_get_type();

EvSettings*
ev_settings_new (GCancellable* cancellable,
                 GError** error);
GSettings*
ev_settings_get_settings (EvSettings* self,
                          const gchar* schema_id);

#if __cplusplus
}
#endif // __cplusplus

#endif // __EV2_SETTINGS_INCLUDED__
