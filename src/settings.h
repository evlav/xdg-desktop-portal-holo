// settings.h: org.freedesktop.impl.portal.Settings
//
// SPDX-FileCopyrightText: 2024  Igalia S.L.
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <gio/gio.h>
#include <stdbool.h>

G_BEGIN_DECLS

bool
settings_init (GDBusConnection *connection,
               GError **error);

G_END_DECLS
