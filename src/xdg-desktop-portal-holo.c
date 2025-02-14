// xdg-desktop-portal-holo.c: Main
//
// SPDX-FileCopyrightText: 2024  Igalia S.L.
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "config.h"

#include "lockdown.h"
#include "settings.h"

#include "utils.h"

#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <glib/gi18n.h>

static gboolean opt_verbose;
static gboolean opt_replace;
static gboolean opt_version;

static GOptionEntry opt_entries[] = {
  {
    .long_name = "verbose",
    .short_name = 'v',
    .flags = 0,
    .arg = G_OPTION_ARG_NONE,
    .arg_data = &opt_verbose,
    .description = "Print debugging information during processing",
    .arg_description = NULL,
  },
  {
    .long_name = "replace",
    .short_name = 'r',
    .flags = 0,
    .arg = G_OPTION_ARG_NONE,
    .arg_data = &opt_replace,
    .description = "Replace a running instance",
    .arg_description = NULL,
  },
  {
    .long_name = "version",
    .short_name = 0,
    .flags = 0,
    .arg = G_OPTION_ARG_NONE,
    .arg_data = &opt_version,
    .description = "Print the version and exit",
    .arg_description = NULL,
  },
  G_OPTION_ENTRY_NULL,
};

static GMainLoop *main_loop;

static void
printerr_handler (const char *msg)
{
  print_error ("%s", msg);
}

static void
on_bus_acquired (GDBusConnection *bus,
                 const char *name,
                 gpointer user_data G_GNUC_UNUSED)
{
  g_autoptr (GError) error = NULL;

  if (!lockdown_init (bus, &error))
    {
      print_warning ("Unable to initialize lockdown interface: %s", error->message);
      g_clear_error (&error);
    }

  if (!settings_init (bus, &error))
    {
      print_warning ("Unable to initialize settings interface: %s", error->message);
      g_clear_error (&error);
    }
}

static void
on_name_acquired (GDBusConnection *bus,
                  const char *name,
                  gpointer user_data G_GNUC_UNUSED)
{
  print_info ("Name acquired: %s", name);
}

static void
on_name_lost (GDBusConnection *bus,
              const char *name,
              gpointer user_data G_GNUC_UNUSED)
{
  print_info ("Name lost: %s", name);
  g_main_loop_quit (main_loop);
}

int
main (int argc,
      char *argv[])
{
  setlocale (LC_ALL, "");
  bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);

  g_set_prgname (PACKAGE_NAME);
  g_set_printerr_handler (printerr_handler);

  g_autoptr (GError) error = NULL;

  g_autoptr (GOptionContext) opt_context = g_option_context_new (" - Holo portal backend");
  g_option_context_set_summary (opt_context, "A backend implementation for xdg-desktop-portal");
  g_option_context_add_main_entries (opt_context, opt_entries, NULL);
  if (!g_option_context_parse (opt_context, &argc, &argv, &error))
    {
      print_error ("%s: %s", g_get_prgname (), error->message);
      print_error ("Try “%s --help” for more information", g_get_prgname ());
      return EXIT_FAILURE;
    }

  if (opt_version)
    {
      fprintf (stdout, "%s\n", PACKAGE_STRING);
      return EXIT_SUCCESS;
    }

  GDBusConnection *session_bus = g_bus_get_sync (G_BUS_TYPE_SESSION, NULL, &error);
  if (session_bus == NULL && error != NULL)
    {
      print_error ("Unable to acquire session bus: %", error->message);
      return EXIT_FAILURE;
    }

  main_loop = g_main_loop_new (NULL, false);

  GBusNameOwnerFlags owner_flags = G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT |
    (opt_replace ? G_BUS_NAME_OWNER_FLAGS_REPLACE : 0);
  gulong owner_id = g_bus_own_name (G_BUS_TYPE_SESSION,
                                    DESKTOP_PORTAL_NAME_STEAM,
                                    owner_flags,
                                    on_bus_acquired,
                                    on_name_acquired,
                                    on_name_lost,
                                    NULL,
                                    NULL);

  g_main_loop_run (main_loop);

  g_bus_unown_name (owner_id);

  return EXIT_SUCCESS;
}
