// SAT Motor Pilot product layer. GPL-3.0-or-later.
#ifndef SATPRODUCT_H
#define SATPRODUCT_H
#include <QString>
#include <QRegularExpression>
#include <QTranslator>
#include "configparams.h"

namespace SatProduct {
inline QString displayText(QString text) {
    // Presentation only: never call this on packets, identifiers or stored values.
    text.replace("VESC® Tool", "SAT Motor Pilot");
    text.replace("VESC Tool", "SAT Motor Pilot");
    text.replace("MotorLab", "SAT Motor Pilot");
    text.replace("VESC Packages", "Controller Packages");
    text.replace("VESC Remote", "Remote Control");
    text.replace(QRegularExpression("\\bVESCs\\b"), "controllers");
    text.replace(QRegularExpression("\\bVESC(?:®)?-based\\b"), "compatible");
    text.replace(QRegularExpression("\\bVESC(?:®)?(?=\\W|$)"), "controller");
    return text;
}
inline void adaptLabels(ConfigParams *info) {
    for (const auto &name : info->getParamOrder()) {
        auto *p = info->getParam(name);
        if (!p || name == "gpl_text") continue;
        p->longName = displayText(p->longName);
        p->description = displayText(p->description);
    }
}
inline void applyHelp(ConfigParams *info) {
    adaptLabels(info);
    auto set = [=](QString key, QString title, QString html) {
        if (auto *p = info->getParam(key)) { p->longName=title; p->description=html; }
    };
    set("wizard_startup_intro", "Welcome to SAT Motor Pilot",
        "<h2>Somboon Advance Technology</h2><p>SAT Motor Pilot is the company's motor commissioning workspace. "
        "Connect a supported controller, read its existing configuration, review motor and application settings, "
        "then inspect live measurements.</p><p>This application runs independently. It does not launch or require "
        "another motor configuration application. Compatibility code, third-party notices and original authors are "
        "documented in Help / About and the source distribution.</p>");
    set("wizard_startup_conclusion", "Your workspace is ready",
        "<h2>Start by reading the controller</h2><ol><li>Open Connection and select the intended device.</li>"
        "<li>Confirm its firmware, target ID and connection state.</li><li>Read motor and application configuration "
        "before editing. Keep a backup.</li><li>Run motor detection, write settings or flash firmware only after "
        "reviewing the target and the operation.</li></ol><p>Open Help / SAT quick start for the company workflow. "
        "Project source and current build information: "
        "<a href=\"https://github.com/poommyroboticcamera-netizen/SAT-Motor-Pilot\">SAT Motor Pilot project</a>.</p>");
}
class PresentationTranslator : public QTranslator {
public:
    QString translate(const char *, const char *source, const char *, int) const override {
        const QString original = QString::fromUtf8(source);
        if (original.contains("GPL-") || original.contains("Copyright") || original.contains("Benjamin Vedder")) return {};
        const QString translated = displayText(original);
        return translated == original ? QString() : translated;
    }
};
inline QString quickStart() {
    return "<h2>SAT Motor Pilot — Somboon Advance Technology</h2>"
        "<p><b>1. Connect and identify.</b> Use Connection. Check the selected port, controller UUID, firmware and CAN target.</p>"
        "<p><b>2. Read and preserve.</b> Read the current motor and application configuration. Export XML before commissioning.</p>"
        "<p><b>3. Configure.</b> Use Motor Settings and App Settings. Motor detection can energize the motor; review its limits and target first.</p>"
        "<p><b>4. Verify.</b> Toolbar Write reads and saves the existing configuration, asks for confirmation, "
        "then checks acknowledgement and fresh readback. A timeout after sending is an uncertain result.</p>"
        "<p><b>5. Measure.</b> Use Realtime Data and Log Analysis. Disconnected fields are not device measurements.</p>"
        "<p>Firmware and controller packages use compatible external data sources when explicitly requested; "
        "application builds and support belong to the SAT Motor Pilot project. No background check for another application is performed.</p>";
}
}
#endif
