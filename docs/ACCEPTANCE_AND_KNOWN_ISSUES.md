# Acceptance และประเด็นที่ยังต้องยืนยัน

สถานะหลักฐาน ณ 23 ตุลาคม 2026 (Asia/Bangkok) เอกสารนี้แยก source ที่มีอยู่ ผลที่รันจริง และงานที่ยังเปิดไว้ ไม่รับรองความเทียบเท่าทั้งโปรแกรมหรือการใช้งานกับมอเตอร์จริง


## SAT product conversion 0.2.0

โปรแกรมปัจจุบันคือ SAT Motor Pilot ของสมบูรณ์ มี dashboard/navigation, onboarding, คู่มือ/changelog, application identity และ settings ของตนเองตาม COMPANY_PRODUCT.md รูปแบบหน้าแรกต่างจากต้นฉบับโดยตั้งใจตามคำสั่งล่าสุด จึงไม่ใช้ pixel identity ของหน้าแรกเป็นเกณฑ์รับงานใหม่ ผล 88-item core regression บอกว่าโปรโตคอล/serializer/schema ในขอบเขตนั้นคงเดิม ไม่ได้ยืนยันฮาร์ดแวร์ ผล navigation 4 ปุ่มและ onboarding เป็นการทดสอบ UI เท่านั้น ขั้นตอน read/write, detection และ flash กับอุปกรณ์จริงยังค้างเช่นเดิม
## Company shell 0.3.0

หน้าตา SAT ปรับใหม่ทั้งแถบหัว เมนู แถบคำสั่ง และแผง Manual drive การเปิด/ปิดแผงไม่ส่ง command ตาม runtime audit และ native dark title bar ใช้ได้บนเครื่องที่ทดสอบ ภาพเทียบรุ่นนี้มีขนาดหน้าต่างต่างจาก reference ทั้ง 33 คู่ จึงไม่คำนวณ pixel delta ข้ามขนาด ภาพ side-by-side เก็บขนาดจริงโดยไม่ rescale ผลภาพเปรียบเทียบรุ่นก่อนเป็นหลักฐานเก่า ไม่ใช่การรับรองหน้าตาปัจจุบัน

USB เป็นช่องทางเริ่มทดสอบบอร์ด VESC: จ่ายไฟตามข้อกำหนดของบอร์ด ต่อ USB data cable เปิด Device link แล้วตรวจ firmware และอ่าน configuration ก่อนเปลี่ยนค่า รายชื่อ schema เป็นขอบเขตของ source ไม่ใช่ผลทดสอบบอร์ดจริง

## Evidence levels

| หัวข้อ | หลักฐานปัจจุบัน | สิ่งที่ยืนยันได้ | สิ่งที่ยังยืนยันไม่ได้ |
|:--|:--|:--|:--|
| รุ่นต้นทาง | Pinned VESC Tool 7.00 release commit | ระบุ source release ที่ใช้ build | ไม่รับรองว่า installed Qt6 binary เป็น reproducible build จาก commit นี้ |
| Firmware | Matching bldc 7.00 source commit และ schema inventory | ระบุ firmware source reference / schemas ที่มี | Firmware ที่ติดตั้งจริง ความเข้ากันได้ทุก command หรือ flash success |
| Packet/scalar serialization | QtTest รัน original code: 11 passed | เวกเตอร์และ boundary ที่ระบุใน tests ผ่าน | Transport ทั้งระบบ, timeout, handshake และ response ทุก command |
| หน้าแอป | Runtime audit บันทึก 33 visible page rows ของ reference และ MotorLab | เปิดหน้าและบันทึก screenshot ใน disconnected state | Pixel parity, ทุก tab/menu/dialog/wizard และผลเมื่อกด controls |
| Motor config XML | Production ConfigParams: 211 parameters, serialized 488 bytes | save → perturb destination → load คืนค่า; 197 differences ก่อน load → 0 หลัง load; bytes เท่ากัน | Physical controller read/write, ทุก schema และ failure mode |
| App config XML | Production ConfigParams: 122 parameters, serialized 284 bytes | save → perturb destination → load คืนค่า; 117 differences ก่อน load → 0 หลัง load; bytes เท่ากัน | Physical controller read/write, input peripherals หรือทุก schema |
| Source inventories | 5,170 UI/function rows; 6,606 parameter records; 157 command IDs | ค้นหาขอบเขต source และเตรียม acceptance | ไม่ใช่จำนวนฟังก์ชันที่ทดสอบผ่าน |
| ฮาร์ดแวร์ | ยังไม่มี live-transport/hardware trial ในงานนี้ | ไม่มีผล hardware ที่จะอ้าง | Motor detection, drive, faults, current accuracy, BLE/CAN radio/backend และ flashing |

