// email.c: org.freedesktop.impl.portal.Email
//
// SPDX-FileCopyrightText: 2025 Valve Corporation
// SPDX-License-Identifier: BSD-3-Clause

#include "config.h"

#include "email.h"
#include "request.h"

#include "utils.h"
#include "xdg-desktop-portal-dbus.h"

static bool
handle_compose_email (XdpImplEmail *object,
                      GDBusMethodInvocation *invocation,
                      const char *arg_handle,
                      const char *arg_app_id,
                      const char */*arg_parent_window*/,
                      GVariant *arg_options)
{
  const char *sender = g_dbus_method_invocation_get_sender (invocation);
  g_autoptr(Request) request = request_new (sender, arg_app_id, arg_handle);
  g_object_ref (request);

  guint response = 0;
  g_autoptr(GAppInfo) info = get_steam_uri_helper ();
  if (!info)
    response = 2;
  else
    {
      const char *address = NULL;
      g_variant_lookup (arg_options, "address", "&s", &address);
      const char * const *addresses = NULL;
      g_variant_lookup (arg_options, "addresses", "^a&s", &addresses);
      if (!address && addresses)
        {
          address = addresses[0];
        }

      // The portal API for e-mail allows passing additional addresses,
      // CC and BCC fields, a subject, a body, and even attachments,
      // but steam-http-loader only allows one address and discards all
      // parameters when passed a mailto: URL, so there is no point in
      // passing them through.

      g_autoptr(GString) url = g_string_new (I_("mailto://"));
      g_string_append_printf (url, "%s", address);
      g_debug ("Launching %s with %s", g_app_info_get_display_name (info), url->str);
      g_autoptr(GList) uris = NULL;
      uris = g_list_append (uris, url->str);

      g_autoptr(GError) error = NULL;
      if (!g_app_info_launch_uris (info, uris, NULL, &error)) {
          response = 2;
          g_warning ("Failed to launch %s: %s", g_app_info_get_display_name (info), error->message);
          g_clear_error (&error);
      }
    }

  GVariantBuilder opt_builder;
  g_variant_builder_init (&opt_builder, G_VARIANT_TYPE_VARDICT);
  xdp_impl_email_complete_compose_email (object,
                                         invocation,
                                         response,
                                         g_variant_builder_end (&opt_builder));

  g_object_unref (request);

  return true;
}

bool
email_init (GDBusConnection *connection,
            GError **error)
{
  GDBusInterfaceSkeleton *helper =
    G_DBUS_INTERFACE_SKELETON (xdp_impl_email_skeleton_new ());

  g_signal_connect (helper, "handle-compose-email", G_CALLBACK (handle_compose_email), NULL);

  if (!g_dbus_interface_skeleton_export (helper, connection, DESKTOP_PORTAL_OBJECT_PATH, error))
    {
      return false;
    }

  g_debug ("Providing implementation for interface: %s",
           g_dbus_interface_skeleton_get_info (helper)->name);

  return true;
}
