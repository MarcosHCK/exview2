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

/*
 * Reopen
 *
 */
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
                var message = new Ev.Message.error_with_gerror(e);
                ((Gtk.Window)message).set_application(this.application);
                ((Gtk.Dialog)message).run();
                ((Gtk.Widget)message).destroy();
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

/*
 * Preferences dialog
 *
 */

    private Ev.Preferences preferences_dialog = null;

    [GtkCallback]
    private void on_preferences(Gtk.Button button)
    {
      if(unlikely(preferences_dialog == null))
      {
        try
        {
          preferences_dialog = new Ev.Preferences((Gtk.Window)this, null);
        }
        catch(GLib.Error e) {
          var message = new Ev.Message.error_with_gerror(e);
          ((Gtk.Window)message).set_application(this.application);
          ((Gtk.Dialog)message).run();
          ((Gtk.Widget)message).destroy();
          assert_not_reached();
        }

        ((Gtk.Widget)preferences_dialog).
        @delete_event.connect((widget, event) => {
          widget.hide();
        return false;
        });
      }

    /*
     * Run dialog
     *
     */
      ((Gtk.Window)preferences_dialog).present();
    }

/*
 * About dialog
 *
 */
    private Gtk.AboutDialog about_dialog = null;

    [GtkCallback]
    private void on_about(Gtk.Button button)
    {
      if(unlikely(about_dialog == null))
      {
        about_dialog = new Gtk.AboutDialog();
        ((Gtk.Window)about_dialog).set_transient_for((Gtk.Window)this);

      /*
       * Constants
       *
       */
        const string[] artists = {
          "MarcosHCK",
          null,
        };
        const string[] authors = {
          "MarcosHCK",
          null,
        };
        const string[] documenters = {
          "MarcosHCK",
          null,
        };

        const string copyright = "Copyright 2021-2022 MarcosHCK";

      /*
       * Set strings
       *
       */
        about_dialog.title =
        _("About %s").printf(Config.PACKAGE_NAME);

        about_dialog.set_artists(artists);
        about_dialog.set_authors(authors);
        about_dialog.set_copyright(copyright);
        about_dialog.set_documenters(documenters);
        about_dialog.set_license("GNU GPLv3.0");
        about_dialog.set_license_type(Gtk.License.GPL_3_0);
        about_dialog.set_program_name(Config.PACKAGE_NAME);
        about_dialog.set_translator_credits(_("translator-credits"));
        about_dialog.set_version(Config.PACKAGE_VERSION);
        about_dialog.set_website(Config.PACKAGE_URL);
        about_dialog.set_website_label(_("Visit our website"));
        about_dialog.set_wrap_license(true);

        ((Gtk.Widget)about_dialog).
        @delete_event.connect((widget, event) => {
          widget.hide();
        return false;
        });
      }

    /*
     * Run dialog
     *
     */
      ((Gtk.Dialog)about_dialog).run();
      ((Gtk.Widget)about_dialog).hide();
    }

/*
 * Category switch
 *
 */
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

/*
 * Methods
 *
 */
    public void open(GLib.File file, GLib.Cancellable? cancellable = null) throws GLib.Error
    {
      GLib.InputStream stream = null;
      Ev.ViewContext context = null;

    /*
     * Gather arguments
     *
     */
      try {
        stream = file.read(cancellable);
        context = new Ev.ViewContext(2, GLib.Type.STRING, GLib.Type.OBJECT, GLib.Type.NONE);
      } catch(GLib.Error e) {
        throw e;
      }

      var app = (Ev.Application) ((Gtk.Window) this).get_application();
      var manager = (Ev.Parser) (Ev.ModuleManager) app.get_module_manager();

    /*
     * Make the call
     *
     */
      try {
        manager.parse(context, stream, cancellable);
      } catch(GLib.Error e) {
        throw e;
      }

    /*
     * Update GUI and load status
     *
     */
      treeview1.model = context.get_store();
      headerbar1.set_subtitle(file.peek_path());
      loaded = true;
    }
  }
}
