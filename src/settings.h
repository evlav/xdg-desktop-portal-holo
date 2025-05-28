// settings.h: org.freedesktop.impl.portal.Settings
//
// SPDX-FileCopyrightText: 2024 Valve Corporation
// SPDX-License-Identifier: BSD-3-Clause

#include <gio/gio.h>
#include <stdbool.h>

G_BEGIN_DECLS

bool
settings_init (GDBusConnection *connection,
               GError **error);

G_END_DECLS
