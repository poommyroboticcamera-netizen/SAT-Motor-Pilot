# REA evidence ledger

## SAT Motor Pilot 0.3.0, 2026-10-23

REA inspected the final `dist/SATMotorPilot/SATMotorPilot.exe` using the portable native PE resource parser. Evidence `ev_3fea6f40783b48fc279398d46ceb96c44df37dcb3ca3683d63ebe7a9f7e5d289` establishes PE32+, x86-64, 22,180,352 bytes and SHA-256 `e60a51a39223146287b6558a7459c7f9238503e22bb5c8a20a0ef62ef1c0e387`. It completed 19 resource entries / 8 resources. Full records are in `evidence/rea-company-executable.json`; the target SHA identifies the final record among retained observations.

The Windows version-resource probe reports product SAT Motor Pilot, company Somboon Advance Technology and version 0.3.0.0. Supported version/About probes exit successfully with system-only PATH. The executable reports build source `51d1dd0d` and retains original source attribution. See `evidence/sat-executable-identity.json`, `sat-version.txt`, and `sat-about.txt`.

REA inspected the overwritten trial ZIP, SHA-256 `8097db5df220351dd8e4ec8d7185587b8979b9e85370b1732a93566a142c407a`. Inspection `ev_3a083b06a229cd012c2ade0dee83d15151590e39796e94f1cfe8534637bba0cc` contains 1,168 file occurrences, 1,159 content nodes and zero integrity contradictions within the bounded central-directory inventory. Full records are in `evidence/rea-company-package.json`. The package session was closed after export.

The native Windows audit also runs the overwritten installation at `C:/Users/poomc/Downloads/SATMotorPilot/SATMotorPilot.exe` and reaches all 33 visible navigation rows. It captures actual changing startup frames, checks the early logo reveal and transitions to the working window. Current display font is `Impact`, an explicit fallback because licensed Muffie is not present. Startup frames/video and runtime results are under `evidence/ui/motorlab/`.

Static artifact observations do not establish hardware behavior. Runtime evidence comes from instrumented Qt, not a REA native UI provider. No native decompilation was needed for the available source. Earlier provider limitations below remain historical reference.

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