Runtime reports: [MotorLab](../evidence/ui/motorlab/runtime-audit.json), [reference](../evidence/ui/reference/runtime-audit.json). Audit mode บล็อก physical outbound transport ทั้งสอง build เปิดผ่าน native Windows platform โดยใช้ software renderer และ PATH เฉพาะ Windows system directories; exit code 0 ทั้งคู่ มีภาพเทียบ 33 คู่พร้อม pixel metrics โดยปิดการโหลดแผนที่แบบ asynchronous เหมือนกันทั้งสองฝั่ง ดู evidence/ui/comparison/metrics.json และ portable-launch.json การเปลี่ยนโลโก้บริษัทและสีเน้นน้ำเงินเป็นความต่างที่ผู้ใช้ระบุโดยตั้งใจ ไม่ถือเป็นการผ่าน workflow หรือฮาร์ดแวร์

## ผล local persistence หมายถึงอะไร

Audit ใช้ระบบ ConfigParams ของโปรแกรมจริง ไม่ได้แทนด้วย dictionary จำลอง ก่อน load ทำให้ destination มีค่าต่างจากต้นฉบับอย่างตั้งใจ แล้วตรวจความต่างหลังอ่าน XML และเปรียบเทียบ serialized bytes อีกชั้นหนึ่ง จึงช่วยป้องกันผลผ่านแบบ destination ยังถือค่าเดิมอยู่ การตรวจนี้เขียนและอ่านไฟล์ local เท่านั้น ไม่มีการเขียน configuration ลง controller

ผลที่มีเป็นชุด motor/app ที่ audit โหลดใน runtime ปัจจุบัน ยังต้องทดสอบ Unicode path, XML ขาดหาย/เสีย, permission denied, unknown field, version mismatch และ schema อื่น ก่อนอ้าง coverage ของระบบ persistence ทั้งหมด

## Known issues และ runtime warnings

พบ runtime diagnostics ที่เกี่ยวกับ QML binding loops, package archive ที่ไม่มี และการหา parameter group `gpd` ข้อมูลเหล่านี้เกิดในทรัพยากร/ขั้นตอนที่สืบทอดจาก upstream ควรเทียบ reference กับ MotorLab และเก็บ exact message ต่อรอบ ไม่ควรปิดทิ้งแล้วถือว่าไม่มีปัญหา

- **QML binding loops:** หน้าเปิดได้ไม่ได้แปลว่า layout หรือทุกค่าใน QML update ถูกต้อง ตรวจการ resize, loading state, bound values และ screenshot ที่ได้รับผลกระทบ
- **Missing package archive:** ชุด offline อาจไม่มี package resource ที่หน้าจอคาดหวัง ต้องตรวจรายการ packages, error และการโหลดไฟล์ก่อนอ้างว่าใช้งาน package workflow ได้
- **Missing `gpd` parameter group:** ต้องตรวจ schema/firmware ที่เลือกและหน้า GPDrive ว่ามี control ครบหรือแสดง empty state ถูกต้อง ยังไม่รับรอง GPDrive workflow
- **Asset/dependency notices:** รายการที่ยังไม่ยืนยันใน [THIRD_PARTY_NOTICES](../THIRD_PARTY_NOTICES.md) ต้องจัดการก่อน public binary release ชุด executable ที่เตรียมเป็น local trial

## งาน acceptance ที่ยังเปิด

| กลุ่ม | งานที่ต้องทำต่อ | เกณฑ์รับ |
|:--|:--|:--|
| UI | เทียบขนาดหน้าต่าง DPI/theme/font/data state เดียวกัน | ทุกหน้า/tab/menu/context menu/modal/wizard มีภาพและผลที่ตรวจได้; ความต่างอธิบายได้ |
| Runtime controls | กด controls ตาม scenario และตรวจ enabled/disabled/errors | ผลจริงตรง scenario และไม่รายงาน success ก่อน acknowledgment |
| Connection | Serial/TCP/UDP/BLE ตามที่เลือกใช้ | Connect/disconnect/reconnect, timeout และ truncated/corrupt input มีผลและ error ที่ตรวจได้ |
| Firmware negotiation | จำลอง/บันทึก response หลายรุ่นที่ระบุ | เลือก schema ถูก, unsupported command ถูกปฏิเสธ/เตือนอย่างเหมาะสม |
| Configuration | File round trips เพิ่ม failure cases; physical read/write ภายหลัง | ค่า unrelated ไม่เปลี่ยน, diff ตรง, read-back มีหลักฐาน |
| Telemetry/logs | Recorded fixtures พร้อม expected units/scales | กราฟ ตัวเลข CSV และ playback ตรงข้อมูลอ้างอิง |
| Motor/control | Test rig และ instruments ที่ระบุ | Limits/fault/stop และค่าที่วัดจริงผ่านเกณฑ์ของอุปกรณ์นั้น |
| Package/install | Clean Windows, Unicode paths, plugins/notices | เปิดและใช้งาน scenario ได้จาก portable folder โดยไม่พึ่ง Qt ในเครื่อง build |

