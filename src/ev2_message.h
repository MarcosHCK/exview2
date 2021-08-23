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
#ifndef __EV2_MESSAGE_INCLUDED__
#define __EV2_MESSAGE_INCLUDED__
#include <gtk/gtk.h>

#define EV_TYPE_MESSAGE             (ev_message_get_type())
#define EV_MESSAGE(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), EV_TYPE_MESSAGE, EvMessage))
#define EV_MESSAGE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_MESSAGE, EvMessageClass))
#define EV_IS_MESSAGE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), EV_TYPE_MESSAGE))
#define EV_IS_MESSAGE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), EV_TYPE_MESSAGE))
#define EV_MESSAGE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), EV_TYPE_MESSAGE, EvMessageClass))

typedef struct _EvMessage       EvMessage;
typedef struct _EvMessageClass  EvMessageClass;

#if __cplusplus
extern "C" {
#endif // __cplusplus

GType
ev_message_get_type();

EvMessage*
ev_message_new_question(const gchar* text);
EvMessage*
ev_message_new_message(const gchar* text);
EvMessage*
ev_message_new_warning(const gchar* text);
EvMessage*
ev_message_new_error(const gchar* text);
EvMessage*
ev_message_new_error_with_gerror(GError* error);

#if __cplusplus
}
#endif // __cplusplus

#endif // __EV2_MESSAGE_INCLUDED__
