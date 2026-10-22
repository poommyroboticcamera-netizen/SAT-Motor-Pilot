# Validation evidence and remaining acceptance work

Pinned source: upstream stable VESC Tool 7.00 commit `01d5f10901116c311e3fb84d5a1541f663d3ce20`.

## What these inventories prove

`ui-function-traceability.csv` records every `.ui` and `.qml` resource recursively, named Qt widgets/actions/items, visible QML string properties, application C++ entry points in pages/widgets/wizards and main/editor windows, and navigation/wizard construction sites. It includes mobile resources, example QML, and vendored editor forms; the source path identifies those scopes. It is a source inventory, not a statement that every item is reachable in this Windows build. Dynamic firmware parameters are enumerated separately for every supplied XML version in `parameter-traceability.csv`; `command-traceability.csv` enumerates the COMM command IDs. Conditional compilation, runtime firmware capabilities, dynamically built dialogs, strings outside the scanned patterns, and third-party internals can require additional runtime coverage. Line numbers are navigation hints, not runtime coverage.

Every runtime cell starts **PENDING**. Keeping upstream implementation does not establish tested equivalence. The source and generated inventories are reproducible with `generate_inventory.py` and `generate_parameter_inventory.py` using Python 3 standard library only.

## Executable protocol checks

`protocol-tests.pro` compiles the original `../source/packet.cpp` and `../source/vbytearray.cpp` with QtTest. No copied replacement parser or simulated UI implements the system under test. Tests cover CRC, fixed outbound wire bytes, length boundaries 1/255/256/10000, rejection of empty/oversize outbound data, every split of a representative binary frame, a 10000-byte frame one byte at a time, concatenated frames, invalid CRC/stop/length, reset after partial input, basic noise resynchronization, signed big-endian integers, fixed point, IEEE 1.0 encoding and non-destructive insufficient numeric input.

Expected vectors were authored independently of the production table: CRC uses bitwise polynomial 0x1021, initial 0, no reflection, xorout 0 (CRC-16/XMODEM); `123456789` check = 0x31c3. The fixed complete frame is `02 09 31 32 33 34 35 36 37 38 39 31 c3 03`. The serializer vectors use direct big-endian arithmetic and IEEE-754 binary32 1.0 = 0x3f800000. This is not a firmware-captured golden corpus. CRC-corrupted frames are explicitly reset before a fresh frame because arbitrary payloads may contain valid-looking incomplete starts; only simple noise resynchronization is asserted. Automatic recovery from every malformed sequence is not claimed.

Build in a separate directory with the supplied Qt 5.15.2 MinGW toolchain:

```powershell
$env:PATH = '<Qt>/Tools/mingw810_64/bin;<Qt>/5.15.2/mingw81_64/bin;' + $env:PATH
New-Item -ItemType Directory -Force validation/build | Out-Null
Set-Location validation/build
& '<Qt>/5.15.2/mingw81_64/bin/qmake.exe' ../protocol-tests.pro
mingw32-make.exe -j4
./release/protocol-tests.exe -o results.xml,junitxml -o results.txt,txt
```

## Acceptance gaps requiring explicit evidence

1. Windows reference and MotorLab screenshots at identical scale, font, theme and size for each reachable page, tab, menu, context menu, modal and wizard stage. Include validation errors, empty/offline and connected states. Automated inventory discovery is not screenshot equivalence.
2. Record command/reply bytes and settings read/write round trips for serial, TCP, UDP, BLE and CAN forwarding as applicable. Exercise disconnect, timeout, bad CRC, truncated packets, incompatible firmware, cancellation and reconnect. Current tests do not instantiate Commands, VescInterface or a controller.
3. Use recorded telemetry to compare units, scaling, graphs, sample/export formats and log playback. Test file open/save/import/export, configuration backup/restore, preferences persistence, scripts and package operations.
4. Firmware upload, motor detection, FOC/Hall/encoder calibration, controller commands and physical faults require an authorized appropriate test rig. They have not been validated by offline unit tests; no hardware action is part of these tests.
5. Build an installable package, test clean Windows installation, first launch, paths with non-ASCII text, removal, and license/source notices. Runtime smoke tests are distinct from packaging acceptance.
6. Mark each inventory row with actual evidence path, result and any limitation after exercise. Pending items must remain pending and cannot be promoted merely from source reuse.


## Executed result (2026-10-22, Windows)

The tests were built with supplied Qt 5.15.2 MinGW 8.1 64-bit and executed on Windows: **11 passed, 0 failed, 0 skipped**, including QtTest initialization and cleanup (nine substantive test methods). Raw evidence: `build/results.txt` and `build/results.xml`. This result covers original packet and scalar serializer implementation only. UI/command/parameter/hardware runtime inventory entries remain PENDING.

Reproduce with `./run-protocol-tests.ps1` from this directory, or invoke its full path from any directory. An initial attempt while the compiler archive was still extracting failed for missing system header; the completed toolchain build and execution above passed.

Retained copies for repository publication are `results/protocol-tests.txt` and `results/protocol-tests.junit.xml` (identical to the executed build output). The build directory itself may be excluded from version control.

## Later runtime evidence

A disconnected application audit now records 33 visible page rows and production ConfigParams local XML round trips (motor: 211 parameters, 488 serialized bytes; application: 122 parameters, 284 bytes). Destination values were deliberately perturbed before load, yielding 197/117 differences respectively before load and zero afterward, with identical serialized bytes. These are local file persistence checks, not physical controller read/write. See `../evidence/ui/motorlab/runtime-audit.json` and `../docs/ACCEPTANCE_AND_KNOWN_ISSUES.md`.

`update_runtime_inventory.py` updates only form rows matched by runtime widget class with saved screenshots to RENDERED_ONLY; all child controls/functions remain PENDING. The first audit has blank titles; class/source matching is retained separately in `rendered-page-evidence.csv`. Re-run this updater after regenerating source inventory or native audit. The root worker is preparing a later native Windows MotorLab audit; the first MotorLab captures used offscreen rendering.
