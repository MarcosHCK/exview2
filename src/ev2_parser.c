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
#include <ev2_parser.h>
#define EvParserInterface EvParserIface

G_DEFINE_QUARK(ev-parser-error-quark,
               ev_parser_error);

/*
 * Definition
 *
 */

G_DEFINE_INTERFACE
(EvParser,
 ev_parser,
 G_TYPE_OBJECT);

static
gboolean ev_parser_iface_parse(EvParser* parser, EvViewContext* view_ctx, GInputStream* stream, GCancellable* cancellable, GError** error) {
  g_set_error
  (error,
   EV_PARSER_ERROR,
   EV_PARSER_ERROR_FAILED,
   "EvParser::parse not implemented for '%s'",
   g_type_name(G_TYPE_FROM_INSTANCE(parser)));
  g_warning
  ("EvParser::parse not implemented for '%s'",
   g_type_name(G_TYPE_FROM_INSTANCE(parser)));
return FALSE;
}

static
void ev_parser_default_init(EvParserIface* iface) {
  iface->parse = ev_parser_iface_parse;
}

/*
 * Methods
 *
 */

gboolean
ev_parser_parse(EvParser* parser,
                EvViewContext* view_ctx,
                GInputStream* stream,
                GCancellable* cancellable,
                GError** error)
{
  g_return_val_if_fail(EV_IS_PARSER(parser), FALSE);
  g_return_val_if_fail(G_IS_INPUT_STREAM(stream), FALSE);
  g_return_val_if_fail(error == NULL || *error == NULL, FALSE);

  EvParserIface* iface = EV_PARSER_GET_IFACE(parser);
return iface->parse(parser, view_ctx, stream, cancellable, error);
}
