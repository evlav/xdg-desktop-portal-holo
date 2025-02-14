project('xdg-desktop-portal-holo', 'c',
  version: '2024.1',
  meson_version: '>=1.2',
  license: 'LGPL-2.1-or-later',
  default_options: [
    'warning_level=2',
    'c_std=gnu11,c11',
  ],
)

prefix = get_option('prefix')
datadir = get_option('datadir')
libdir = get_option('libdir')
libexecdir = get_option('libexecdir')
sysconfdir = get_option('sysconfdir')

systemd_user_unit_dir = get_option('systemd_user_unit_dir')
if systemd_user_unit_dir == ''
  systemd = dependency('systemd', version: '>= 242')
  systemd_user_unit_dir = systemd.get_variable(
    pkgconfig: 'systemduserunitdir',
    pkgconfig_define: ['prefix', get_option('prefix')]
  )
endif

dbus_service_dir = get_option('dbus_service_dir')
if dbus_service_dir == ''
  dbus_service_dir = datadir / 'dbus-1' / 'services'
endif

subdir('data')
subdir('src')

summary({
    'prefix': prefix,
    'datadir': datadir,
    'libdir': libdir,
    'libexecdir': libexecdir,
    'dbus_service_dir': dbus_service_dir,
    'systemduserunitdir': systemd_user_unit_dir,
  },
  section: 'Directories',
)
