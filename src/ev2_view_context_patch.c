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
#include <config.h>
#include <ev2_view_context_patch.h>

GtkTreeStore*
ev_view_context_new_categories_valist(int n_columns,
                                      va_list l)
{
  g_assert(sizeof(GType) * n_columns < 1024);
  GType* g_types = g_alloca(sizeof(GType) * n_columns);
  gint i;

  for(i = 0;i < n_columns;i++)
    g_types[i] = va_arg(l, GType);

  return
  gtk_tree_store_newv(n_columns, g_types);
}
