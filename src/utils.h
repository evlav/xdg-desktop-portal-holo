// utils.h: Utility functions
//
// SPDX-FileCopyrightText: 2024 Valve Corporation
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <glib.h>

#define DESKTOP_PORTAL_OBJECT_PATH "/org/freedesktop/portal/desktop"
#define DESKTOP_PORTAL_NAME_STEAM "org.freedesktop.impl.portal.desktop.holo"

G_BEGIN_DECLS

typedef enum {
  XDG_DESKTOP_PORTAL_ERROR_FAILED     = 0,
  XDG_DESKTOP_PORTAL_ERROR_INVALID_ARGUMENT,
  XDG_DESKTOP_PORTAL_ERROR_NOT_FOUND,
  XDG_DESKTOP_PORTAL_ERROR_EXISTS,
  XDG_DESKTOP_PORTAL_ERROR_NOT_ALLOWED,
  XDG_DESKTOP_PORTAL_ERROR_CANCELLED,
  XDG_DESKTOP_PORTAL_ERROR_WINDOW_DESTROYED
} XdgDesktopPortalErrorEnum;

#define XDG_DESKTOP_PORTAL_ERROR xdg_desktop_portal_error_quark ()

GQuark  xdg_desktop_portal_error_quark (void);

void
print_error (const char *fmt,
             ...);

void
print_warning (const char *fmt,
               ...);

void
print_info (const char *fmt,
            ...);

char *xdp_get_app_id_from_desktop_id (const char *desktop_id);

G_END_DECLS
