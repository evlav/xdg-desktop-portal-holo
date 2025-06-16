gnome = import('gnome')

cc = meson.get_compiler('c')
cflags = cc.get_supported_arguments([
  '-fno-strict-aliasing',
  '-Wduplicated-branches',
  '-Wimplicit-fallthrough',
  '-Wmisleading-indentation',
  '-Wmissing-field-initializers',
  '-Wnonnull',
  '-Wnull-dereference',
  '-Wunused',
  '-Wstrict-prototypes',
  '-Wno-unused-parameter',
  '-Wno-cast-function-type',
  '-Wno-pedantic',
  '-Wno-bad-function-cast',
  '-Wno-string-plus-int',
  '-Wno-typedef-redefinition',
])

if get_option('werror')
  cflags += cc.get_supported_arguments([
    '-Werror=implicit-function-declaration',
    '-Werror=missing-prototypes',
    '-Werror=pointer-sign',
    '-Werror=format=2',
    '-Werror=init-self',
    '-Werror=missing-include-dirs',
    '-Werror=pointer-arith',
    '-Werror=unused-result',
    '-Werror=implicit-function-declaration',
    '-Werror=missing-prototypes',
    '-Werror=pointer-sign',
  ])
endif

xdg_desktop_portal_dep = dependency('xdg-desktop-portal', version: '>= 1.17.0')

desktop_portal_interfaces_dir = xdg_desktop_portal_dep.get_variable(pkgconfig: 'interfaces_dir')

# Additional interfaces
desktop_portal_interfaces += [
  'org.freedesktop.impl.portal.Request',
]

desktop_portal_interfaces_files = []
foreach intf: desktop_portal_interfaces
  desktop_portal_interfaces_files += [desktop_portal_interfaces_dir / '@0@.xml'.format(intf)]
endforeach


built_sources = []

built_sources += gnome.gdbus_codegen(
  'xdg-desktop-portal-dbus',
  sources: desktop_portal_interfaces_files,
  interface_prefix: 'org.freedesktop.impl.portal.',
  namespace: 'XdpImpl',
)

config_h = configuration_data()
config_h.set_quoted('GETTEXT_PACKAGE', meson.project_name())
config_h.set_quoted('LOCALEDIR', prefix / get_option('localedir'))
config_h.set_quoted('PACKAGE_NAME', meson.project_name())
config_h.set_quoted('PACKAGE_STRING', '@0@ @1@'.format(meson.project_name(), meson.project_version()))
built_sources += configure_file(output: 'config.h', configuration: config_h)

deps = [
  declare_dependency(sources: built_sources),
  meson.get_compiler('c').find_library('m'),
  dependency('fontconfig'),
  dependency('glib-2.0', version: '>= 2.62'),
  dependency('gio-unix-2.0'),
  xdg_desktop_portal_dep,
]

sources = [
  'appchooser.c',
  'email.c',
  'lockdown.c',
  'request.c',
  'settings.c',
  'utils.c',

  'xdg-desktop-portal-holo.c',
]

add_project_arguments(['-D_GNU_SOURCE'], language: 'c')

executable(
  'xdg-desktop-portal-holo',
  sources: [
    sources,
    built_sources,
  ],
  c_args: cflags,
  dependencies: deps,
  install: true,
  install_dir: libexecdir,
)
