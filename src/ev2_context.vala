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
  public class ViewContext : GLib.Object
  {
    private Gtk.TreeStore categories = null;
    private GLib.Queue<Gtk.TreeIter?> iters = null;

    public void append(string name, Gtk.TreeStore contents)
    {
      Gtk.TreeIter iter;
      categories.append(out iter, iters.peek_head());
      categories.@set(iter, 0, name, 1, (GLib.Object) contents);
    }

    public void prepend(string name, Gtk.TreeStore contents)
    {
      Gtk.TreeIter iter;
      categories.prepend(out iter, iters.peek_head());
      categories.@set(iter, 0, name, 1, (GLib.Object) contents);
    }

    public void descend(string name)
    {
      Gtk.TreeIter iter;
      categories.append(out iter, iters.peek_head());
      categories.@set(iter, 0, name);
      iters.push_head(iter);
    }

    public void ascend()
    {
      var n = iters.get_length();
      if(likely(n > 1))
        iters.pop_head();
      else
        warning("Attempt to ascend over base\r\n");
    }

    public ViewContext()
    {
      Object();

/*
 * Categories store
 *
 */
      this.categories =
      new Gtk.TreeStore
      (2,
       GLib.Type.STRING,
       GLib.Type.OBJECT,
       GLib.Type.NONE);

/*
 * Subcategories iterator
 * stack
 *
 */
      this.iters =
      new GLib.Queue<Gtk.TreeIter?>();
      this.iters.push_head(null);
    }

    public ViewContext.ascending(Ev.ViewContext parent, string name)
    {
      Object();

/*
 * Initialize objects
 *
 */
      this.iters =
      new GLib.Queue<Gtk.TreeIter?>();
      this.categories = parent.categories;

/*
 * Append 
 *
 */
      Gtk.TreeIter iter;
      parent.categories.append(out iter, iters.peek_head());
      parent.categories.@set(iter, 0, name);
      this.iters.push_head(iter);
    }
  }
}
