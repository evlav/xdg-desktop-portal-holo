# XDG desktop portal for holo-specific interfaces

This XDG desktop portal backend implements the following [backend interfaces](https://flatpak.github.io/xdg-desktop-portal/docs/impl-dbus-interfaces.html):

* [AppChooser](https://flatpak.github.io/xdg-desktop-portal/docs/doc-org.freedesktop.impl.portal.AppChooser.html)
* [Email](https://flatpak.github.io/xdg-desktop-portal/docs/doc-org.freedesktop.impl.portal.Email.html)
* [Lockdown](https://flatpak.github.io/xdg-desktop-portal/docs/doc-org.freedesktop.impl.portal.Lockdown.html)
* [Settings](https://flatpak.github.io/xdg-desktop-portal/docs/doc-org.freedesktop.impl.portal.Settings.html)

## How to build and install

```shell
$ meson setup --prefix /usr _build .
$ meson compile -C _build
$ meson install -C _build --no-rebuild
```

## Authors

* Emmanuele Bassi <ebassi@igalia.com>
* Olivier Tilloy <otilloy@igalia.com>

## License

Original code for xdg-desktop-portal-holo is published under the [3-Clause BSD license](LICENSES/BSD-3-Clause.txt).

This project includes copies of `request.{h,c}` (from [xdg-desktop-portal-gtk](https://github.com/flatpak/xdg-desktop-portal-gtk)) which are published under the [GNU Lesser General Public License version 2 or later](LICENSES/LGPL-2.1-or-later.txt).

The aggregate of all sources is therefore published under the [GNU Lesser General Public License version 2.1 or later](LICENSES/LGPL-2.1-or-later.txt).

