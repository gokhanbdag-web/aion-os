# AION OS 6.1 — Network interface discovery foundation

Introduces a bounded inventory of PCI network-class devices and lookup for Ethernet-class devices, retaining PCI BDF coordinates, vendor/device IDs, and subclass/interface codes. This lays groundwork for attaching drivers to devices discovered by the PCI subsystem.

**Limitations:** The inventory is not yet wired to PCI scanning and does not implement an Ethernet NIC driver, DMA queues, MAC address retrieval, link negotiation or packet transmission. These require actual hardware drivers and subsequent network-stack milestones. ARM64 systems with non-PCI NICs will require a separate discovery adapter.
