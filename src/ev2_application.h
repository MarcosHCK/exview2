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
#ifndef __EV2_APPLICATION_INCLUDED__
#define __EV2_APPLICATION_INCLUDED__ 1
#include <gtk/gtk.h>

#define EV_TYPE_APPLICATION             (ev_application_get_type())
#define EV_APPLICATION(object)          (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_APPLICATION, EvApplication))
#define EV_APPLICATION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_APPLICATION, EvApplicationClass))
#define EV_IS_APPLICATION(object)       (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_APPLICATION))
#define EV_IS_APPLICATION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), EV_TYPE_APPLICATION))
#define EV_APPLICATION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), EV_TYPE_APPLICATION, EvApplicationClass))

typedef struct _EvApplication       EvApplication;
typedef struct _EvApplicationClass  EvApplicationClass;

#if __cplusplus
extern "C" {
#endif // __cplusplus

GType
ev_application_get_type();

struct _EvApplicationClass
{
  GtkApplicationClass parent_class;
};

gpointer
ev_application_get_module_manager(EvApplication* application);

#if __cplusplus
}
#endif // __cplusplus

#endif // __EV2_APPLICATION_INCLUDED__
