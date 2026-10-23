## SAT Motor Pilot V0.3.1

แอปตั้งค่าและตรวจสอบระบบมอเตอร์สำหรับ Windows โดย Somboon Advance Technology

### สิ่งที่มีในรุ่นนี้

- หน้าจอสีดำ–เทาเข้ม ใช้สีน้ำเงินบริษัทเน้นส่วนสำคัญ และปุ่มเกือบเหลี่ยมในสไตล์ LAB
- แอนิเมชันโลโก้สมบูรณ์ตอนเปิดแอปประมาณ 1.6 วินาที
- พื้นที่ Motor workspace, Input workspace และ Measurement lab พร้อมทางลัดเริ่มงาน
- คำสั่งเดิมทั้ง 16 รายการเข้าถึงได้ผ่านแถบคำสั่งและ Session tools
- รองรับ Muffie ที่ติดตั้งหรือมีไฟล์สิทธิ์ใช้งาน; หากไม่มี ใช้ Impact หรือ Segoe UI สำรอง ไม่มีไฟล์ Muffie แนบในชุด
- เลขเวอร์ชันในไฟล์ Windows, About และหน้าเปิดแอปตรงกันเป็น 0.3.1

### ดาวน์โหลดและเริ่มใช้งาน

1. ดาวน์โหลด **SAT-Motor-Pilot-Windows-x64.zip** และแตกไฟล์ทั้งโฟลเดอร์
2. เปิด **SATMotorPilot/SATMotorPilot.exe** โดยเก็บ DLL, platforms และ qml ไว้ด้วยกัน
3. หากอัปเดตจากรุ่นเดิม ให้ปิดแอปก่อนแล้วเขียนทับโฟลเดอร์เดิมได้
4. เปิด **Device link** เพื่อตรวจอุปกรณ์และ firmware แล้วอ่านค่าก่อนแก้ไข

ใช้ไฟล์ **SHA256SUMS.txt** ตรวจสอบความครบถ้วนของ ZIP ได้

### ผลทดสอบและสถานะ

- ส่วนติดต่อผู้ใช้: 14 runtime checks ผ่าน
- Packet/scalar: 11 passed, 0 failed รวม init/cleanup
- Compatibility core: 88/88 checks
- ตรวจแสดงหน้าจอ 33 รายการและทดสอบบันทึก/โหลด XML ในเครื่อง

รุ่นนี้เป็น **Engineering Preview** ยังไม่ได้ทดสอบกับ controller และมอเตอร์จริงในงานนี้ รายละเอียดอยู่ใน [Acceptance and known issues](https://github.com/poommyroboticcamera-netizen/SAT-Motor-Pilot/blob/V0.3.1/docs/ACCEPTANCE_AND_KNOWN_ISSUES.md)

### ซอร์สและเครดิต

ซอร์สที่ตรงกับชุดนี้อยู่ใน tag **V0.3.1** และ Source code assets ด้านล่าง พร้อมขั้นตอน build ใน README ภายใต้ GPL-3.0-or-later โดยเก็บเครดิตและเงื่อนไขของ compatibility engine เดิม ดู [LICENSE](https://github.com/poommyroboticcamera-netizen/SAT-Motor-Pilot/blob/V0.3.1/LICENSE) และ [Third-party notices](https://github.com/poommyroboticcamera-netizen/SAT-Motor-Pilot/blob/V0.3.1/THIRD_PARTY_NOTICES.md) ซึ่งระบุสถานะการตรวจ resource/dependency ไว้
