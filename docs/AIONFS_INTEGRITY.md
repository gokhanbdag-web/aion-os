# AION OS 4.5 — AION FS integrity

Adds CRC-32 checksums for individual data blocks, stored in a separate caller-reserved checksum block. Reads validate the magic, format version, block association and checksum, rejecting corrupt data and metadata. The caller must reserve both blocks and avoid overwriting them through raw AION FS access.

This is **corruption detection, not repair**. CRC-32 is not a cryptographic authentication primitive. Writes are not atomic: interruption between data and checksum writes may leave a mismatch. This milestone does not provide journal, recovery, global allocation, or transparent integrity coverage of every block. Recovery is milestone 4.6.
