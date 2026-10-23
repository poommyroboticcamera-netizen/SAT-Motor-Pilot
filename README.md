**Language:** English · [ไทย](README_TH.md)

In the Windows app, choose **Help → Language / ภาษา → ไทย**. Restart the app to apply the selection. English is the initial setting; some technical text still appears in English in Thai mode.

<div align="center">

<img src="source/res/motorlab/logo.png" alt="Somboon Advance Technology — SAT Motor Pilot" width="460">

# SAT Motor Pilot

**Complete motor setup and monitoring tool for Windows**

![Windows](https://img.shields.io/badge/Windows-x64-30343B?style=for-the-badge)
![Version](https://img.shields.io/badge/Version-0.3.1-304B8A?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Engineering_Preview-30343B?style=for-the-badge)
![License](https://img.shields.io/badge/License-GPL--3.0--or--later-304B8A?style=for-the-badge)

[Download V0.3.1](https://github.com/poommyroboticcamera-netizen/SAT-Motor-Pilot/releases/latest) · [Get started](#get-started) · [ability](#capabilities) · [Test results](#test-results) · [Build from source](#build-from-source)

</div>

## About the project

**SAT Motor Pilot** is an app for connecting to a controller, reading and adjusting motor values, checking signals, and analyzing operating data. Suitable for system preparation work, testing on a LAB table, and checking settings before use.

The app has its own name, screen, and instructions for use. It can be activated without installing or opening the VESC Tool. The communication system is developed based on GPL code to maintain compatibility with boards using the VESC protocol.

<img src="evidence/ui/motorlab/screenshots/page-000.png" alt="SAT Motor Pilot: black laboratory interface with Somboon branding" width="100%">

[Watch the app opening animation](evidence/ui/motorlab/startup.mp4)

### Screen format

- **Mainly black and dark gray** Use company blue to highlight selected buttons and statuses.
- **Bold, narrow title font** Supports Muffie with installed or licensed files. This set uses Impact as an alternative; Settings text and numbers are still legible.
- **LAB Tool Style** Almost square corners. clear dividing line and the command is at the top
- **App opening animation** The complete logo slowly appears on a black background. With a line indicating the start of work approximately 1.6 seconds before entering the main page.
- **Transparent logo** blends in with the background. There is no white slab.
- **Manual drive foldable** Open only when manual control is required. Opening the panel does not send motor commands.

## Work steps

```mermaid
flowchart LR
    A["1. เชื่อมต่อandตรวจอุปกรณ์"] --> B["2. อ่านค่าandสำรองข้อมูล"]
    B --> C["3. ตั้งค่ามอเตอร์andอินพุต"]
    C --> D["4. ตรวจสัญญาณandผลการทำงาน"]
```

| work area | used for |
|:--|:--|
| **Overview / Motor control lab** | Start a job, view the status, and open required setup steps. |
| **Device link** | Select connection, check port and firmware information. |
| **Motor workspace** | Motor settings, control methods, limits and PID |
| **Input workspace** | Set up UART, PPM, ADC, remote and other inputs. |
| **Measurement lab** | Open live scope, sampled data and analysis tools |
| **Session tools** | Access additional controller commands and tools. |

The status on the screen comes from the actual connection. When the device is not connected, it will show **NO DEVICE SESSION / DISCONNECTED**

## Capabilities

| group | Scope available in the app |
|:--|:--|
| connection | Serial/USB, BLE, TCP/UDP and CAN forwarding |
| Motor settings | BLDC, DC, FOC, GPDrive, PID and parameters according to firmware |
| Input settings | PPM, ADC, UART, NRF, PAS, IMU and remote control |
| Data and Analysis | Realtime data, sampled data, experiments, BMS, IMU and log analysis |
| engineering tools | Terminal, LispBM, QML, SWD, ESP programmer and controller packages |
| Settings file | Read/save XML and backup configuration |

Just having a tool in an app doesn't mean it's been tested on every hardware. This model is also **Engineering Preview** View [Certification status and limitations](docs/ACCEPTANCE_AND_KNOWN_ISSUES.md)

## Get started

download **`SAT-Motor-Pilot-Windows-x64.zip`** from [GitHub Releases](https://github.com/poommyroboticcamera-netizen/SAT-Motor-Pilot/releases/latest) Latest version **V0.3.1**

1. Extract the entire ZIP folder and open it. `SATMotorPilot/SATMotorPilot.exe`
2. Store DLLs and folders `platforms`, `qml` next to the app file as before
3. Follow the instructions for the first time and open **Device link**
4. When the controller is detected, check the firmware version and read the values ​​before modifying.

### Connect the VESC board

The app maintains the communication system and checks the firmware of the compatibility core, so it is designed to connect to the VESC board. Start by supplying power according to the board's specifications, connect the USB cable that can transmit data, select the port in **Device link** and connect, then check the firmware and use **Read motor / Read inputs** to read the values.

Compatibility depends on the board and installed firmware. The reference version of the project is **7.00**. If the app reports that the firmware is not supported, check the information to make sure it matches before using the command to write the motor check values ​​or flash the firmware. [Compatibility scope](docs/REFERENCE_AND_COMPATIBILITY.md)

The **Write motor / Write inputs** button on the main bar provides steps to read current values, backup data, list changes, request confirmation, and read back. The **Output off** section applies to the selected controller, not an emergency stop, for all devices on the system.

### Update original file

The build file is at `dist/SATMotorPilot/SATMotorPilot.exe` and ZIP uses the same name every time. Close the app before overwriting the original folder. No need to separate folders by version number.

## Test results

The following results are a test of programs and files on Windows. **The actual controller has not been connected**

| examination | Latest results |
|:--|:--|
| Build and open the app | Open with the runtime that comes with the package, without relying on Qt from the machine's PATH. |
| SAT user interface | **14 runtime checks passed**, 0 failed and 1 source review |
| Home page navigation | All 4 shortcuts open the correct work page. |
| Manual drive | Can be opened/closed, folded from the start and does not send commands from opening the panel. |
| Packet and data conversion | **11 passed, 0 failed** total init/cleanup; There are 9 test methods |
| Compatibility core | **88/88 checks** in command, packet, serializer, schema and transport areas checked |
| Save/Load XML | Motor 211 parameters and application 122 parameters return the correct value after loading. |
| screen display | Save 33 images in a disconnected state. |

evidence: [Runtime audit](evidence/ui/motorlab/runtime-audit.json) · [SAT test results](evidence/company-product-acceptance.json) · [Protocol tests](validation/results/protocol-tests.txt) · [Core regression](evidence/company-core-regression.json) · [Delivery manifest](evidence/delivery-manifest.json)

Tasks that still need to be verified on real devices include controller read/write, motor detection, motor driving, live telemetry, reconnect, BLE/CAN and firmware flashing. The above results do not guarantee compatibility with all boards. Audit blocks hardware output packets.

Direct PC CAN adapter requires an open build. `HAS_CANBUS` This default set is not turned on. As for CAN forwarding, it is a communication through the connected controller.

## Build from source

Use **Windows x64, Git, Python 3, PowerShell and Qt 5.15.2 / MinGW 8.1 x64** to match the project.

```powershell
git clone https://github.com/poommyroboticcamera-netizen/SAT-Motor-Pilot.git
cd SAT-Motor-Pilot
./bootstrap.ps1
./build.ps1 -Clean
```

`bootstrap.ps1` Prepare the toolchain in `tooling/` of the project. If Qt already exists, specify the location as well. `./build.ps1 -QtRoot C:/path/to/Qt`

```powershell
./validation/run-protocol-tests.ps1
python validation/verify_compatibility_core.py
./validation/run-ui-audit.ps1
python validation/record_company_acceptance.py
```

Main project: `source/sat_motor_pilot.pro` result: `dist/SATMotorPilot/`

## Additional documents

- [Somboon's product scope](docs/COMPANY_PRODUCT.md)
- [Certification status and limitations](docs/ACCEPTANCE_AND_KNOWN_ISSUES.md)
- [Steps to confirm writing values](docs/CONFIGURATION_WRITE_SAFETY.md)
- [References and compatibility](docs/REFERENCE_AND_COMPATIBILITY.md)
- [Evidence of app file inspection with REA](docs/REA_FINDINGS.md)
- [Change history](source/res/sat_changelog.md)

## Source and license

SAT Motor Pilot is developed from VESC Tool stable **7.00** by Benjamin Vedder and co-developers under **GPL-3.0-or-later**, keeping the original credits and requirements. This project does not claim to be written. All new compatibility engine or has been certified by the original project owner

Source reference: `01d5f10901116c311e3fb84d5a1541f663d3ce20` · Firmware reference: `26af69484014b6771d02e68c22f4cb96645b5f22`

The Somboon Advance Technology logo is a user submitted image. This version uses a modified image with a transparent background and keeps the original. See. [LICENSE](LICENSE) and [THIRD_PARTY_NOTICES](THIRD_PARTY_NOTICES.md) for credit and items resource/dependency that still need to be checked
