// lockdown.c: org.freedesktop.impl.portal.Lockdown
//
// SPDX-FileCopyrightText: 2024-2025 Valve Corporation
// SPDX-License-Identifier: BSD-3-Clause

#include "config.h"

#include "lockdown.h"

#include "utils.h"
#include "xdg-desktop-portal-dbus.h"

#include <string.h>

#define LOCKDOWN_GROUP  "Lockdown"
#define PRIVACY_GROUP   "Privacy"

#define LOCKDOWN_PRINTING_KEY                   "Printing"
#define LOCKDOWN_SAVE_TO_DISK_KEY               "SaveToDisk"
#define LOCKDOWN_APPLICATION_HANDLERS_KEY       "ApplicationHandlers"
#define LOCKDOWN_LOCATION_KEY                   "Location"
#define PRIVACY_CAMERA_KEY                      "Camera"
#define PRIVACY_MICROPHONE_KEY                  "Microphone"
#define PRIVACY_SOUND_OUTPUT_KEY                "SoundOutput"

G_DECLARE_FINAL_TYPE (LockdownManager, lockdown_manager, LOCKDOWN, MANAGER, GObject)

struct _LockdownManager
{
  GObject parent_instance;

  /* HashTable<unowned str, int> */
  GHashTable *keys;

  GFileMonitor *file_monitor;
};

static LockdownManager *manager;

static inline void
lockdown_manager_set_key (LockdownManager *self,
                          const char *key,
                          gboolean value)
{
  g_hash_table_insert (self->keys, (gpointer) key, GINT_TO_POINTER (value));
}

static inline gboolean
lockdown_manager_get_key (LockdownManager *self,
                          const char *key)
{
  gpointer res = g_hash_table_lookup (self->keys, key);

  return GPOINTER_TO_INT (res);
}

static bool load_lockdown_config (LockdownManager *lockdown_manager,
                                  bool notify);

static void
lockdown_manager__file_monitor__changed (GFileMonitor *monitor,
                                         GFile *file,
                                         GFile *other_file,
                                         gpointer user_data)
{
  load_lockdown_config (user_data, true);
}

static bool
load_lockdown_config (LockdownManager *lockdown_manager,
                      bool notify)
{
  g_autoptr (GPtrArray) search_dirs = g_ptr_array_new_null_terminated (8, g_free, true);

  /* XDG_CONFIG_HOME/SteamOS/portal/lockdown.conf */
  GPathBuf buf;
  g_path_buf_init_from_path (&buf, g_get_user_config_dir ());
  g_path_buf_push (&buf, "SteamOS");
  g_path_buf_push (&buf, "portal");
  char *path = g_path_buf_clear_to_path (&buf);
  g_ptr_array_add (search_dirs, path);

  /* XDG_CONFIG_DIRS/SteamOS/portal/lockdown.conf */
  const char * const *system_dirs = g_get_system_config_dirs ();
  for (size_t i = 0; system_dirs[i] != NULL; i++)
    {
      g_path_buf_init_from_path (&buf, system_dirs[i]);
      g_path_buf_push (&buf, "SteamOS");
      g_path_buf_push (&buf, "portal");
      char *path = g_path_buf_clear_to_path (&buf);

      g_ptr_array_add (search_dirs, path);
    }

  g_autoptr (GKeyFile) kf = g_key_file_new ();
  g_autofree char *full_path = NULL;
  g_autoptr (GError) error = NULL;

  if (!g_key_file_load_from_dirs (kf, "lockdown.conf", (const char **) search_dirs->pdata, &full_path, G_KEY_FILE_NONE, &error))
    {
      g_debug ("Unable to read lockdown.conf: %s", error->message);
      return false;
    }

  g_debug ("Loading lockdown configuration from: %s", full_path);

  gboolean printing = !g_key_file_get_boolean (kf, LOCKDOWN_GROUP, LOCKDOWN_PRINTING_KEY, NULL);
  gboolean save_to_disk = !g_key_file_get_boolean (kf, LOCKDOWN_GROUP, LOCKDOWN_SAVE_TO_DISK_KEY, NULL);
  gboolean app_handlers = !g_key_file_get_boolean (kf, LOCKDOWN_GROUP, LOCKDOWN_APPLICATION_HANDLERS_KEY, NULL);
  gboolean location = !g_key_file_get_boolean (kf, LOCKDOWN_GROUP, LOCKDOWN_LOCATION_KEY, NULL);
  gboolean camera = !g_key_file_get_boolean (kf, PRIVACY_GROUP, PRIVACY_CAMERA_KEY, NULL);
  gboolean microphone = !g_key_file_get_boolean (kf, PRIVACY_GROUP, PRIVACY_MICROPHONE_KEY, NULL);
  gboolean sound_output = !g_key_file_get_boolean (kf, PRIVACY_GROUP, PRIVACY_SOUND_OUTPUT_KEY, NULL);

  if (notify)
    {
      g_object_set (lockdown_manager,
                    "printing", printing,
                    "save-to-disk", save_to_disk,
                    "application-handlers", app_handlers,
                    "location", location,
                    "camera", camera,
                    "microphone", microphone,
                    "sound-output", sound_output,
                    NULL);
    }
  else
    {
      lockdown_manager_set_key (lockdown_manager, I_("printing"), printing);
      lockdown_manager_set_key (lockdown_manager, I_("save-to-disk"), save_to_disk);
      lockdown_manager_set_key (lockdown_manager, I_("application-handlers"), app_handlers);
      lockdown_manager_set_key (lockdown_manager, I_("location"), location);
      lockdown_manager_set_key (lockdown_manager, I_("camera"), camera);
      lockdown_manager_set_key (lockdown_manager, I_("microphone"), microphone);
      lockdown_manager_set_key (lockdown_manager, I_("sound-output"), sound_output);
    }

  if (lockdown_manager->file_monitor == NULL)
    {
      g_autoptr (GFile) file = g_file_new_for_path (full_path);
      lockdown_manager->file_monitor = g_file_monitor_file (file, G_FILE_MONITOR_NONE, NULL, &error);
      if (error != NULL)
        g_debug ("Unable to monitor lockdown.conf: %s", error->message);

      g_signal_connect (lockdown_manager->file_monitor, "changed", G_CALLBACK (lockdown_manager__file_monitor__changed), lockdown_manager);
    }

  return true;
}

