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
#include <ev2_application.h>
#include <ev2_message.h>
#include <ev2_module_manager.h>
#include <ev2_parser.h>
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
  EvModuleManager* manager;
};

static
const GOptionEntry opts[] = {
  {"version", 'v', 0, G_OPTION_ARG_NONE, NULL, "Print version information and exit", NULL},
  {NULL, 0, 0, G_OPTION_ARG_NONE, NULL, NULL, NULL},
};

G_DEFINE_TYPE
(EvApplication,
 ev_application,
 GTK_TYPE_APPLICATION);

static
gint ev_application_class_handle_local_options(GApplication* pobj, GVariantDict* options) {
  GVariant* value =
  g_variant_dict_lookup_value
  (options,
   "version",
   G_VARIANT_TYPE_BOOLEAN);
  gint return_ = -1;

  if(value != NULL)
  {
    if(g_variant_get_boolean(value) == TRUE)
    {
      g_print
      ("Copyright 2021-2022 MarcosHCK\r\n"
       PACKAGE_STRING "\r\n"
       "\r\n"
       "Please report bugs to " PACKAGE_BUGREPORT "\r\n"
       "Visit our website at " PACKAGE_URL "\r\n");
      return_ = 0;
    }

    g_variant_unref(value);
  }
return return_;
}

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
      EvMessage* message =
      ev_message_new_error_with_gerror(tmp_err);
      gtk_window_set_application(GTK_WINDOW(message), GTK_APPLICATION(pself));
      gtk_dialog_run(GTK_DIALOG(message));
      gtk_widget_destroy(GTK_WIDGET(message));
    }
  }
}

static
void ev_application_class_dispose(GObject* pself) {
  EvApplication* self = EV_APPLICATION(pself);

/*
 * Dispose
 *
 */
  g_clear_object(&(self->manager));

/*
 * Chain-up
 *
 */
  G_OBJECT_CLASS(ev_application_parent_class)->dispose(pself);
}

static
void ev_application_class_init(EvApplicationClass* klass) {
  GApplicationClass* aclass = G_APPLICATION_CLASS(klass);
  GObjectClass* oclass = G_OBJECT_CLASS(klass);

/*
 * vtable
 *
 */
  aclass->handle_local_options = ev_application_class_handle_local_options;
  aclass->activate = ev_application_class_activate;
  aclass->open = ev_application_class_open;
  oclass->dispose = ev_application_class_dispose;
}

static
void ev_application_init(EvApplication* self) {
  g_application_add_main_option_entries
  (G_APPLICATION(self), opts);
}

/*
 * Object methods
 *
 */

gpointer
ev_application_get_module_manager(EvApplication* application) {
  g_return_val_if_fail(EV_IS_APPLICATION(application), NULL);
return application->manager;
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

  GError* tmp_err = NULL;
  ((EvApplication*)app)->manager =
  ev_module_manager_new(NULL, &tmp_err);
  if G_UNLIKELY(tmp_err != NULL)
  {
    g_critical(tmp_err->message);
    g_error_free(tmp_err);
    g_assert_not_reached();
  }

  g_resources_register(ev_resources_get_resource());

  int status = g_application_run(app, argc, argv);
  while(G_IS_OBJECT(app))
    g_object_unref(app);
return status;
}
