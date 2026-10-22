#include <QtTest>
#include "packet.h"
#include "vbytearray.h"

// Independently specified CRC-16/XMODEM bitwise oracle: poly 0x1021, init 0,
// no reflection, xorout 0; "123456789" has published check value 0x31c3.
static quint16 oracle(const QByteArray &b) {
    quint16 crc=0;
    for (unsigned char c:b) {
        crc ^= quint16(c)<<8;
        for(int i=0;i<8;i++) crc=quint16((crc&0x8000)?(crc<<1)^0x1021:crc<<1);
    }
    return crc;
}
static QByteArray frame(const QByteArray &b) {
    QByteArray r;
    if(b.size()<=255) {r.append(char(2));r.append(char(b.size()));}
    else {r.append(char(3));r.append(char(b.size()>>8));r.append(char(b.size()));}
    r+=b;quint16 c=oracle(b);r.append(char(c>>8));r.append(char(c));r.append(char(3));return r;
}
class ProtocolTests:public QObject {
 Q_OBJECT
private slots:
 void crcKnownVector() {
  QByteArray b("123456789"); QCOMPARE(oracle(b),quint16(0x31c3));
  QCOMPARE(Packet::crc16(reinterpret_cast<const unsigned char*>(b.constData()),b.size()),quint16(0x31c3));
  QCOMPARE(Packet::crc16(nullptr,0),quint16(0));
 }
 void encoderBoundaries() {
  Packet p; QList<QByteArray> sent;
  connect(&p,&Packet::dataToSend,[&](QByteArray &b){sent<<b;});
  for(int n:{1,255,256,10000}) {QByteArray b(n,char(0x5a));p.sendPacket(b);QCOMPARE(sent.takeLast(),frame(b));}
  p.sendPacket(QByteArray());p.sendPacket(QByteArray(10001,'x'));QVERIFY(sent.isEmpty());
  p.sendPacket(QByteArray("123456789"));QCOMPARE(sent.takeLast().toHex(),QByteArray("020931323334353637383931c303"));
 }
 void fragmentationAtEveryBoundary() {
  const QByteArray payload=QByteArray::fromHex("00020304ff7f80");const QByteArray f=frame(payload);
  for(int split=0;split<=f.size();split++) {
   Packet p;QList<QByteArray> got;connect(&p,&Packet::packetReceived,[&](QByteArray &b){got<<b;});
   p.processData(f.left(split));p.processData(f.mid(split));QCOMPARE(got,QList<QByteArray>{payload});
  }
 }
 void longBytewiseAndConcatenated() {
  Packet p;QList<QByteArray> got;connect(&p,&Packet::packetReceived,[&](QByteArray &b){got<<b;});
  QByteArray b(10000,'Z');for(char c:frame(b)) p.processData(QByteArray(1,c));
  QCOMPARE(got,QList<QByteArray>{b});got.clear();
  p.processData(frame("one")+frame("two"));QCOMPARE(got,(QList<QByteArray>{"one","two"}));
 }
 void rejectMalformedAndRecover() {
  const QByteArray good=frame("ok");
  QList<QByteArray> bad;
  QByteArray crc=frame("AAA");crc[crc.size()-3]=char(crc.at(crc.size()-3)^1);bad<<crc;
  QByteArray stop=frame("AAA");stop[stop.size()-1]=char(0);bad<<stop;
  bad<<QByteArray::fromHex("0200")<<QByteArray::fromHex("032711")<<QByteArray::fromHex("04010000");
  bad<<QByteArray::fromHex("fffe000100");
  for(const QByteArray &b:bad) {
   Packet p;QList<QByteArray> got;connect(&p,&Packet::packetReceived,[&](QByteArray &d){got<<d;});
   p.processData(b);QVERIFY(got.isEmpty());p.resetState();p.processData(good);QCOMPARE(got,QList<QByteArray>{"ok"});
  }
  Packet p;QList<QByteArray> got;connect(&p,&Packet::packetReceived,[&](QByteArray &d){got<<d;});
  p.processData(QByteArray::fromHex("fffe00")+good);QCOMPARE(got,QList<QByteArray>{"ok"});
 }
 void resetDropsPartialPacket() {
  Packet p;QList<QByteArray> got;connect(&p,&Packet::packetReceived,[&](QByteArray &d){got<<d;});
  p.processData(frame("abandoned").left(5));p.resetState();p.processData(frame("fresh"));QCOMPARE(got,QList<QByteArray>{"fresh"});
 }
 void integerWireVectors() {
  VByteArray b;b.vbAppendInt16(-2);b.vbAppendUint32(0x12345678);b.vbAppendInt64(-3);b.vbAppendUint8(0xab);
  QCOMPARE(b.toHex(),QByteArray("fffe12345678fffffffffffffffdab"));
  QCOMPARE(b.vbPopFrontInt16(),qint16(-2));QCOMPARE(b.vbPopFrontUint32(),quint32(0x12345678));
  QCOMPARE(b.vbPopFrontInt64(),qint64(-3));QCOMPARE(b.vbPopFrontUint8(),quint8(0xab));QVERIFY(b.isEmpty());
 }
 void fixedPointAndFloatVectors() {
  VByteArray b;b.vbAppendDouble32(-12.345,1000);b.vbAppendDouble16(1.25,100);b.vbAppendDouble32Auto(1.0);
  QCOMPARE(b.toHex(),QByteArray("ffffcfc7007d3f800000"));
  QCOMPARE(b.vbPopFrontDouble32(1000),-12.345);QCOMPARE(b.vbPopFrontDouble16(100),1.25);QCOMPARE(b.vbPopFrontDouble32Auto(),1.0);
 }
 void insufficientInputIsNonDestructive() {
  VByteArray b(QByteArray::fromHex("1234"));QCOMPARE(b.vbPopFrontInt32(),qint32(0));QCOMPARE(b.toHex(),QByteArray("1234"));
 }
};
QTEST_GUILESS_MAIN(ProtocolTests)
#include "protocol_tests.moc"

