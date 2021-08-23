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
  errordomain PreferencesError
  {
    FAILED,
    MISSING_TOPLEVEL,
    MULTIPLE_TOPLEVEL,
  }

  [GtkTemplate (ui = "/org/hck/exview2/ui/preferences.ui")]
  public class Preferences : Gtk.Window, GLib.Initable
  {
    [GtkChild]
    private Gtk.Box box1;

    private Gtk.Widget get_toplevel_(Gtk.Builder builder) throws GLib.Error
    {
      Gtk.Widget toplevel = null;

      var object_ = builder.get_object("toplevel");
      if(likely(object_ != null))
        toplevel = object_ as Gtk.Widget;
      else
      {
        /*
         * alternative search (brute force)
         *
         */
        var children = builder.get_objects();
        foreach(var child in children)
        {
          if(child is Gtk.Widget)
          {
            var widget = child as Gtk.Widget;
            if(unlikely(widget.get_toplevel() == null))
            {
              if(likely(toplevel == null))
                toplevel = widget;
              else
                throw new PreferencesError.MULTIPLE_TOPLEVEL("Multiple toplevel widget\r\n");
            }
          }
        }
      }

      if(unlikely(toplevel == null))
        throw new PreferencesError.MISSING_TOPLEVEL("Missing toplevel widget\r\n");
    return toplevel;
    }

    private bool init(GLib.Cancellable? cancellable = null) throws GLib.Error
    {
      var app = (Ev.Application) GLib.Application.get_default();
      var manager = (Ev.ModuleManager) app.get_module_manager();
      var snippets = manager.list_snippets();

      foreach(var snippet in snippets)
      {
        Gtk.Builder builder = new Gtk.Builder();

        try
        {
          builder.add_from_string
          ((string) snippet.get_data(),
           (size_t) snippet.get_size());
        }
        catch(GLib.Error e) {
          throw e;
        }

        try
        {
          var toplevel = get_toplevel_(builder);
          ((Gtk.Container) this.box1).add(toplevel);
          ((Gtk.Widget) this.box1).show_all();
        }
        catch(GLib.Error e) {
          throw e;
        }
      }
    return true;
    }

    public Preferences(Gtk.Window transient, GLib.Cancellable? cancellable = null) throws GLib.Error
    {
      Object();
      ((Gtk.Window)this).transient_for = transient;
      this.init(cancellable);
    }
  }
}
