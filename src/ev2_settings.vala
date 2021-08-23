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
  public class Settings : GLib.Object, GLib.Initable
  {
    private GLib.SettingsSchemaSource source = null;
    private GLib.HashTable<string,GLib.SettingsSchema> schemas =
    new GLib.HashTable<string, GLib.SettingsSchema>(GLib.str_hash, GLib.str_equal);

    public bool init(GLib.Cancellable? cancellable = null) throws GLib.Error
    {
      try
      {
        var debug = GLib.Environment.get_variable("EV_DEBUG");
        if(debug != null && @bool.parse(debug))
          source = new GLib.SettingsSchemaSource.from_directory(
            Config.ABSTOPBUILDDIR + "/settings/",
            GLib.SettingsSchemaSource.get_default(),
            true);
        else
          source = new GLib.SettingsSchemaSource.from_directory(
            Config.SCHEMASDIR,
            GLib.SettingsSchemaSource.get_default(),
            true);
      }
      catch(GLib.Error e)
      {
        throw e;
      }
    return true;
    }

    public GLib.Settings? get_settings(string schema_id)
    {
      var schema = schemas.lookup(schema_id);
      if(unlikely(schema == null))
      {
        schema = source.lookup(schema_id, true);
        if(unlikely(schema == null))
          return null;

        schemas.insert(schema_id, schema);
        return this.get_settings(schema_id);
      }
    return new GLib.Settings.full(schema, null, null);
    }

    public Settings(GLib.Cancellable? cancellable = null) throws GLib.Error
    {
      Object();

      try {
        this.init(cancellable);
      } catch(GLib.Error e) {
        throw e;
      }
    }
  }
}
