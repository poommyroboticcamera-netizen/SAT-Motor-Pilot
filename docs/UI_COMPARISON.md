# ภาพเทียบ UI และความแตกต่าง

เปรียบเทียบ pinned source release 7.00 กับ MotorLab ในสถานะ disconnected ด้วย Windows Qt 5.15.2 เหมือนกัน เปิดผ่าน native `windows` platform ใช้ software OpenGL/Qt Quick และ settings แยกกัน ไม่มีอุปกรณ์เชื่อมต่อหรือ packet จากฮาร์ดแวร์

- เก็บ 33 คู่ภาพ พร้อมขนาดจริงใน `evidence/ui/comparison/metrics.json` ขนาดตรงกันทุกคู่
- ขอหน้าต่างเริ่มต้น 1280 × 900 แต่ Qt ขยายตาม minimum size ของหน้าและ DPI จริง จึงต้องดูขนาดต่อหน้าในรายงาน ไม่อ้างว่าภาพทั้งหมดเป็น 1280 × 900
- บันทึก DPI, device pixel ratio, Qt version, หน้าที่เห็นและปุ่มที่เปิด/ปิด ใน `runtime-audit.json`
- ปิดการวาด remote OpenStreetMap เหมือนกันเฉพาะ audit เพื่อขจัดความต่างจาก tile ที่โหลดไม่พร้อมกัน; การใช้งานปกติคง implementation ต้นฉบับ
- ภาพ `side-by-side-NNN.png` เป็นภาพวางเทียบ และ `diff-NNN.png` เก็บค่าความต่างจริง ไม่ได้ลบหรือ mask ส่วนแบรนด์

ความต่างที่ตั้งใจ: โลโก้ Somboon Advance Technology ที่ผู้ใช้แนบ ชื่อ MotorLab และไอคอน Windows พื้นหลังขาวของภาพโลโก้เดิม สีเลือกเมนู/Material accent สีน้ำเงิน #304B8A แทนส้ม และข้อความ About แสดงเครดิต/แหล่งซอร์สใหม่

ความต่างเชิงพฤติกรรม: ปุ่ม Write motor/application บน toolbar เพิ่ม fresh read, backup, confirmation และ readback ตามเอกสาร CONFIGURATION_WRITE_SAFETY.md ส่วนข้อมูล debug/version ใช้ชื่อแอปใหม่ สถานะ ACK ไม่ถูกเรียกว่าผ่าน readback ก่อนตรวจจริง

ยังไม่ได้รับรอง: ทุกแท็บย่อย ทุก wizard/dialog ทุก DPI/theme/resize/shortcut/focus order และ connected/disconnected/unsupported-firmware workflow การนับภาพไม่ได้แปลว่าปุ่มและฟังก์ชันทั้งหมดผ่านแล้ว ไม่ได้ใช้ภาพเหล่านี้อ้างว่าเทียบกับ official Qt6 binary แล้ว รายการควบคุมและงานค้างอยู่ใน validation/ui-function-traceability.csv

สร้างภาพซ้ำหลัง build: `validation/run-ui-audit.ps1` และ `python validation/compare_ui.py` ต้องมี reference build สำหรับการเทียบคู่ ใช้ reference.pro ที่สร้างไว้หรือ validation/prepare-reference.py พร้อม upstream Git metadata ตามคำอธิบายในสคริปต์
