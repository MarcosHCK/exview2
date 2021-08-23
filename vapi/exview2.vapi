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

namespace Ev
{
  [CCode (cheader_filename = "ev2_application.h")]
  public class Application : Gtk.Application
  {
    public unowned Ev.Parser get_module_manager();
  }

  [CCode (cheader_filename = "ev2_parser.h")]
  public interface Parser : GLib.Object
  {
    public virtual bool parse(Ev.ViewContext view_ctx, GLib.InputStream stream, GLib.Cancellable? cancellable = null) throws GLib.Error;
  }
}