G_DEFINE_TYPE (LockdownManager, lockdown_manager, G_TYPE_OBJECT)

enum
{
  PROP_PRINTING = 1,
  PROP_SAVE_TO_DISK,
  PROP_APPLICATION_HANDLERS,
  PROP_LOCATION,
  PROP_CAMERA,
  PROP_MICROPHONE,
  PROP_SOUND_OUTPUT,

  N_PROPS
};

static GParamSpec *obj_props[N_PROPS];

static void
lockdown_manager_constructed (GObject *gobject)
{
  load_lockdown_config ((LockdownManager *) gobject, false);
}

static void
lockdown_manager_set_property (GObject *gobject,
                               guint prop_id,
                               const GValue *value,
                               GParamSpec *pspec)
{
  lockdown_manager_set_key ((LockdownManager *) gobject, pspec->name, g_value_get_boolean (value));
}

static void
lockdown_manager_get_property (GObject *gobject,
                               guint prop_id,
                               GValue *value,
                               GParamSpec *pspec)
{
  g_value_set_boolean (value, lockdown_manager_get_key ((LockdownManager *) gobject, pspec->name));
}

static void
lockdown_manager_finalize (GObject *gobject)
{
  LockdownManager *self = LOCKDOWN_MANAGER (gobject);

  g_clear_object (&self->file_monitor);
  g_clear_pointer (&self->keys, g_hash_table_unref);

  G_OBJECT_CLASS (lockdown_manager_parent_class)->finalize (gobject);
}

static void
lockdown_manager_class_init (LockdownManagerClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->constructed = lockdown_manager_constructed;
  gobject_class->set_property = lockdown_manager_set_property;
  gobject_class->get_property = lockdown_manager_get_property;
  gobject_class->finalize = lockdown_manager_finalize;

  obj_props[PROP_PRINTING] = g_param_spec_boolean (I_("printing"), NULL, NULL, TRUE, G_PARAM_READWRITE | G_PARAM_STATIC_NAME);
  obj_props[PROP_SAVE_TO_DISK] = g_param_spec_boolean (I_("save-to-disk"), NULL, NULL, TRUE, G_PARAM_READWRITE | G_PARAM_STATIC_NAME);
  obj_props[PROP_APPLICATION_HANDLERS] = g_param_spec_boolean (I_("application-handlers"), NULL, NULL, TRUE, G_PARAM_READWRITE | G_PARAM_STATIC_NAME);
  obj_props[PROP_LOCATION] = g_param_spec_boolean (I_("location"), NULL, NULL, TRUE, G_PARAM_READWRITE | G_PARAM_STATIC_NAME);
  obj_props[PROP_CAMERA] = g_param_spec_boolean (I_("camera"), NULL, NULL, TRUE, G_PARAM_READWRITE | G_PARAM_STATIC_NAME);
  obj_props[PROP_MICROPHONE] = g_param_spec_boolean (I_("microphone"), NULL, NULL, TRUE, G_PARAM_READWRITE | G_PARAM_STATIC_NAME);
  obj_props[PROP_SOUND_OUTPUT] = g_param_spec_boolean (I_("sound-output"), NULL, NULL, TRUE, G_PARAM_READWRITE | G_PARAM_STATIC_NAME);
  g_object_class_install_properties (gobject_class, N_PROPS, obj_props);
}

static void
lockdown_manager_init (LockdownManager *self)
{
  self->keys = g_hash_table_new (g_str_hash, g_str_equal);
}

bool
lockdown_init (GDBusConnection *connection,
               GError **error)
{
  if (g_once_init_enter_pointer (&manager))
    {
      GDBusInterfaceSkeleton *helper =
        G_DBUS_INTERFACE_SKELETON (xdp_impl_lockdown_skeleton_new ());

      LockdownManager *res = g_object_new (lockdown_manager_get_type (), NULL);

      GBindingFlags flags = G_BINDING_BIDIRECTIONAL | G_BINDING_INVERT_BOOLEAN;
      g_object_bind_property (res, "printing", helper, "disable-printing", flags);
      g_object_bind_property (res, "save-to-disk", helper, "disable-save-to-disk", flags);
      g_object_bind_property (res, "application-handlers", helper, "disable-application-handlers", flags);
      g_object_bind_property (res, "location", helper, "disable-location", flags);
      g_object_bind_property (res, "camera", helper, "disable-camera", flags);
      g_object_bind_property (res, "microphone", helper, "disable-microphone", flags);
      g_object_bind_property (res, "sound-output", helper, "disable-sound-output", flags);

      if (!g_dbus_interface_skeleton_export (helper, connection, DESKTOP_PORTAL_OBJECT_PATH, error))
        {
          g_object_unref (res);
          return false;
        }

      g_debug ("Providing implementation for interface: %s", g_dbus_interface_skeleton_get_info (helper)->name);

      g_once_init_leave_pointer (&manager, res);
    }

  return true;
}
