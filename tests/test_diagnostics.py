#!/usr/bin/env python3
"""Check diagnostic event format and freestanding source invariants."""
from pathlib import Path
import re
import unittest

ROOT = Path(__file__).resolve().parents[1]
class DiagnosticsTests(unittest.TestCase):
    def test_interface_is_defined(self):
        header = (ROOT / "boot/uefi/diagnostics.h").read_text()
        for api in ("aion_diag_write", "aion_diag_event", "aion_diag_hex"):
            self.assertIn(api, header)

    def test_both_arch_transports(self):
        source = (ROOT / "boot/uefi/diagnostics.c").read_text()
        self.assertIn("__x86_64__", source)
        self.assertIn("__aarch64__", source)
        self.assertIn("0x09000000UL", source)
        self.assertIn("0xe9", source)
        self.assertIn("AION_DIAG:", source)

    def test_no_hosted_runtime(self):
        source = (ROOT / "boot/uefi/diagnostics.c").read_text()
        self.assertIsNone(re.search(r"#include\s*<(stdio|stdlib|string)\.h>", source))

if __name__ == "__main__":
    unittest.main()
