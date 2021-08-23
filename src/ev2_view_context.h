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
ev_view_context_new (gint n_columns,
                     ...);
EvViewContext*
ev_view_context_new_ascending (EvViewContext* parent,
                               ...);
void
ev_view_context_append (EvViewContext* self,
                        ...);
void
ev_view_context_appendv (EvViewContext* self,
                         va_list l);
void
ev_view_context_prepend (EvViewContext* self,
                         ...);
void
ev_view_context_prependv (EvViewContext* self,
                          va_list l);
void
ev_view_context_descend (EvViewContext* self,
                         ...);
void
ev_view_context_descendv (EvViewContext* self,
                          va_list l);
void
ev_view_context_ascend (EvViewContext* self);
gpointer
ev_view_context_get_store (EvViewContext* self);

/**
 * page_builder_new:
 *
 * Defines necessary variables needed
 * for page building. Note that this macro
 * needs to be call only once per scope (like functions),
 * and generally at the top (before any other page building
 * macro).
 */
#define page_builder_new() \
\
EvViewContext* page_r = NULL; \
GString* page_p = g_string_sized_new(0); \
GString* page_v = g_string_sized_new(0); \
gpointer page_pb = NULL;

/**
 * page_builder_begin:
 * @view_ctx: view context.
 * @title: page title.
 * @expand: whether page should be
 * appended as a expander.
 *
 * Starts page building process. Appends an entry
 * on @view_ctx with title @title (column #0).
 */
#define page_builder_begin(view_ctx, title, expand) \
\
G_STMT_START { \
  EvViewContext* context = ev_view_context_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_OBJECT); \
  g_set_object(&page_r, context); \
  g_object_unref(context); \
  if(((expand)) == TRUE) \
    ev_view_context_descend \
    ((view_ctx), \
     0, (title), \
     1, ev_view_context_get_store(page_r), \
     -1); \
  else \
    ev_view_context_append \
    ((view_ctx), \
     0, (title), \
     1, ev_view_context_get_store(page_r), \
     -1); \
} G_STMT_END

/**
 * page_builder_end:
 *
 * Stops page building process. Note that this macro
 * is intended to do it explicitly, because  #page_builder_begin
 * stops ongoing process before start a new one.
 */
#define page_builder_end() \
\
G_STMT_START { \
  g_clear_object(&page_r); \
  g_string_free(page_p, TRUE); \
  g_string_free(page_v, TRUE); \
  g_clear_object(&page_pb); \
} G_STMT_END

/**
 * page_builder_printp:
 *
 * Sets next row property value (most likely it's title).
 */
#define page_builder_printp(...) \
\
G_STMT_START { \
  g_string_printf(page_p, __VA_ARGS__); \
} G_STMT_END

/**
 * page_builder_printv:
 *
 * Sets next row value value (most likely it's content).
 */
#define page_builder_printv(...) \
\
G_STMT_START { \
  g_string_printf(page_v, __VA_ARGS__); \
} G_STMT_END

/**
 * page_builder_printpb:
 *
 * Sets next row pixbuf value (most likely it's content).
 * Note that set both value and pixbuf is usually a bad idea
 * and is considered a programming error (therefore no check will
 * be performed to avoid it).
 */
#define page_builder_printpb(pixbuf) \
\
G_STMT_START { \
  g_set_object(&page_pb, pixbuf); \
} G_STMT_END

/**
 * page_builder_setrow:
 *
 * Appends a new row to current on building page.
 * You should call #page_builder_printp, #page_builder_printv
 * and #page_builder_printpb before this, since property and
 * values are set with them.
 */
#define page_builder_setrow(expand) \
\
G_STMT_START { \
  if(page_p->len > 0) \
  { \
    if(((expand)) == TRUE) \
      ev_view_context_descend \
      (page_r, \
       0, page_p->str, \
       1, page_v->str, \
       2, page_pb, \
       -1); \
    else \
      ev_view_context_append \
      (page_r, \
       0, page_p->str, \
       1, page_v->str, \
       2, page_pb, \
       -1); \
\
     g_string_truncate(page_p, 0); \
     g_string_truncate(page_v, 0); \
     g_clear_object(&page_pb); \
  } \
} G_STMT_END

/**
 * page_builder_closerow:
 *
 * Closes an previously builded expander row.
 */
#define page_builder_closerow() \
\
G_STMT_START { \
  ev_view_context_ascend(page_r); \
} G_STMT_END

#if __cplusplus
}
#endif // __cplusplus

#endif // __EV2_VIEW_CONTEXT_INCLUDED__
