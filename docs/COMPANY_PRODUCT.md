# SAT Motor Pilot product boundary — 2026-10-23

The company application is SAT Motor Pilot. MotorLab 0.1 was a preliminary rebrand and is superseded by 0.3.0.

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

## 0.3.0 presentation and in-place delivery

The black SAT laboratory shell replaces the old vertical icon toolbar and permanent drive strip with a horizontal configuration bar, session menu and collapsed manual-drive drawer. Existing QAction objects and original drive slots are reused. Navigation labels are company-specific; stable internal page names remain unchanged so routing continues to reach the same working pages. A top company bar shows real connection state without polling hardware through new commands.

Black and charcoal surfaces, square controls, typography, selected navigation accents and transparent logo derivatives replace the white logo panels and grey shell. The unchanged user-supplied image remains in source/res/motorlab/somboon-user-supplied.png. The transparent master in app-icon.png was derived using image editing; all active raster assets are regenerated at their existing paths. The company emblem is framed with QPixmap at runtime.

On the tested Windows host, the native dark title-bar attribute succeeds. The implementation dynamically loads DwmSetWindowAttribute and tolerates its absence. Reference: [Microsoft desktop themes](https://learn.microsoft.com/windows/apps/desktop/modernize/apply-windows-themes), [DWM attributes](https://learn.microsoft.com/windows/win32/api/dwmapi/ne-dwmapi-dwmwindowattribute).

Build, portable folder and ZIP reuse their existing paths. The audit verifies drawer visibility and zero emitted commands when toggling it. That check does not exercise physical drive outputs. Updated screenshot comparison records actual unequal window sizes without scaling; pixel identity with the original shell is not a target for this version.
