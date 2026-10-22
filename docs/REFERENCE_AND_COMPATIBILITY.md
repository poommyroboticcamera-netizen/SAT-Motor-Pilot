# Reference and compatibility boundary

Checked on 22 October 2026 (Asia/Bangkok).

## Selected stable source

MotorLab's stable reference is VESC Tool **7.00**, upstream commit **01d5f10901116c311e3fb84d5a1541f663d3ce20**, authored 2026-05-15 15:57:49 UTC with message “Release 7.00”. The exact project file declares `VT_VERSION = 7.00`, `VT_IS_TEST_VERSION = 0`, and configuration version 4. The source changelog dates 7.00 to 2026-05-15. The later 7.01 project version has a nonzero test flag and a TBD changelog; its version transition is explicitly named “Beta 7.01”. Therefore repository HEAD is not a stable release reference.

The GitHub releases endpoint returned an empty array, and `git ls-remote --tags` returned no tags. This project pins the release commit, not an invented release tag. The public official download landing page did not expose a downloadable version identifier in the retrieved page. Selection therefore rests on upstream release source evidence; it is not proof that this is the newest authenticated binary available behind a website account.

Sources: [release commit](https://github.com/vedderb/vesc_tool/commit/01d5f10901116c311e3fb84d5a1541f663d3ce20), [pinned project file](https://github.com/vedderb/vesc_tool/blob/01d5f10901116c311e3fb84d5a1541f663d3ce20/vesc_tool.pro), [official project page](https://vesc-project.com/vesc_tool). The local manifest and version-history evidence record these identifiers.

## Installed binary is a separate reference

The existing `C:\Users\poomc\Downloads\VESC_Tool-win-Qt6\VESC_Tool.exe` is 20,027,904 bytes and has SHA-256 `9ecfb464d5284af1b85139025e122a7404757729416998d0fa652930fbef300a` (root worker's PE inspection). Windows file-version and product-version fields are empty. Its adjacent Qt6Core.dll reports 6.8.3.0; `installed-dll-versions.json` records the adjacent DLL versions.

An early CLI probe using `--debugOutFile <evidence-file> --version` exited 0 and reported **7.00-0+255006e8**. Windows stdout/stderr were empty; Qt diagnostic output was captured through the app's supported debug-file option. `installed-version-probe.json` records the command, exit and unchanged binary hash, and `installed-version-output.txt` contains the result. Environment data directories were redirected; native QSettings may read the existing darkMode value before parsing the CLI. No GUI or VescInterface was requested and no hardware command was passed. The reported short commit differs from the selected source release commit. Version agreement therefore does not establish full feature or pixel equivalence, nor cryptographic official download provenance.

REA reported no open native analysis session during this worker's check. Native Ghidra analysis was unavailable because GHIDRA_INSTALL_DIR was not configured; Java probing also reported an absolute-path issue. The runtime action was limited to the early CLI version probe above. This worker did not change app settings, connect hardware, or attempt decompilation. The full capability response is retained in `evidence/rea-reference-session.json`; the root worker exported separate PE evidence to `evidence/rea-evidence.json`.

GitHub resolves the reported short commit to **255006e8f55a37ea8539d4f0462dd290daf23454**, dated 2026-07-01 21:15:46 UTC, message “Merge branch 'vedderb:master' into qt6-port”. This identifies a Qt6-port source lineage distinct from the selected release source. The upstream resolution is retained in `installed-reported-source-commit.json`; a self-reported commit is not a reproducible-build attestation.

## Firmware and transports

The pinned source includes configuration schemas named 3.55 through 3.66, 4.00–4.02, 5.00–5.03, 6.00, 6.02, 6.05, 6.06 and 7.00. These are schema inventories, not a blanket support certification. Firmware handshake, XML selection, command support and warnings must continue to use upstream negotiation. No connected controller model or device firmware hash was established in this investigation.

The matching upstream firmware source reference is [bldc release 7.00, commit 26af69484014b6771d02e68c22f4cb96645b5f22](https://github.com/vedderb/bldc/commit/26af69484014b6771d02e68c22f4cb96645b5f22), authored 2026-05-15 15:55:57 UTC with message “Release 7.00”. Its `conf_general.h` declares major 7, minor 00, and test number 0. This pairs the source releases by their explicit version declarations and release commits; it does not establish that a downloaded firmware payload or the existing device uses that commit. Retained evidence includes the exact header and version history.

Upstream 7.00 defaults to `CONFIG += exclude_fw`; its changelog says firmware is downloaded on demand. The build must report whether it retains this flag. Shipping the app does not by itself ship all firmware images or prove firmware download/flash success. A firmware payload must be separately identified before any hardware flashing test.

Serial and Bluetooth build support are enabled in the desktop project; TCP/UDP behavior is in VescInterface. Direct native CAN backend is guarded by `HAS_CANBUS`, commented out in the default project. CAN forwarding through a connected VESC and direct PC CAN are different capabilities. Test and report them separately. A build with that flag disabled must not claim a tested PC CAN backend.

## Evidence still required for a parity claim

Source inheritance establishes architectural continuity, not complete UI or hardware parity. Release acceptance still requires: an identified original binary, matching window size/DPI/theme/data state, every page/dialog/wizard inventory, side-by-side captures, protocol fixture results, corrupt/partial-packet and timeout tests, firmware negotiation cases, config/log round trips, and tests on the actual target hardware. Record pass/fail/not-run individually. Offline tests cannot certify motor detection, live telemetry, BLE radio behavior, CAN hardware, firmware flashing, emergency stopping, or electrical safety.

Any retained feature awaiting hardware verification must be described as inherited and unverified, not successful. Do not silently replace real device data with simulated data. Do not write a success message before the corresponding upstream acknowledgement or verified result.
