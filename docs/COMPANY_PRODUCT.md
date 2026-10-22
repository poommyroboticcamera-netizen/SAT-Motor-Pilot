# SAT Motor Pilot product boundary — 2026-10-23

The company application is SAT Motor Pilot. MotorLab 0.1 was a preliminary rebrand and is superseded by 0.2.0.

- Company executable `SATMotorPilot.exe`, product/company/version metadata and taskbar identity `Somboon.SATMotorPilot`.
- Qt organization `SomboonAdvanceTechnology`, domain `satmotorpilot.local`, application `SAT Motor Pilot`; settings and backups separate from both preliminary MotorLab and the reference application.
- SAT Workspace commissioning dashboard, current target status, four real navigation buttons and retained expert tools.
- SAT first-run intro/conclusion, quick-start manual, company changelog, project/build and issue links.
- Presentation translation and parameter/help metadata adaptation only; no translation of command IDs, namespaces, wire data or saved values.
- Automatic reference-application updater removed from desktop/board-setup startup. The retained QML API is a local information dialog with no network request.
- User-supplied Somboon artwork and blue #304B8A.

The executable is built from source and runs using its packaged Qt DLLs. It does not shell-launch, automate, wrap or require the reference executable.

The GPL-derived compatibility engine retains command/packet/scalar/config serializers, schemas, transports and advanced tools. This is a derivative company product, not a clean-room rewrite or claim of exclusive authorship over upstream code. Source notices, About credits and license terms remain intact.

The 88-item regression normalizes source line endings and permits the audit diagnostic name change; protocol/scalar tests and XML round trips supply additional local evidence. These are not hardware acceptance tests.

Controller firmware/model names, protocol namespaces and interoperable formats can retain real names for accurate target identification. Firmware/package data sources remain compatible; no company backend or certification is fabricated.

The Windows audit tests actual navigation, first-run pages and acceptance state, local XML persistence and disconnected rendering. It blocks physical packets. Live hardware, every wizard/dialog and the full parity matrix remain open in ACCEPTANCE_AND_KNOWN_ISSUES.md.