## ขั้นตอนทดสอบฮาร์ดแวร์ภายหลัง

ขั้นตอนนี้เป็น checklist สำหรับผู้ปฏิบัติงาน ไม่ใช่สคริปต์สั่งงานอัตโนมัติ ในรอบงานปัจจุบันยังไม่ได้ทำขั้นตอนเหล่านี้

1. **ระบุอุปกรณ์และ baseline:** บันทึก board revision, controller firmware/version/hash ที่อ่านได้, motor, supply และ wiring ตรวจ reference ของ firmware และเครื่องมือวัดที่จะใช้ ตั้งเกณฑ์ยอมรับก่อนเริ่ม
2. **เตรียม bench:** ใช้ supply ที่จำกัดกระแสได้ ยึดมอเตอร์/โหลดให้เหมาะสม และเตรียมทางตัดกำลังที่ตรวจได้ แยกการทดสอบ UI จากการอนุญาตให้ภาคกำลังทำงาน
3. **เริ่มจากการอ่าน:** เชื่อมต่อ transport ที่ต้องการ อ่าน identity, fault และ telemetry เก็บ raw exchange/เวลา/ผลลัพธ์ ยังไม่กด Write, Detection, Calibration, Upload หรือ motor-control command
4. **สำรอง configuration:** อ่าน motor/app config จาก controller บันทึกไฟล์และ hash ตรวจว่าอ่านกลับได้ เก็บ baseline สำหรับตรวจ unrelated changes
5. **ตรวจ telemetry:** เปรียบเทียบ bus voltage, temperature และ current zero กับเครื่องมืออิสระ เก็บหน่วย สเกล sample conditions และเกณฑ์ ก่อนใช้ค่าดังกล่าวตัดสินผลการขับ
6. **ทดสอบ transport failure:** ในสภาวะที่ไม่มีคำสั่งขับมอเตอร์ ตรวจ disconnect/reconnect/timeout และยืนยันว่า UI ไม่ค้างหรือถือข้อมูล stale ว่าเป็นค่าปัจจุบัน
7. **เขียนเฉพาะเมื่อผู้ปฏิบัติงานเลือกทดสอบ:** ตรวจ diff ของค่าที่ตั้งใจเปลี่ยนด้วยสายตา เลือกค่าที่เหมาะกับชุดทดลอง แล้วสั่ง Write จาก UI เอง เก็บ acknowledgment และ read-back เทียบ baseline หากมี unrelated changes ให้หยุดตรวจสาเหตุ
8. **Detection/calibration/drive เป็นแต่ละ test case:** ระบุเงื่อนไข current/voltage/speed/load ที่เหมาะกับ hardware ก่อนสั่งงานจาก UI ทดสอบทีละขั้น บันทึก waveform/telemetry/fault/การหยุดตามเกณฑ์ ห้ามใช้ผล XML หรือ packet tests แทนผล physical drive
9. **Firmware update แยกต่างหาก:** ระบุ exact firmware payload และ hash, hardware target, backup และ recovery path ก่อนผู้ปฏิบัติงานสั่ง upload ตรวจ version/read-back หลังเสร็จ
10. **ปิดรอบ:** หยุดคำสั่ง ทวนสถานะ controller คืนค่าเฉพาะที่ตั้งใจคืนด้วยการตรวจ diff/read-back และบันทึกผล PASS/FAIL/NOT RUN พร้อมหลักฐานต่อ inventory item

Direct PC CAN และ CAN forwarding ต้องเป็น test cases คนละชุด เช่นเดียวกับ Serial, TCP, UDP และ BLE ไม่ใช้ความสำเร็จของ transport หนึ่งรับรองอีก transport

## สถานะ traceability

`validation/ui-function-traceability.csv` ให้ค่า `RENDERED_ONLY` เฉพาะแถว form ที่จับคู่จาก runtime class ไปยัง `.ui` และมี screenshot จริง แถว control/action/function ยังคง `PENDING` แม้ชื่อ control ปรากฏใน runtime inventory รายละเอียดจับคู่แยกใน [rendered-page-evidence.csv](../validation/rendered-page-evidence.csv) มี runtime page rows มากกว่า unique forms ได้เมื่อหลายหน้าใช้ form เดียวกัน หรือหน้าเป็น custom widget ที่ไม่ได้ขึ้นต้นด้วย Page

ชื่อหน้าใน audit รอบแรกว่าง จึงใช้ class ของ widget และตำแหน่ง screenshot เป็นหลักฐานจับคู่ ไม่เดาชื่อหน้าแล้วเปลี่ยนเป็นผ่าน Root worker สามารถรัน audit รุ่นปรับปรุงและ `validation/update_runtime_inventory.py` เพื่อ refresh เฉพาะ rendered markers ได้
