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
#ifndef __EV2_VIEW_CONTEXT_PATCH_INCLUDED__
#define __EV2_VIEW_CONTEXT_PATCH_INCLUDED__
#include <gtk/gtk.h>

#if __cplusplus
extern "C" {
#endif // __cplusplus

GtkTreeStore*
ev_view_context_new_categories_valist(int n_columns,
                                      va_list l);

#if __cplusplus
}
#endif // __cplusplus

#endif // __EV2_VIEW_CONTEXT_PATCH_INCLUDED__
