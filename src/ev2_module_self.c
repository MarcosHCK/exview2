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
#include <ev2_module.h>

G_MODULE_EXPORT
gboolean EV_MODULE_INIT_FUNCTION (EvModule       *module,
                                  GCancellable   *cancellable,
                                  GError        **error)
{
  GError* tmp_err = NULL;
  ev_module_check_version(module, PACKAGE_VERSION, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    return FALSE;
  }

  GBytes* snippet =
  g_resources_lookup_data(GRESNAME "/ui/prefblock.ui", 0, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_propagate_error(error, tmp_err);
    return FALSE;
  }

  ev_module_set_name(module, "[exview2] Self module");
  ev_module_set_snippet(module, snippet);
  g_bytes_unref(snippet);
return TRUE;
}
