// lockdown.h: org.freedesktop.impl.portal.Lockdown
//
// SPDX-FileCopyrightText: 2024 Igalia S.L.
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <gio/gio.h>
#include <stdbool.h>

G_BEGIN_DECLS

bool
lockdown_init (GDBusConnection *bus, GError **error);

G_END_DECLS
