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
#ifndef __EV2_PARSER_INCLUDED__
#define __EV2_PARSER_INCLUDED__ 1
#include <ev2_view_context.h>
#include <gio/gio.h>

/**
 * EV_PARSER_ERROR:
 *
 * Error domain for EV_PARSER. Errors in this domain will be from the #EvParserError enumeration.
 * See #GError for more information on error domains.
 */
#define EV_PARSER_ERROR (ev_parser_error_quark())

/**
 * EvModuleError:
 * @EV_PARSER_ERROR_FAILED: generic error condition.
 * @EV_PARSER_ERROR_UNPARSEABLE: input is unparseable.
 *
 * Error code returned by #EvParser API functions.
 * Note that %EV_PARSER_ERROR_FAILED is here only for compatibility with
 * error domain definition paradigm as defined on GLib documentation.
 */
typedef enum {
  EV_PARSER_ERROR_FAILED,
  EV_PARSER_ERROR_UNPARSEABLE,
} EvParserError;

#define EV_TYPE_PARSER            (ev_parser_get_type())
#define EV_PARSER(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), EV_TYPE_PARSER, EvParser))
#define EV_PARSER_CLASS(obj)      (G_TYPE_CHECK_CLASS_CAST((obj), EV_TYPE_PARSER, EvParserIface))
#define EV_IS_PARSER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), EV_TYPE_PARSER))
#define EV_PARSER_GET_IFACE(obj)  (G_TYPE_INSTANCE_GET_INTERFACE((obj), EV_TYPE_PARSER, EvParserIface))

typedef struct _EvParser      EvParser;
typedef struct _EvParserIface EvParserIface;

#if __cplusplus
extern "C" {
#endif // __cplusplus

GType
ev_parser_get_type();
GQuark
ev_parser_error_quark();

struct _EvParserIface
{
  GTypeInterface parent_iface;

  gboolean (*parse) (EvParser* parser, EvViewContext* view_ctx, GInputStream* stream, GCancellable* cancellable, GError** error);
};

gboolean
ev_parser_parse(EvParser* parser,
                EvViewContext* view_ctx,
                GInputStream* stream,
                GCancellable* cancellable,
                GError** error);

#if __cplusplus
}
#endif // __cplusplus

#endif // __EV2_PARSER_INCLUDED__
