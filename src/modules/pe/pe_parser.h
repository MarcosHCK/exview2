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
  pe_bitlen_t bitlen;
  pe_dos_header_t dos;
  pe_header_t pe;
  union
  {
    pe_optional_header32_t _32;
    pe_optional_header64_t _64;
  } opt;

  goffset sections;
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
gboolean
_pe_parser_parse_pe_header(EvPeParser* self,
                           EvViewContext* view_ctx,
                           GInputStream* stream,
                           GCancellable* cancellable,
                           GError** error);
gboolean
_pe_parser_parse_opt_header(EvPeParser* self,
                            EvViewContext* view_ctx,
                            GInputStream* stream,
                            GCancellable* cancellable,
                            GError** error);
gboolean
_pe_parser_parse_sections(EvPeParser* self,
                          EvViewContext* view_ctx,
                          GInputStream* stream,
                          GCancellable* cancellable,
                          GError** error);
gboolean
_pe_parser_parse_imports(EvPeParser* self,
                         EvViewContext* view_ctx,
                         GInputStream* stream,
                         GCancellable* cancellable,
                         GError** error);
gboolean
_pe_parser_parse_exports(EvPeParser* self,
                         EvViewContext* view_ctx,
                         GInputStream* stream,
                         GCancellable* cancellable,
                         GError** error);

/*
 * Human-readable conversion functions
 *
 */
/*<private>*/
const gchar* _s_pever(uint16_t optmagic);
const gchar* _s_pearch(pe_arch_t arch);
const gchar* _s_petimestamp(uint32_t timestamp);
gchar* _s_pechars(pe_flag_t characteristics);
const gchar* _s_pesubsystem(pe_subsystem_t subsystem);
gchar* _s_pedllchars(pe_dllflags_t flags);
const gchar* _s_peddirname(int n);
const gchar* _s_pesectchars(pe_section_flags_t characteristics);
const gchar* _s_peresid(pe_resource_id_t id);
gchar* _s_pefileveros(pe_filever_os_t type);
const gchar* _s_pefilevertype(pe_filever_type_t type);

/*
 * Utilities
 *
 */
/*<private>*/
pe_section_header_t*
_pe_parser_get_section_by_vaddr(EvPeParser* self,
                                vaddr_t vaddr,
                                GInputStream* stream,
                                GCancellable* cancellable,
                                GError** error);
goffset
_pe_parser_translate_vaddr(EvPeParser* self,
                           vaddr_t vaddr,
                           GInputStream* stream,
                           GCancellable* cancellable,
                           GError** error);
gchar*
_pe_parser_load_string(EvPeParser* self,
                       goffset at_,
                       gssize size_,
                       gsize* psize,
                       GInputStream* stream,
                       GCancellable* cancellable,
                       GError** error);

#if __cplusplus
}
#endif // __cplusplus

#endif // __PE_PARSER_INCLUDED__
