# AION OS 6.3 — IPv4 / TCP / UDP foundations

Provides bounded IPv4 packet header parsing (including variable IHL, total length, header checksum and fragmentation metadata), UDP datagram parsing, and TCP segment header parsing (ports, sequence, acknowledgment, flags, window, data offset). Includes malformed-input regression tests and freestanding x86-64/ARM64 compilation.

**Scope:** This is protocol decoding, not a functional IP stack. It does not validate UDP/TCP pseudo-header checksums, reassemble IPv4 fragments, route packets, maintain TCP connections, implement retransmissions/congestion control, or transmit network packets. IPv6 and packet I/O are not yet supported.
