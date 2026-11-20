# AION OS 6.2 — Ethernet frame foundation

Implements bounded Ethernet II header parsing and construction, plus single-tag IEEE 802.1Q/802.1ad parsing with VLAN ID and priority extraction. Rejects malformed lengths, nested VLAN tags, and IEEE 802.3 length-field frames pending separate support.

This is frame-format logic only. NIC DMA/ring operations, ARP, packet transmission/reception, minimum-frame padding and FCS generation/verification are not yet implemented. No live networking is claimed.
