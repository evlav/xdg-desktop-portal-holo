// lockdown.h: org.freedesktop.impl.portal.Lockdown
//
// SPDX-FileCopyrightText: 2024 Valve Corporation
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <gio/gio.h>
#include <stdbool.h>

G_BEGIN_DECLS

bool
lockdown_init (GDBusConnection *bus, GError **error);

G_END_DECLS
