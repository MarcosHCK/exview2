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
#include <ev2_macros.h>
#include <elf_structs.h>
#include <glib-object.h>

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

#if __cplusplus
}
#endif // __cplusplus

#endif // __ELF_PARSER_INCLUDED__
