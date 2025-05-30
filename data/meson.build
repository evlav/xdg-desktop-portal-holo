desktop_portal_interfaces = [
  'org.freedesktop.impl.portal.AppChooser',
  'org.freedesktop.impl.portal.Lockdown',
  'org.freedesktop.impl.portal.Settings',
]

libexecdir_conf = configuration_data()
libexecdir_conf.set('libexecdir', prefix / libexecdir)

holo_portal_conf = configuration_data()
holo_portal_conf.set('PORTALS', ';'.join(desktop_portal_interfaces))

# Special place where only the data files for the holo and gamescope portals
# will be installed (holo.portal, gamescope.portal and gamescope-portals.conf).
# The /usr/bin/start-gamescope-session script makes the
# $XDG_DESKTOP_PORTAL_DIR environment variable point to that place to ensure
# that no other portals are found.
portal_dir = datadir / 'xdg-desktop-portal' / 'gamescope-portals'

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
  install_dir: sysconfdir / 'xdg' / 'SteamOS' / 'portal',
)
