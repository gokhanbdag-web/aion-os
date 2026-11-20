# AION OS 6.5 — TLS record framing foundation

Provides bounded parsing of TLS record headers and a header-writing helper. Supports TLS record content types 20–23 and legacy record versions 0x0300–0x0303, with explicit payload size limits, truncation rejection and consumed-byte reporting for concatenated records. Unit tests, UBSan and freestanding x86-64/ARM64 builds are included.

**Security boundary:** This is framing only. It does **not** implement TLS 1.3 handshakes, encryption, authentication, key exchange, certificate verification, replay protection or secure communication. A production TLS implementation requires a vetted cryptographic library, verified state machine and dedicated security review. Do not treat parsed application-data records as trusted plaintext.
