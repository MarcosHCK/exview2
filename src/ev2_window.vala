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

namespace Ev
{
  [GtkTemplate (ui = "/org/hck/exview2/ui/window.ui")]
  public class Window : Gtk.Window
  {
    [GtkChild]
    private Gtk.HeaderBar headerbar1;
    [GtkChild]
    private Gtk.TreeView treeview1;
    [GtkChild]
    private Gtk.TreeView treeview2;

    private bool loaded = false;

    [GtkCallback]
    private void on_open(Gtk.Button button)
    {
      var chooser = new Gtk.FileChooserDialog
      (_("Open an executable ..."),
       (Gtk.Window)this,
       Gtk.FileChooserAction.OPEN,
       _("Ok"), Gtk.ResponseType.OK,
       _("Cancel"), Gtk.ResponseType.CANCEL,
       null);

      chooser.select_multiple = true;

      var response = ((Gtk.Dialog)chooser).run();
      switch(response)
      {
      case Gtk.ResponseType.OK:
        var uris = ((Gtk.FileChooser)chooser).get_uris();
        if(uris != null)
        {
          for(unowned var list = uris;list != null;list = list.next)
          {
            var app = this.application;
            var file = GLib.File.new_for_uri((string) list.data);
            if(unlikely(loaded == false))
            {
              try
              {
                this.open(file, null);
              } catch(GLib.Error e)
              {
                critical(e.message);
                assert_not_reached();
              }
            }
            else
            {
              GLib.File files[] = {file,};
              app.open(files, "open");
            }
          }
        }
        break;
      }

      ((Gtk.Widget)chooser).destroy();
    }

    [GtkCallback]
    private void on_cursor_changed(Gtk.TreeView treeview)
    {
      Gtk.TreeModel model = treeview.get_model();
      if(model != null)
      {
        Gtk.TreeStore store = (Gtk.TreeStore)model;
        Gtk.TreePath path = null;
        Gtk.TreeIter iter;

        treeview.get_cursor(out path, null);
        if(path != null)
        {
          model.get_iter(out iter, path);
          if(store.iter_is_valid(iter))
          {
            Gtk.TreeStore store_ = null;
            model.@get(iter, 1, out store_);
            treeview2.model = store_;
            treeview2.expand_all();
          }
          else
          {
            treeview2.model = null;
          }

          path.free();
        }
        else
        {
          treeview2.model = null;
        }
      }
    }

    [GtkCallback]
    private bool on_delete_event(Gtk.Widget widget, Gdk.EventAny event)
    {
      return false;
    }

    public void open(GLib.File file, GLib.Cancellable? cancellable = null) throws GLib.Error
    {
      var stream = file.read(cancellable);
      

      headerbar1.set_subtitle(file.peek_path());
      loaded = true;
    }
  }
}
