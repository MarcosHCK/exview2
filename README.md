# Exview2

Exview2 is a lightweight and easy to use file viewer. I known there are a lot
of this out there, but Exview2 could unite all of the on a single application.
Of curse, Exview2 could be extended by modules (plug-ins), to allow it to view
a great deal of formats.

---

### How to use Exview2

Exview2 comes with a two panel GUI layout, with the leftmost panel representing
contents categories, which could be selected to show its contents on the
other panel. These "categories" represent remarkable things on the open file.
For example, in a ELF file, categories would be things like program headers,
sections, symbols, etc.

---

### How to extended Exview2

Exview2 can be extended using a plugin system. Plugins could be install
separately from the main program an added on demand to meet the needs of
each user.

If you want to develop your own plugin, you should know Exview2 allows you to:

 * Install GSettings schemas, which carries a long list of advantages.
 * Register UI definitions (using GtkBuilder XML definitions) to be shown
   at Exview2 preferences GUI. Here you could put some features you want to make
   optional, or any generic settings your plugin needs. Note that Exview2 comes
   with a custom object class, EvSettingsConnector, which allows a code-less
   GSettings binding.
 * Multiple formats inside one plugin, so you could ship a parser for both
   PE32 and PE32+ executable formats in the same plugin.

Now, for independent developers:

 * GSettings schemas installs usually at /usr/share/exview2/schemas or
   /usr/local/share/exview2/schemas.
 * UI definitions are shipped along with plugin executable, and register
   using an API interface.

If you want to add your plugin to the source tree:

 * GSettings schemas goes on [settings](https://github.com/MarcosHCK/exview2/tree/master/settings).
 * Your module code goes on [modules](https://github.com/MarcosHCK/exview2/tree/master/src/modules),
   inside a folder named after your plug-in.
 * UI definitions have not a defined way to compile, but you could copy them on
   [resources](https://github.com/MarcosHCK/exview2/tree/master/src/resources/ui), and do the common
   preparation (you know how GResource works).

---

### Homepage

I am still working on that

### Changelog

See [NEWS](https://github.com/MarcosHCK/exview2/blob/master/NEWS) for details on changes and fixes made in the current release.
