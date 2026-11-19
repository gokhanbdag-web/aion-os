# AION OS 5.1 — PCI device discovery

PCI enumeration reads PCI configuration-space registers via an injected platform callback. It discovers functions on bus 0 and follows PCI-to-PCI bridge secondary/subordinate bus ranges, collecting vendor/device IDs, class, subclass and programming interface.

The platform must supply a working config-space backend (legacy CF8/CFC or ECAM as appropriate). This module does not yet configure BARs, allocate resources, enable bus mastering, initialize drivers or implement ACPI/MCFG discovery. A fixed inventory cap of 64 devices returns ENOSPC when exceeded.
