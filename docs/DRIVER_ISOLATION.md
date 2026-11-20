# AION OS 5.6 — Driver isolation policy

A fail-closed, owner-bound resource access policy for MMIO/DMA address ranges. Grants are explicit, bounded and non-overlapping; permissions are checked for each requested range. Revocation disables the policy and clears all grants.

**Security boundary:** This module is a policy decision component, not hardware-enforced isolation. Enforcing decisions requires integration at every device-memory mapping and DMA entry point, plus IOMMU configuration and per-driver address spaces. Until those are connected, a driver with unrestricted kernel privileges can bypass these checks. The test suite verifies policy semantics only.
