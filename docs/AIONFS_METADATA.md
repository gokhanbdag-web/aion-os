# AION OS 4.4 — AION FS metadata

Introduces a bounded single-block directory metadata format with up to eight fixed-size entries (name, kind, first data block, block count). Supports add, find, load, save, and rejects malformed names, duplicate names, overlapping extents, out-of-range extents, and references to the metadata block.

The caller chooses and reserves the metadata block (normally data block 0). This milestone does not implement a general allocator, hierarchical directories, atomic writes, or journaling. AION FS 4.3 raw data writes can still overwrite metadata: callers must respect the reserved region. Later milestones cover integrity and recovery.
