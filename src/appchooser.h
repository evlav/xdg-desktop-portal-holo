// appchooser.h: org.freedesktop.impl.portal.AppChooser
//
// SPDX-FileCopyrightText: 2025 Igalia S.L.
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <gio/gio.h>
#include <stdbool.h>

G_BEGIN_DECLS

bool
app_chooser_init (GDBusConnection *bus, GError **error);

G_END_DECLS
