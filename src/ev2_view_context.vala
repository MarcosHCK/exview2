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
  public class ViewContext : GLib.Object
  {
    private Gtk.TreeStore categories = null;
    private GLib.Queue<Gtk.TreeIter?> iters = null;

    [CCode (sentinel = "-1")]
    public void append(...)
    {
      var l = va_list();
      this.appendv(l);
    }

    public void appendv(va_list l)
    {
      Gtk.TreeIter iter;
      categories.append(out iter, iters.peek_head());
      categories.set_valist(iter, l);
    }

    [CCode (sentinel = "-1")]
    public void prepend(...)
    {
      var l = va_list();
      this.prependv(l);
    }

    public void prependv(va_list l)
    {
      Gtk.TreeIter iter;
      categories.prepend(out iter, iters.peek_head());
      categories.set_valist(iter, l);
    }

    [CCode (sentinel = "-1")]
    public void descend(...)
    {
      var l = va_list();
      this.descendv(l);
    }

    public void descendv(va_list l)
    {
      Gtk.TreeIter iter;
      categories.append(out iter, iters.peek_head());
      categories.set_valist(iter, l);
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

    public Gtk.TreeModel get_store()
    {
      return (Gtk.TreeModel) this.categories;
    }

    [CCode (sentinel = "G_TYPE_NONE")]
    public ViewContext(int n_columns, ...)
    {
      Object();
      var l = va_list();

/*
 * Categories store
 *
 */
      this.categories = Ev.new_categories_valist(n_columns, l);

/*
 * Subcategories iterator
 * stack
 *
 */
      this.iters =
      new GLib.Queue<Gtk.TreeIter?>();
      this.iters.push_head(null);
    }

    [CCode (sentinel = "-1")]
    public ViewContext.ascending(Ev.ViewContext parent, ...)
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
 * Arguments
 * 
 */
      var l = va_list();

/*
 * Append 
 *
 */
      Gtk.TreeIter iter;
      parent.categories.append(out iter, iters.peek_head());
      parent.categories.set_valist(iter, l);
      this.iters.push_head(iter);
    }
  }
}
