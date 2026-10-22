# MotorLab 0.1.0 — Windows trial

แอปตั้งค่าและตรวจข้อมูลมอเตอร์จากซอร์ส VESC Tool stable 7.00 ปรับชื่อเป็น MotorLab และใช้โลโก้ Somboon Advance Technology กับธีมสีน้ำเงินตามที่ผู้ใช้ระบุ

1. แตกไฟล์ ZIP ทั้งโฟลเดอร์ก่อนเปิดโปรแกรม
2. เปิด `MotorLab.exe` จากโฟลเดอร์ที่มี DLL, platforms และ qml อยู่ด้วย
3. ในการเปิดครั้งแรก อ่านและทำตามหน้าคำแนะนำ/คำเตือนที่สืบทอดจากต้นฉบับ
4. ตรวจอุปกรณ์และรุ่น firmware ก่อนเชื่อมต่อ เริ่มด้วยการอ่านข้อมูล ตรวจหน่วยและค่าที่แสดงก่อนสั่งเปลี่ยนค่าหรือขับมอเตอร์

ซอร์ส วิธี build README และหลักฐานทดสอบ: https://github.com/poommyroboticcamera-netizen/SAT-Motor-Pilot

สถานะ engineering preview: ผ่านการ build บน Windows, protocol/scalar tests และ local XML persistence พร้อมการเปิดหน้าที่แสดงในสถานะ disconnected ยังไม่ผ่าน acceptance ทุก workflow หรือทดสอบกับ controller จริงในงานนี้ การใช้ source เดิมไม่ได้รับรองฮาร์ดแวร์ทุกชุด

ปุ่ม Write หลักบน toolbar เพิ่มการอ่านค่าปัจจุบัน สำรอง XML แสดงอุปกรณ์/ค่าที่เปลี่ยน ขอการยืนยัน และตรวจอ่านกลับ ส่วน wizard, CLI และเส้นทางอื่นยังคงพฤติกรรมต้นฉบับ อย่าถือว่าการยืนยัน ACK เพียงอย่างเดียวเป็นการยืนยันค่าที่เก็บจริง

MotorLab เก็บ settings แยกจากต้นฉบับ ไม่มีการ flash firmware หรือสั่งมอเตอร์อัตโนมัติระหว่างการทดสอบทั่วไป Audit mode มีไว้สำหรับตรวจ UI/ไฟล์ในเครื่องและบล็อก packet ที่จะส่งออก

GPL-3.0-or-later สำหรับซอร์สแอป เก็บเครดิต Benjamin Vedder และผู้ร่วมพัฒนา ตาม LICENSE และ THIRD_PARTY_NOTICES.md โลโก้บริษัทเป็นไฟล์จากผู้ใช้ ไม่อ้างว่าเป็นงานที่สร้างใหม่หรือ GPL artwork ชุดนี้สำหรับทดลองใน workspace; public binary release ยังรอการตรวจ resource licenses ตามที่ระบุใน notices
