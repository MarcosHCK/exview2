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
#ifndef __PE_PARSER_INCLUDED__
#define __PE_PARSER_INCLUDED__ 1
#include <ev2_macros.h>
#include <ev2_parser.h>
#include <ev2_view_context.h>
#include <pe_structs.h>

#define EV_TYPE_PE_PARSER            (ev_pe_parser_get_type())
#define EV_PE_PARSER(object)         (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_PE_PARSER, EvPeParser))
#define EV_PE_PARSER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_PE_PARSER, EvPeParserClass))
#define EV_IS_PE_PARSER(object)      (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_PE_PARSER))
#define EV_IS_PE_PARSER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), EV_TYPE_PE_PARSER))
#define EV_PE_PARSER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), EV_TYPE_PE_PARSER, EvPeParserClass))

typedef struct _EvPeParser      EvPeParser;
typedef struct _EvPeParserClass EvPeParserClass;

#if __cplusplus
extern "C" {
#endif // __cplusplus

GType
ev_pe_parser_get_type();

struct _EvPeParser
{
  GObject parent_instance;

  /*<private>*/
  pe_dos_header_t dos;
  pe_header_t pe;
  union
  {
    pe_optional_header32_t _32;
    pe_optional_header64_t _64;
  } opt;

  pe_section_header_t* sects;
};


struct _EvPeParserClass
{
  GObjectClass parent_class;
};

/*
 * Parsers
 *
 */
/*<private>*/
gboolean
_pe_parser_parse_dos_header(EvPeParser* self,
                            EvViewContext* view_ctx,
                            GInputStream* stream,
                            GCancellable* cancellable,
                            GError** error);

/*
 * Human-readable conversion functions
 *
 */
/*<private>*/

/*
 * Utilities
 *
 */
/*<private>*/

#if __cplusplus
}
#endif // __cplusplus

#endif // __PE_PARSER_INCLUDED__
