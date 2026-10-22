# REA evidence ledger

## SAT company application, 2026-10-23

REA inspected the actual newly built `dist/SATMotorPilot/SATMotorPilot.exe` using the native PE resource parser. Evidence `ev_fefdf24435ebe9f8d62fef43a2f366fea4dfaecb2b63724d9b883015e216ad5f` establishes PE32+, x86-64, 21,215,744 bytes, SHA-256 `0a0b23164a5601ded2063fe563fd76b22ec1cc2360d3b70c3238cf9e267eee19`. It completed 19 resource entries / 8 resources. Complete PE evidence is in `evidence/rea-company-executable.json`.

The Windows version-resource probe independently reports product SAT Motor Pilot, company Somboon Advance Technology, version 0.2.0.0. The supported CLI version/About probes exit successfully using only the packaged runtime and system PATH, and preserve original source attribution. See `sat-executable-identity.json`, `sat-version.txt`, and `sat-about.txt`.

REA also inspected the actual company trial ZIP: SHA-256 `c557d19056dc0c4a60f797a4e28743485cc765fa035d85124a9f11e1529602e9`; inspection `ev_b8f58e8ce274e4f2446338c21d542eb3052847e0373fdca1ea07fbc84093aa2a`. The inventory contains 1,173 occurrences (1,168 files, 5 directories), 1,164 content nodes and zero integrity contradictions within the ZIP inventory scope. Complete records are in `evidence/rea-company-package.json`. The opened package session was closed successfully after export.

These are static artifact observations, not native call tracing or proof of hardware behavior. UI/navigation/first-run evidence comes from the explicitly instrumented Windows runtime audit, not a REA native UI capture. No native decompilation was needed to read the complete available source. The earlier provider limitations below are retained as historical reference.

---

Investigation date: 2026-10-22. Skill: `reverse-engineer-anything`, version 42. REA MCP server: 6.7.0.

| Finding | Evidence | Scope / limitation |
|---|---|---|
| Installed executable is x86-64 PE32+, 20,027,904 bytes, SHA256 `9ecfb464d5284af1b85139025e122a7404757729416998d0fa652930fbef300a` | `rea-evidence.json`, `ev_34b1584a49b126ebf547e37e82a43e4cd779ca2b08e49086130deb7b9906d16c` | Static PE resources; one manifest resource, no icon groups or version resource. Does not establish UI or protocol behavior. |
| Local distributed ZIP SHA256 `f3d9e3fd05f0aefe599638060a0039bfc3647f9a74c7a2f1a478830d7d070a88` | `rea-package-evidence.json`, inspection `ev_e5d3b99d4a6d8474ba051dcd838a610ea9c2dd67c6c10e0e9821aa4d8116d678` | 1,503 entry occurrences: 1,489 files and 14 directories; 1,043 content nodes. Zero integrity contradictions in bounded ZIP inventory. Does not inspect non-entry bytes or execute the package. |
| Binary reports `7.00-0+255006e8`, adjacent Qt runtime reports 6.8.3 | `installed-version-probe.json`, `installed-version-output.txt`, `installed-dll-versions.json` | CLI version probe through normal development tools, separately from REA. No controller connection requested. Reported commit is a Qt6-port merge and differs from the selected Qt5 stable release source. |
| Native decompilation unavailable | `rea-reference-session.json` | Ghidra installation is not configured. Java probe also reports an executable-path issue. No pseudocode, disassembly, native call trace, or official-binary UI capture is claimed. REA native UI observation is unsupported on this Windows host. |

The REA export preserves complete records and locations. The package analysis session was closed after exporting. Attempting an optional analysis snapshot returned `evidence_integrity_mismatch` because archive snapshots require a concrete provider analysis profile; the evidence bundle export succeeded, and a subsequent plain `close_binary` succeeded. No snapshot is claimed.

The UI baseline in `evidence/ui/reference` is a source-built, instrumented pinned release, not the installed official Qt6 binary. Its shared outbound-transport guard prevents audit packet transmission. Source reads, builds and test harnesses use ordinary development tools; REA was not used to replace repository reading.
