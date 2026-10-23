<div align="center">

<img src="source/res/motorlab/logo.png" alt="Somboon Advance Technology — SAT Motor Pilot" width="480">

# SAT Motor Pilot

**พื้นที่ทำงานตั้งค่า ทดสอบ และตรวจข้อมูลมอเตอร์ของสมบูรณ์ บน Windows**

![Windows](https://img.shields.io/badge/Platform-Windows_x64-304B8A?style=for-the-badge)
![Qt](https://img.shields.io/badge/Qt-5.15.2-304B8A?style=for-the-badge&logo=qt)
![Version](https://img.shields.io/badge/Version-0.3.0-304B8A?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Engineering_Preview-245987?style=for-the-badge)
![License](https://img.shields.io/badge/Source-GPL--3.0--or--later-2563EB?style=for-the-badge)

[เริ่มใช้งาน](#เริ่มใช้งาน) · [Build](#build-จาก-source) · [หลักฐาน](#ผลทดสอบและหลักฐาน) · [รายการที่ยังต้องยืนยัน](docs/ACCEPTANCE_AND_KNOWN_ISSUES.md)

</div>

## โปรแกรมของสมบูรณ์

**SAT Motor Pilot 0.3.0** เปิดใช้งานเป็นโปรแกรมของตนเอง ไม่เปิดหรือเรียกใช้โปรแกรม VESC Tool และไม่ต้องติดตั้งโปรแกรมนั้นก่อน ใช้ชื่อไฟล์ `SATMotorPilot.exe`, Windows product/company metadata, AppUserModelID และ settings namespace ของตนเอง

เวอร์ชันนี้จัดหน้าตาและขั้นตอนใช้งานในแบบของสมบูรณ์: แถบชื่อบริษัทและสถานะการเชื่อมต่อด้านบน, เมนู Overview / Motor workspace / Input workspace / Measurement lab, แถบคำสั่งแนวนอน และหน้า Motor control lab ที่มีทางลัด 4 ขั้นตอน ควบคุมเกือบเหลี่ยมและเส้นแบ่งชัดเจนในสไตล์ LAB โลโก้ไม่มีพื้นขาว ธีมดำและเทาเข้มใช้สีดำเป็นหลักและสีน้ำเงินเฉพาะส่วนสำคัญ ครอบคลุมเมนู ปุ่ม ช่องกรอกข้อมูล และแถบชื่อหน้าต่างบน Windows ที่ทดสอบ

<img src="evidence/ui/motorlab/screenshots/page-000.png" alt="SAT Motor Pilot company workspace, disconnected" width="100%">

ส่วนสื่อสารและเครื่องมือตั้งค่าอาศัยโค้ด GPL จากรุ่นอ้างอิงเพื่อรักษาความเข้ากันได้กับ controller และ firmware จึงยังเก็บเครดิตผู้พัฒนาเดิมและไลเซนส์ใน About/เอกสาร การเป็นผลิตภัณฑ์ของสมบูรณ์ไม่ได้หมายความว่าอ้างว่าเขียนโค้ดต้นทางทั้งหมดขึ้นใหม่ ดู [ขอบเขตผลิตภัณฑ์และ compatibility core](docs/COMPANY_PRODUCT.md)

แผง **Manual drive** เปิดเฉพาะเมื่อผู้ใช้กด ปุ่มเปิด/ปิดแผงไม่ส่งคำสั่งมอเตอร์ เครื่องมือเดิมยังเข้าถึงได้จาก **Session tools** ส่วน **Output off** ใช้กับ controller ที่เลือกเท่านั้น

อัปเดตไฟล์เดิมใน `dist/SATMotorPilot/` และ ZIP ชื่อเดิม ไม่สร้างโฟลเดอร์แยกตามเวอร์ชัน

## ขั้นตอนทำงาน

```mermaid
flowchart LR
    A["SAT Workspace"] --> B["1. Connect & Identify"]
    B --> C["2. Read & Back Up"]
    C --> D["3. Configure Motor / Application"]
    D --> E["Confirm → Write → Readback"]
    E --> F["4. Live Measurements / Logs"]
```

หน้าเริ่มต้นแสดง **DISCONNECTED** เมื่อไม่มีอุปกรณ์ ไม่สร้างข้อมูล controller ขึ้นมาแทน เมื่อเชื่อมต่อจะแสดง port, firmware, UUID และ CAN target จากสถานะจริง ทางลัดหน้าแรกเป็นเพียงการเปิดหน้าทำงาน ไม่ส่งคำสั่งหมุนมอเตอร์หรือเขียนค่า

## ความสามารถ

| กลุ่ม | สิ่งที่มีในโปรแกรม | สถานะหลักฐาน |
|:--|:--|:--|
| SAT console | แถบบริษัท, target status, ทางลัด 4 ขั้น, Manual drive ที่พับเก็บได้ | ทดสอบการนำทางจริงใน Windows |
| เริ่มใช้งานครั้งแรก | บทนำ SAT, ข้อกำหนดการใช้, warranty, ขั้นตอนเริ่มงาน | ตรวจ 4 หน้า; Usage/Warranty ยังต้องยอมรับก่อนผ่าน |
| คู่มือและแอป | SAT quick start, project/builds, issues, changelog, About | ไม่มีการตรวจอัปเดตแอปต้นฉบับอัตโนมัติ |
| Motor/App configuration | General, BLDC, DC, FOC, GPDrive, PID, PPM, ADC, UART, NRF, PAS, IMU | สืบทอด engine/schema; รอ controller acceptance |
| Transport | Serial/USB, BLE, TCP/UDP และ CAN forwarding | คงกลไกเดิม; รอทดสอบแต่ละอุปกรณ์/transport |
| Telemetry/analysis | Realtime, sampled data, experiments, BMS, IMU, log analysis | ตรวจ disconnected UI; live readings ยังรอยืนยัน |
| Engineering tools | Terminal, LispBM, QML, SWD, ESP programmer, packages | มี implementation; รายฟังก์ชันยังมี acceptance ค้าง |
| Toolbar Write | อ่านค่าปัจจุบัน → backup → ยืนยัน → ACK → fresh readback | มีในโค้ด; ยังไม่ส่ง write ไปอุปกรณ์จริง |

Direct PC CAN adapter ต้องใช้ build/backend ที่เปิด `HAS_CANBUS`; build default นี้ไม่ได้เปิด จึงไม่ใช้การมี CAN forwarding มาอ้างว่าทดสอบ PC CAN adapter แล้ว Firmware/package downloads เป็นข้อมูล compatibility เมื่อเรียกฟังก์ชันนั้น ไม่ใช่การติดตั้งหรือเปิดใช้แอปต้นฉบับ ไม่มีการ flash firmware อัตโนมัติในชุดทดสอบ

## เริ่มใช้งาน

ชุดทดลองใน workspace: `SAT-Motor-Pilot-Windows-x64.zip`

1. แตก ZIP ทั้งโฟลเดอร์
2. เปิด `SATMotorPilot/SATMotorPilot.exe` โดยเก็บ DLL, `platforms` และ `qml` ไว้ด้วยกัน
3. ทำตามการเริ่มใช้งานครั้งแรก แล้วเปิด **Device link** จาก Overview
4. ตรวจอุปกรณ์และ firmware จากนั้นเริ่มอ่าน configuration ก่อนเปลี่ยนค่า

ไฟล์หลัง build: `dist/SATMotorPilot/SATMotorPilot.exe` ใช้ runtime Qt ที่มากับแพ็กเกจ รุ่นนี้ยังเป็น engineering preview และมี resource/dependency review ก่อน public binary release

## Build จาก source

ใช้ Git, Python 3, PowerShell และ Qt 5.15.2 / MinGW 8.1 x64 ให้ตรงกัน เพราะ engine ใช้ Qt private headers

```powershell
git clone https://github.com/poommyroboticcamera-netizen/SAT-Motor-Pilot.git
cd SAT-Motor-Pilot
./bootstrap.ps1
./build.ps1 -Clean
```

`bootstrap.ps1` ติดตั้ง toolchain ใน `tooling/` ของโปรเจกต์ ไม่ต้องติดตั้งแอปอ้างอิง ระบุ Qt ที่มีอยู่ด้วย `./build.ps1 -QtRoot C:/path/to/Qt`

โปรเจกต์: `source/sat_motor_pilot.pro` ส่วน `source/vesc_tool.pro` เป็นรายการ source/dependencies ของ compatibility engine ที่เก็บเพื่อรักษาสถาปัตยกรรมเดิม

## ผลทดสอบและหลักฐาน

| การตรวจ | ขอบเขตและผล | หลักฐาน |
|:--|:--|:--|
| Windows launch | เปิดจาก runtime ในแพ็กเกจ โดย PATH เฉพาะ Windows system | [Launch](evidence/ui/motorlab/launch.json), [Runtime](evidence/ui/motorlab/runtime-audit.json) |
| SAT navigation | ปุ่มหน้าแรก 4 ปุ่มไปยังหน้าที่ถูกต้อง | `workspaceNavigation` ใน runtime report |
| Company shell | เปิด/ปิด Manual drive ไม่ส่ง command; Windows dark title bar ผ่านบนเครื่องทดสอบ | `manualPanel`, `darkTitlebarApplied` ใน runtime report |
| Onboarding | 4 หน้า; required acceptance ยังคงอยู่ | `onboardingPages` และ [ภาพ](evidence/ui/motorlab/onboarding) |
| Packet/scalar | **11 passed, 0 failed** รวม init/cleanup; 9 test methods | [ผลข้อความ](validation/results/protocol-tests.txt), [JUnit](validation/results/protocol-tests.junit.xml) |
| Compatibility regression | **88 checks** ของ command/packet/serializer/schema และ transport functions | [Core regression](evidence/company-core-regression.json) |
| XML persistence | motor 211 params / 488 bytes, app 122 params / 284 bytes; เปลี่ยน destination ก่อน load แล้วค่ากลับมาครบ | `configurationRoundtrips` ใน runtime report |
| Disconnected rendering | 33 navigation rows; ไม่ได้แปลว่าทุก control ผ่าน | Runtime และ [traceability](validation/ui-function-traceability.csv) |
| REA / ต้นทาง | ไฟล์แอปจริง, package inventory, hash, provider limitations | [REA findings](docs/REA_FINDINGS.md), [reference](docs/REFERENCE_AND_COMPATIBILITY.md) |

Core regression normalize CRLF และยอมให้เปลี่ยนชื่อใน audit diagnostic เท่านั้น ไม่ใช้ source comparison อ้างว่าผ่านฮาร์ดแวร์

```powershell
./validation/run-protocol-tests.ps1
python validation/verify_compatibility_core.py
./validation/run-ui-audit.ps1
```

Audit ใช้ settings แยก บล็อก physical outbound packets และไม่กดคำสั่งมอเตอร์/เขียนค่า/flash **ยังไม่รับรองความเทียบเท่าฮาร์ดแวร์ทุกชุดหรือทุก workflow** ต้องเพิ่มหลักฐาน controller read/write, detection, firmware flashing, BLE/CAN adapter, reconnect และ live telemetry ดู [Acceptance](docs/ACCEPTANCE_AND_KNOWN_ISSUES.md) และ [Toolbar Write](docs/CONFIGURATION_WRITE_SAFETY.md)

## แหล่งอ้างอิงและไลเซนส์

Compatibility reference: VESC Tool stable **7.00**, commit `01d5f10901116c311e3fb84d5a1541f663d3ce20`; firmware reference: bldc 7.00, commit `26af69484014b6771d02e68c22f4cb96645b5f22`

เก็บ copyright ของ Benjamin Vedder และผู้ร่วมพัฒนา รวมทั้ง [GPL-3.0-or-later](LICENSE) และ [THIRD_PARTY_NOTICES](THIRD_PARTY_NOTICES.md) ไม่อ้างการรับรองจากเจ้าของ engine เดิม โลโก้ Somboon Advance Technology เป็นไฟล์ที่ผู้ใช้จัดส่ง ไม่อ้างว่าเป็นงาน GPL ที่สร้างใหม่
