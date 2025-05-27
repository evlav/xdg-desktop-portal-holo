.. _xdg-desktop-portal-holo-lockdown(5):
.. meta::
   :copyright: 2025 Valve Corporation


================================
xdg-desktop-portal-holo-lockdown
================================

lockdown.conf
-------------

SYNOPSIS
--------

|  $XDG_CONFIG_DIRS/SteamOS/portal/lockdown.conf
|  $XDG_CONFIG_HOME/SteamOS/portal/lockdown.conf

DESCRIPTION
-----------

The ``lockdown.conf`` configuration file specifies the lockdown settings for the
Holo xdg-desktop-portal backend.

The format used for the configuration file is a key/value pairs file as
described by the `XDG desktop entry specification <https://specifications.freedesktop.org/desktop-entry-spec/latest/basic-format.html>`_.

KEYS
----

The keys are divided into two groups:

* Lockdown

* Privacy

Lockdown
~~~~~~~~

The following keys are part of the ``Lockdown`` group:

* **Printing**: a boolean value to control whether printing is enabled; use
  ``false`` to disable printing support.

* **SaveToDisk**: a boolean value to control whether saving to disk is enabled;
  use ``false`` to disable saving to disk.

* **ApplicationHandlers**: a boolean value to control whether application
  handlers are enabled; use ``false`` to disable application handlers.

* **Location**: a boolean value to control whether location services are
  enabled; use ``false`` to disable location services.

Privacy
~~~~~~~

The following keys are part of the ``Privacy`` group:

* **Camera**: a boolean value to control whether camera access is enabled; use
  ``false`` to disable access to cameras.

* **Microphone**: a boolean value to control whether microphone access is
  enabled; use ``false`` to disable access to the microphone.

* **SoundOutput**: a boolean value to control whether sound output is enabled;
  use ``false`` to disable sound output.

SEE ALSO
--------

* `Lockdown portal <https://flatpak.github.io/xdg-desktop-portal/docs/doc-org.freedesktop.impl.portal.Lockdown.html>`_
