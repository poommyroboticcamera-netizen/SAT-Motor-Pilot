// MotorLab configuration-write transaction. GPL-3.0-or-later, like the host project.
#ifndef MOTORLABCONFIGWRITE_H
#define MOTORLABCONFIGWRITE_H

#include "vescinterface.h"
#include <QCryptographicHash>
#include <QDateTime>
#include <QDir>
#include <QEventLoop>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QScopedValueRollback>
#include <QStandardPaths>
#include <QTimer>
#include <QUuid>

namespace MotorLabConfigWrite {
inline QString valueText(const ConfigParam &p) {
    switch (p.type) {
    case CFG_T_DOUBLE: return QString::number(p.valDouble, 'g', 16) + p.suffix;
    case CFG_T_QSTRING: return p.valString;
    case CFG_T_BOOL: return p.valInt ? QStringLiteral("true") : QStringLiteral("false");
    default: return QString::number(p.valInt) + p.suffix;
    }
}

inline void run(QWidget *parent, VescInterface *vesc, bool motor) {
    static bool busy = false;
    if (busy) return;
    QScopedValueRollback<bool> lock(busy, true);
    const QString kind = motor ? QStringLiteral("motor") : QStringLiteral("application");
    auto fail = [&](const QString &s) { QMessageBox::warning(parent, "MotorLab configuration", s); };
    Commands *commands = vesc->commands();
    if (!vesc->isPortConnected() || !vesc->fwRx() || commands->isLimitedMode() ||
            vesc->deserializeFailedSinceConnected()) {
        fail("Write blocked: connect a supported controller and complete its firmware/configuration handshake first.");
        return;
    }
    const QString port = vesc->getConnectedPortName();
    const QString uuid = vesc->getConnectedUuid();
    const bool forwarding = commands->getSendCan();
    const int can = commands->getCanSendId();
    auto sameTarget = [&]() {
        return vesc->isPortConnected() && vesc->fwRx() && !commands->isLimitedMode() &&
                !vesc->deserializeFailedSinceConnected() && port == vesc->getConnectedPortName() &&
                uuid == vesc->getConnectedUuid() && forwarding == commands->getSendCan() &&
                can == commands->getCanSendId();
    };
    if (uuid.isEmpty()) { fail("Write blocked: controller UUID is unavailable."); return; }
    ConfigParams *config = motor ? vesc->mcConfig() : vesc->appConfig();
    ConfigParams desired;
    desired = *config; // Upstream explicitly implements value assignment for its QObject subclass.
    VByteArray expected;
    desired.serialize(expected);
    const QString root = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    const QString backups = root + "/backups";
    if (!QDir().mkpath(backups)) { fail("Write blocked: cannot create the configuration backup directory."); return; }
    const QString transaction = QUuid::createUuid().toString(QUuid::WithoutBraces);
    QFile audit(root + "/config-write-audit.jsonl");
    if (!audit.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        fail("Write blocked: cannot open the configuration audit log."); return;
    }
    auto record = [&](QString outcome, QString detail) {
        QJsonObject event{{"timeUtc", QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs)},
                          {"transaction", transaction}, {"kind", kind}, {"port", port},
                          {"uuid", uuid}, {"canForwarding", forwarding}, {"canId", can},
                          {"outcome", outcome}, {"detail", detail},
                          {"expectedSha256", QString::fromLatin1(QCryptographicHash::hash(expected, QCryptographicHash::Sha256).toHex())}};
        const QByteArray line = QJsonDocument(event).toJson(QJsonDocument::Compact) + '\n';
        return audit.write(line) == line.size() && audit.flush();
    };
    auto readFresh = [&]() {
        QEventLoop loop;
        QTimer timeout;
        timeout.setSingleShot(true);
        bool received = false;
        auto connection = QObject::connect(config, &ConfigParams::updated, &loop, [&]() {
            received = sameTarget(); loop.quit();
        });
        QObject::connect(&timeout, &QTimer::timeout, &loop, &QEventLoop::quit);
        timeout.start(5000);
        if (motor) commands->getMcconf(); else commands->getAppConf();
        if (!received) loop.exec(QEventLoop::ExcludeUserInputEvents);
        QObject::disconnect(connection);
        return received && sameTarget();
    };
    if (!readFresh()) {
        *config = desired; config->updateDone();
        record("blocked", "Fresh pre-write read timed out or target changed; no write sent.");
        fail("Write blocked: fresh configuration read failed or the target changed. No write was sent."); return;
    }
    ConfigParams current;
    current = *config;
    QString safeUuid = uuid;
    safeUuid.remove(':'); safeUuid.remove('/'); safeUuid.remove('\\');
    const QString backup = backups + "/" + QDateTime::currentDateTimeUtc().toString("yyyyMMddTHHmmsszzz") +
            "_" + safeUuid + "_CAN" + QString::number(forwarding ? can : -1) + "_" + kind + "_" + transaction + ".xml";
    if (!current.saveXml(backup, motor ? "MCConfiguration" : "APPConfiguration")) {
        *config = desired; config->updateDone();
        record("blocked", "Fresh configuration backup failed; no write sent.");
        fail("Write blocked: the fresh configuration could not be backed up. No write was sent."); return;
    }
    QStringList differences;
    for (const QString &name : desired.getSerializeOrder()) {
        VByteArray before, after;
        current.getParamSerial(before, name); desired.getParamSerial(after, name);
        if (before != after) differences << name + ": " + valueText(current.getParamCopy(name)) +
                                          " -> " + valueText(desired.getParamCopy(name));
    }
    *config = desired; config->updateDone();
    QStringList cautions;
    if (motor) {
        // Preserve upstream's three motor sanity checks before confirmation. The
        // transport call below uses check=false to avoid its concurrent read.
        if ((desired.getParamDouble("l_current_max") * 1.3) > desired.getParamDouble("l_abs_current_max") ||
                (qAbs(desired.getParamDouble("l_current_min")) * 1.3) > desired.getParamDouble("l_abs_current_max"))
            cautions << "Absolute current limit is close to the motor current limit; upstream recommends about 1.5 times the greater motor/braking current.";
        if (desired.getParamBool("l_slow_abs_current"))
            cautions << "Slow ABS Current Limit is enabled; upstream warns this increases the chance of permanent controller damage.";
        if (desired.hasParam("foc_overmod_factor") && desired.getParamDouble("foc_overmod_factor") >= 1.16)
            cautions << "FOC overmodulation exceeds 1.15; upstream recommends 1.15 or less.";
    }
    if (differences.isEmpty()) { record("unchanged", "Fresh device configuration equals requested values; no write sent.");
        QMessageBox::information(parent, "MotorLab configuration", "The device already contains these values. No write was sent."); return; }
    QMessageBox confirmation(QMessageBox::Warning, "Confirm configuration write",
        "Write " + kind + " configuration?\nPort: " + port + "\nUUID: " + uuid +
        "\nCAN forwarding: " + (forwarding ? QString::number(can) : QStringLiteral("off (local controller)")) +
        "\nChanged parameters: " + QString::number(differences.size()) + "\nBackup: " + backup +
        (cautions.isEmpty() ? QString() : "\n\nPotential configuration issues:\n" + cautions.join('\n')),
        QMessageBox::Yes | QMessageBox::Cancel, parent);
    confirmation.setDefaultButton(QMessageBox::Cancel);
    confirmation.setDetailedText(differences.join('\n'));
    if (confirmation.exec() != QMessageBox::Yes) { record("cancelled", "User cancelled; no write sent."); return; }
    if (!sameTarget()) { record("blocked", "Target changed during confirmation; no write sent."); fail("Write blocked: target changed during confirmation."); return; }
    // A read triggered by another upstream component while the modal dialog was open
    // must not replace the exact values reviewed by the user.
    *config = desired;
    if (!record("sending", "Backup: " + backup + "\n" + differences.join('\n'))) {
        fail("Write blocked: audit log could not be flushed. No write was sent."); return;
    }
    QEventLoop loop;
    QTimer timeout;
    timeout.setSingleShot(true);
    bool acknowledged = false;
    const QString ack = motor ? "Motor config write OK" : "App config write OK";
    auto connection = QObject::connect(commands, &Commands::ackReceived, &loop, [&](const QString &received) {
        if (received == ack && sameTarget()) { acknowledged = true; loop.quit(); }
    });
    QObject::connect(&timeout, &QTimer::timeout, &loop, &QEventLoop::quit);
    timeout.start(5000);
    // Disable upstream's additional immediate read/check; this transaction owns the
    // ACK-then-fresh-read sequence and never retries a write.
    if (motor) commands->setMcconf(false); else commands->setAppConf();
    if (!acknowledged) loop.exec(QEventLoop::ExcludeUserInputEvents);
    QObject::disconnect(connection);
    if (!acknowledged || !sameTarget()) {
        record("uncertain", "ACK timed out or target changed after send. No retry attempted.");
        fail("Write outcome is uncertain: no matching acknowledgement, or target changed. Do not assume the write failed. No retry was sent."); return;
    }
    if (!readFresh()) {
        record("uncertain", "ACK received but fresh readback failed. No retry attempted.");
        fail("Write was acknowledged, but fresh readback failed. Configuration is unverified. No retry was sent."); return;
    }
    VByteArray actual;
    config->serialize(actual);
    if (actual != expected) {
        QStringList mismatches;
        for (const QString &name : desired.getSerializeOrder()) {
            VByteArray wanted, got;
            desired.getParamSerial(wanted, name); config->getParamSerial(got, name);
            if (wanted != got) mismatches << name + ": expected " + valueText(desired.getParamCopy(name)) +
                                           ", actual " + valueText(config->getParamCopy(name));
        }
        record("mismatch", mismatches.join('\n'));
        QMessageBox error(QMessageBox::Critical, "Configuration verification failed",
            "Device readback differs from the requested configuration. Actual device values are displayed. No retry was sent.", QMessageBox::Ok, parent);
        error.setDetailedText(mismatches.join('\n')); error.exec(); return;
    }
    if (!record("verified", "Matching ACK and exact serialized fresh readback; backup: " + backup)) {
        fail("Configuration readback matched, but the final audit record could not be saved."); return;
    }
    QMessageBox::information(parent, "MotorLab configuration", "Configuration write verified by acknowledgement and fresh device readback.\nBackup: " + backup);
}
}
#endif
