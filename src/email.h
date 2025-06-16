// email.h: org.freedesktop.impl.portal.Email
//
// SPDX-FileCopyrightText: 2025 Valve Corporation
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <gio/gio.h>
#include <stdbool.h>

G_BEGIN_DECLS

bool
email_init (GDBusConnection *bus, GError **error);

G_END_DECLS
