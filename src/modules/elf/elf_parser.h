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
#ifndef __ELF_PARSER_INCLUDED__
#define __ELF_PARSER_INCLUDED__ 1
#include <elf_structs.h>
#include <ev2_macros.h>
#include <ev2_parser.h>
#include <ev2_view_context.h>

#define EV_TYPE_ELF_PARSER            (ev_elf_parser_get_type())
#define EV_ELF_PARSER(object)         (G_TYPE_CHECK_INSTANCE_CAST((object), EV_TYPE_ELF_PARSER, EvElfParser))
#define EV_ELF_PARSER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), EV_TYPE_ELF_PARSER, EvElfParserClass))
#define EV_IS_ELF_PARSER(object)      (G_TYPE_CHECK_INSTANCE_TYPE((object), EV_TYPE_ELF_PARSER))
#define EV_IS_ELF_PARSER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), EV_TYPE_ELF_PARSER))
#define EV_ELF_PARSER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), EV_TYPE_ELF_PARSER, EvElfParserClass))

typedef struct _EvElfParser      EvElfParser;
typedef struct _EvElfParserClass EvElfParserClass;

#if __cplusplus
extern "C" {
#endif // __cplusplus

GType
ev_elf_parser_get_type();

struct _EvElfParser
{
  GObject parent_instance;

  /*<private>*/
  union
  {
    elf_header32_t header32_;
    elf_header64_t header64_;
    gchar header_b;
  };
};


struct _EvElfParserClass
{
  GObjectClass parent_class;
};

/*
 * Parsers
 *
 */
/*<private>*/
gboolean
_elf_parser_parse_header(EvElfParser* pself,
                         EvViewContext* view_ctx,
                         GInputStream* stream,
                         GCancellable* cancellable,
                         GError** error);
gboolean
_elf_parser_parse_phents(EvElfParser* pself,
                         EvViewContext* view_ctx,
                         GInputStream* stream,
                         GCancellable* cancellable,
                         GError** error);
gboolean
_elf_parser_parse_shents(EvElfParser* pself,
                         EvViewContext* view_ctx,
                         GInputStream* stream,
                         GCancellable* cancellable,
                         GError** error);
gboolean
_elf_parser_parse_dyninfo(EvElfParser* self,
                          EvViewContext* view_ctx,
                          GInputStream* stream,
                          GCancellable* cancellable,
                          GError** error);
gboolean
_elf_parser_parse_symbols(EvElfParser* self,
                          EvViewContext* view_ctx,
                          GInputStream* stream,
                          GCancellable* cancellable,
                          GError** error);

/*
 * Human-readable conversion functions
 *
 */
/*<private>*/
const gchar*
_s_elfabi(elf_abi_t abi);
const gchar*
_s_elftype(elf_type_t abi);
const gchar*
_s_elfmach(elf_machine_t type);
const gchar*
_s_elfphtype(elf_phtype_t type);
const gchar*
_s_elfshtype(elf_shtype_t type);
const gchar*
_s_elfsbind(elf_bind_type_t type);
const gchar*
_s_elfstype(elf_sym_type_t type);
const gchar*
_s_elfsvis(elf_sym_vis_t type);
const gchar*
_s_elfdynt(elf_dyn_type_t type);

/*
 * Utilities
 *
 */
/*<private>*/
gboolean
_elf_parser_get_segment_entry(EvElfParser* self,
                              guint32 idx,
                              elf_phent64_t* ent,
                              GInputStream* stream,
                              GCancellable* cancellable,
                              GError** error);
gboolean
_elf_parser_get_section_entry(EvElfParser* self,
                              guint32 idx,
                              elf_shent64_t* ent,
                              GInputStream* stream,
                              GCancellable* cancellable,
                              GError** error);
gchar*
_elf_parser_get_string(EvElfParser* self,
                       guint32 sh_name,
                       GInputStream* stream,
                       GCancellable* cancellable,
                       GError** error);
goffset
_elf_parser_translate_addr(EvElfParser* self,
                           vaddr_t addr,
                           GInputStream* stream,
                           GCancellable* cancellable,
                           GError** error);

#if __cplusplus
}
#endif // __cplusplus

#endif // __ELF_PARSER_INCLUDED__
