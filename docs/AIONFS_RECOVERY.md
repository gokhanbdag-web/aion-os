# AION OS 4.6 — AION FS recovery

The recovery primitive restores a damaged data block from an independently reserved replica, **only after** the replica has passed the CRC-32 verification introduced in 4.5. A corrupt replica is rejected without modifying the primary. The caller manages block allocation and maintains the replica and its checksum.

This is a bounded, explicit recovery primitive; it is not a journal, an atomic update protocol, automatic mount-time repair, or protection against simultaneous corruption of both copies. CRC-32 does not authenticate malicious changes. Failed writes can still leave partial data; callers must handle I/O failures.
