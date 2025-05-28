// utils.c: Utility functions
//
// SPDX-FileCopyrightText: 2024 Valve Corporation
// SPDX-License-Identifier: BSD-3-Clause

#include "config.h"

#include "utils.h"

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

#include <gio/gio.h>

typedef enum {
  MESSAGE_TYPE_INFO,
  MESSAGE_TYPE_WARNING,
  MESSAGE_TYPE_ERROR
} MessageType;

static void
print_message (MessageType mtype,
               const char *fmt,
               va_list args)
{
  static const struct {
    const char *pre;
    const char *msg;
    const char *post;
  } escaped_prefix[] = {
    [MESSAGE_TYPE_INFO] = {
      .pre = "\x1b[34m\x1b[1m", .msg = "INFO",    .post = "\x1b[22m\x1b[0m",
    },
    [MESSAGE_TYPE_WARNING] = {
      .pre = "\x1b[33m\x1b[1m", .msg = "WARNING", .post = "\x1b[22m\x1b[0m",
    },
    [MESSAGE_TYPE_ERROR] = {
      .pre = "\x1b[31m\x1b[1m", .msg = "ERROR",   .post = "\x1b[22m\x1b[0m",
    },
  };

  g_assert (mtype >= MESSAGE_TYPE_INFO && mtype <= MESSAGE_TYPE_ERROR);

  if (isatty (fileno (stderr)))
    {
      fprintf (stderr, "%s%s%s: ",
               escaped_prefix[mtype].pre,
               escaped_prefix[mtype].msg,
               escaped_prefix[mtype].post);
    }
  else
    {
      fprintf (stderr, "%s: ", escaped_prefix[mtype].msg);
    }

  vfprintf (stderr, fmt, args);
  fputc ('\n', stderr);
}

void
print_error (const char *fmt,
             ...)
{
  va_list args;

  va_start (args, fmt);
  print_message (MESSAGE_TYPE_ERROR, fmt, args);
  va_end (args);
}

void
print_warning (const char *fmt,
               ...)
{
  va_list args;

  va_start (args, fmt);
  print_message (MESSAGE_TYPE_WARNING, fmt, args);
  va_end (args);
}

void
print_info (const char *fmt,
            ...)
{
  va_list args;

  va_start (args, fmt);
  print_message (MESSAGE_TYPE_INFO, fmt, args);
  va_end (args);
}

static const GDBusErrorEntry xdg_desktop_portal_error_entries[] = {
  { XDG_DESKTOP_PORTAL_ERROR_FAILED,           "org.freedesktop.portal.Error.Failed" },
  { XDG_DESKTOP_PORTAL_ERROR_INVALID_ARGUMENT, "org.freedesktop.portal.Error.InvalidArgument" },
  { XDG_DESKTOP_PORTAL_ERROR_NOT_FOUND,        "org.freedesktop.portal.Error.NotFound" },
  { XDG_DESKTOP_PORTAL_ERROR_EXISTS,           "org.freedesktop.portal.Error.Exists" },
  { XDG_DESKTOP_PORTAL_ERROR_NOT_ALLOWED,      "org.freedesktop.portal.Error.NotAllowed" },
  { XDG_DESKTOP_PORTAL_ERROR_CANCELLED,        "org.freedesktop.portal.Error.Cancelled" },
  { XDG_DESKTOP_PORTAL_ERROR_WINDOW_DESTROYED, "org.freedesktop.portal.Error.WindowDestroyed" }
};

GQuark
xdg_desktop_portal_error_quark (void)
{
  static volatile gsize quark_volatile = 0;

  g_dbus_error_register_error_domain ("xdg-desktop-portal-error-quark",
                                      &quark_volatile,
                                      xdg_desktop_portal_error_entries,
                                      G_N_ELEMENTS (xdg_desktop_portal_error_entries));
  return (GQuark) quark_volatile;
}
