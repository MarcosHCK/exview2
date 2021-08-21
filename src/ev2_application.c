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
#include <ev2_application.h>
#include <resources/ev_resources.h>

typedef struct _EvWindow EvWindow;

EvWindow*
ev_window_new();
void
ev_window_open(EvWindow      *window,
               GFile         *file,
               GCancellable  *cancellable,
               GError       **error);

/*
 * Object definition
 *
 */

struct _EvApplication {
  GtkApplication parent_instance;

  /*<private>*/
};

G_DEFINE_TYPE
(EvApplication,
 ev_application,
 GTK_TYPE_APPLICATION);

static
void ev_application_class_activate(GApplication* pself) {
  EvWindow* window = ev_window_new();
  gtk_window_set_application(GTK_WINDOW(window), GTK_APPLICATION(pself));
  gtk_window_present(GTK_WINDOW(window));
}

static
void ev_application_class_open(GApplication* pself, GFile** files, gint n_files, const gchar* hint) {
  gint i;
  for(i = 0;i < n_files;i++)
  {
    EvWindow* window = ev_window_new();
    gtk_window_set_application(GTK_WINDOW(window), GTK_APPLICATION(pself));
    gtk_window_present(GTK_WINDOW(window));

    GError* tmp_err = NULL;
    ev_window_open(window, files[i], NULL, &tmp_err);
    if G_UNLIKELY(tmp_err != NULL)
    {
      g_critical(tmp_err->message);
      g_error_free(tmp_err);
      g_assert_not_reached();
    }
  }
}

static
void ev_application_class_init(EvApplicationClass* klass) {
  GApplicationClass* aclass = G_APPLICATION_CLASS(klass);

/*
 * vtable
 *
 */
  aclass->activate = ev_application_class_activate;
  aclass->open = ev_application_class_open;
}

static
void ev_application_init(EvApplication* self) {
}

/*
 * Entry
 *
 */

int main(int argc, char* argv[]) {
  GApplication* app =
  g_object_new
  (EV_TYPE_APPLICATION,
   "application-id", GAPPNAME,
   "flags", G_APPLICATION_HANDLES_OPEN,
   NULL);

  g_resources_register(ev_resources_get_resource());

  int status = g_application_run(app, argc, argv);
  while(G_IS_OBJECT(app))
    g_object_unref(app);
return status;
}
