# AION FS — milestone 4.3

Initial AION FS on-disk superblock and partition-scoped data-block access.

The first block of a selected partition contains a little-endian magic, version, total partition block count, and data start offset. Format writes one superblock; mount validates it. Data access is partition-bounded. The caller provides one block of scratch space. This is an early filesystem foundation, **not yet a file/directory filesystem**. Metadata, allocation, integrity and recovery are planned in milestones 4.4–4.6. Formatting is destructive to the first block of the selected region. No on-disk integrity checksum or journaling is provided yet.
