# AION OS 6.4 — DNS and DHCP foundations

Introduces bounded DNS header and compressed-label name decoding (with pointer-loop rejection), and DHCPv4 OFFER/ACK option parsing (message type, IPv4 lease address, subnet mask, router, DNS server, server identifier). Tests cover malformed headers, DNS compression loops and basic DHCP options; CI includes UBSan and freestanding x86-64/ARM64 builds.

**Scope limitations:** No DNS resolver, query generation, retries, caching, DHCP client state machine, lease renewal, UDP transport integration, or actual network connectivity yet. DHCP options are parsed, not applied to the network interface.
