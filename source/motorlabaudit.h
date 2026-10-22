// MotorLab disconnected rendering audit. GPL-3.0-or-later; no warranty.
#ifndef MOTORLABAUDIT_H
#define MOTORLABAUDIT_H
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QListWidget>
#include <QLabel>
#include <QAbstractButton>
#include <QAction>
#include <QTimer>
#include <QScreen>
#include <memory>
#include "vescinterface.h"
#include "map/mapwidget.h"

inline void motorLabAudit(MainWindow *window, const QString &directory)
{
    if (directory.isEmpty()) return;
    QDir().mkpath(directory + "/screenshots");
    window->resize(1280, 900);
    auto report = std::make_shared<QJsonObject>();
    (*report)["scope"] = "Disconnected UI rendering and local configuration persistence only; no hardware validation";
    (*report)["transportPolicy"] = "Audit mode requires physical outbound transport blocked by VescInterface";
    (*report)["platform"] = QGuiApplication::platformName();
    (*report)["qtVersion"] = qVersion();
    (*report)["devicePixelRatio"] = window->devicePixelRatioF();
    (*report)["logicalDpi"] = QGuiApplication::primaryScreen()->logicalDotsPerInch();
    (*report)["mapPolicy"] = "Remote OpenStreetMap rendering disabled identically in both audited builds to eliminate asynchronous tile content";
    for (MapWidget *map : window->findChildren<MapWidget *>()) {
        map->setDrawOpenStreetmap(false);
    }
    QJsonArray configs;
    VescInterface *vesc = window->findChild<VescInterface *>();
    if (vesc) {
        const QList<QPair<QString, ConfigParams *>> sources = {
            {"motor", vesc->mcConfig()}, {"application", vesc->appConfig()}};
        for (const auto &entry : sources) {
            ConfigParams original, restored;
            original = *entry.second;
            restored = original;
            // Perturb the detached copy so a no-op XML reader cannot pass this check.
            for (const QString &name : restored.getParamOrder()) {
                ConfigParam *parameter = restored.getParam(name);
                if (!parameter) continue;
                parameter->valDouble += 1.0;
                parameter->valInt ^= 1;
                parameter->valString += "_audit_sentinel";
            }
            const int differencesBeforeLoad = original.checkDifference(&restored).size();
            const QString file = directory + "/" + entry.first + "-config.xml";
            const QString configName = entry.first == "motor" ? "MCConfiguration" : "APPConfiguration";
            VByteArray before, after;
            original.serialize(before);
            bool saved = original.saveXml(file, configName);
            bool loaded = saved && restored.loadXml(file, configName);
            restored.serialize(after);
            const QStringList differences = original.checkDifference(&restored);
            QJsonObject result;
            result["kind"] = entry.first;
            result["parameterCount"] = original.getParamOrder().size();
            result["serializedBytes"] = before.size();
            result["saved"] = saved;
            result["loaded"] = loaded;
            result["xmlStatus"] = restored.xmlStatus();
            result["differencesBeforeLoad"] = differencesBeforeLoad;
            result["differenceCount"] = differences.size();
            result["differences"] = QJsonArray::fromStringList(differences);
            result["serializedBytesEqual"] = before == after;
            result["pass"] = saved && loaded && differencesBeforeLoad > 0 && !before.isEmpty() && differences.isEmpty() && before == after;
            configs.append(result);
        }
    }
    (*report)["configurationRoundtrips"] = configs;
    (*report)["interfaceFound"] = vesc != nullptr;
    auto pages = std::make_shared<QJsonArray>();
    auto row = std::make_shared<int>(0);
    QListWidget *list = window->findChild<QListWidget *>("pageList");
    auto timer = new QTimer(window);
    timer->setInterval(180);
    QObject::connect(timer, &QTimer::timeout, window, [=]() {
        // Capture the previously selected row after it has had a full timer tick to render.
        if (*row > 0 && list) {
            const int previous = *row - 1;
            QListWidgetItem *item = list->item(previous);
            if (item && !item->isHidden()) {
                QJsonObject page;
                page["row"] = previous;
                QString pageTitle = item->text();
                if (QWidget *itemView = list->itemWidget(item)) {
                    QStringList labels;
                    for (QLabel *label : itemView->findChildren<QLabel *>()) {
                        if (!label->text().trimmed().isEmpty()) labels.append(label->text().trimmed());
                    }
                    if (!labels.isEmpty()) pageTitle = labels.join(" | ");
                }
                page["title"] = pageTitle;
                page["enabled"] = bool(item->flags() & Qt::ItemIsEnabled);
                page["windowWidth"] = window->width();
                page["windowHeight"] = window->height();
                const QString image = QString("screenshots/page-%1.png").arg(previous, 3, 10, QChar('0'));
                page["screenshot"] = image;
                page["screenshotSaved"] = window->grab().save(directory + "/" + image);
                QJsonArray controls;
                for (QWidget *control : window->findChildren<QWidget *>()) {
                    if (!control->isVisible() || control->objectName().isEmpty()) continue;
                    QJsonObject c;
                    c["name"] = control->objectName();
                    c["class"] = control->metaObject()->className();
                    c["enabled"] = control->isEnabled();
                    if (QAbstractButton *button = qobject_cast<QAbstractButton *>(control)) c["text"] = button->text();
                    controls.append(c);
                }
                page["visibleControls"] = controls;
                pages->append(page);
            }
        }
        while (list && *row < list->count() && list->item(*row)->isHidden()) ++*row;
        if (!list || *row >= list->count()) {
            timer->stop();
            (*report)["pageListFound"] = list != nullptr;
            (*report)["pages"] = *pages;
            QJsonArray actions;
            for (QAction *action : window->findChildren<QAction *>()) {
                QJsonObject a;
                a["name"] = action->objectName();
                a["text"] = action->text();
                a["enabled"] = action->isEnabled();
                a["visible"] = action->isVisible();
                actions.append(a);
            }
            (*report)["actions"] = actions;
            QFile output(directory + "/runtime-audit.json");
            if (output.open(QIODevice::WriteOnly)) output.write(QJsonDocument(*report).toJson(QJsonDocument::Indented));
            QApplication::quit();
            return;
        }
        list->setCurrentRow(*row);
        ++*row;
    });
    timer->start();
}
#endif
