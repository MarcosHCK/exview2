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
#ifndef __EV2_VIEW_CONTEXT_INCLUDED__
#define __EV2_VIEW_CONTEXT_INCLUDED__
#include <glib-object.h>

#define EV_TYPE_VIEW_CONTEXT            (ev_view_context_get_type())
#define EV_VIEW_CONTEXT(object)         (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_VIEW_CONTEXT, EvViewContext))
#define EV_VIEW_CONTEXT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_VIEW_CONTEXT, EvViewContextClass))
#define EV_IS_VIEW_CONTEXT(object)      (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_VIEW_CONTEXT))
#define EV_IS_VIEW_CONTEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), EV_TYPE_VIEW_CONTEXT))
#define EV_VIEW_CONTEXT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), EV_TYPE_VIEW_CONTEXT, EvViewContextClass))

typedef struct _EvViewContext       EvViewContext;
typedef struct _EvViewContextClass  EvViewContextClass;

#if __cplusplus
extern "C" {
#endif // __cplusplus

GType
ev_view_context_get_type();

EvViewContext*
ev_view_context_new ();
EvViewContext*
ev_view_context_new_ascending (EvViewContext* parent,
                               const gchar* name);
void
ev_view_context_append (EvViewContext* self,
                        const gchar* name,
                        GObject* contents);
void
ev_view_context_prepend (EvViewContext* self,
                         const gchar* name,
                         GObject* contents);
void
ev_view_context_descend (EvViewContext* self,
                         const gchar* name);
void
ev_view_context_ascend (EvViewContext* self);

#if __cplusplus
}
#endif // __cplusplus

#endif // __EV2_VIEW_CONTEXT_INCLUDED__
