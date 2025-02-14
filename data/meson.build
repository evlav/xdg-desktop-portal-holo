desktop_portal_interfaces = [
  'org.freedesktop.impl.portal.Lockdown',
  'org.freedesktop.impl.portal.Settings',
]

portal_dir = datadir / 'xdg-desktop-portal' / 'portals'

libexecdir_conf = configuration_data()
libexecdir_conf.set('libexecdir', prefix / libexecdir)

holo_portal_conf = configuration_data()
holo_portal_conf.set('PORTALS', ';'.join(desktop_portal_interfaces))

portal_dir = datadir / 'xdg-desktop-portal' / 'portals'
configure_file(
  input: 'holo.portal.in',
  output: 'holo.portal',
  configuration: holo_portal_conf,
  install: true,
  install_dir: portal_dir,
)

# Service
configure_file(
  input: 'org.freedesktop.impl.portal.desktop.holo.service.in',
  output: 'org.freedesktop.impl.portal.desktop.holo.service',
  configuration: libexecdir_conf,
  install: true,
  install_dir: dbus_service_dir,
)

# systemd unit
if systemd_user_unit_dir != ''
  configure_file(
    input: 'xdg-desktop-portal-holo.service.in',
    output: 'xdg-desktop-portal-holo.service',
    configuration: libexecdir_conf,
    install: true,
    install_dir: systemd_user_unit_dir,
  )
endif

# Desktop file
desktop_in = configure_file(
  input: 'xdg-desktop-portal-holo.desktop.in',
  output: 'xdg-desktop-portal-holo.desktop',
  configuration: libexecdir_conf,
  install: true,
  install_dir: get_option('datadir') / 'applications',
)

# Configuration files
install_data([
    'lockdown.conf',
    'settings.conf',
  ],
  install_dir: sysconfdir / 'xdg' / 'Holo',
)
