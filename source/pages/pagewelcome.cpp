/*
    Copyright 2016 - 2017 Benjamin Vedder	benjamin@vedder.se

    This file is part of VESC Tool.

    VESC Tool is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    VESC Tool is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#include "pagewelcome.h"
#include "ui_pagewelcome.h"
#include "setupwizardmotor.h"
#include "setupwizardapp.h"
#include "widgets/detectallfocdialog.h"

#include <QMessageBox>
#include <QQmlEngine>
#include <QQmlContext>
#include <QSettings>
#include <QmlHighlighter>
#include <QQuickItem>
#include <QTimer>
#include <QFrame>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>

PageWelcome::PageWelcome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageWelcome)
{
    ui->setupUi(this);
    mUtil = new Utility(this);

    ui->autoConnectButton->setIcon(Utility::getIcon("icons/Connected-96.png"));
    ui->wizardFocSimpleButton->setIcon(Utility::getIcon("icons/Wizard-96.png"));
    ui->wizardAppButton->setIcon(Utility::getIcon("icons/Wizard-96.png"));
    ui->nrfPairButton->setIcon(Utility::getIcon("icons/icons8-fantasy-96.png"));
    ui->multiSettingButton->setIcon(Utility::getIcon("icons/Settings-96.png"));
    ui->invertDirButton->setIcon(Utility::getIcon("icons/Process-96.png"));
    ui->setupBluetoothButton->setIcon(Utility::getIcon("icons/bluetooth.png"));
    ui->wizardIMUButton->setIcon(Utility::getIcon("icons/imu_off.png"));

    layout()->setContentsMargins(0, 0, 0, 0);
    mVesc = nullptr;
    ui->bgWidget->setPixmap(QPixmap());
    ui->label->hide();
    auto dashboard = new QFrame(this);
    dashboard->setObjectName("satCommissioningWorkspace");
    dashboard->setStyleSheet("QFrame#satCommissioningWorkspace {background:#18243a;border:1px solid #304b8a;border-radius:12px;} "
                            "QPushButton {padding:10px;background:#304b8a;color:white;border-radius:5px;} "
                            "QPushButton:hover {background:#3d63ae;}");
    auto grid = new QGridLayout(dashboard);
    grid->setContentsMargins(20,18,20,18);
    auto title = new QLabel("<span style='font-size:23pt;font-weight:600;color:#ffffff'>SAT Motor Pilot</span>", dashboard);
    auto subtitle = new QLabel("SOMBOON ADVANCE TECHNOLOGY  /  MOTOR COMMISSIONING", dashboard);
    subtitle->setWordWrap(true);
    subtitle->setStyleSheet("color:#71b9e8;font-size:10pt;");
    auto guidance = new QLabel("Connect and identify your controller. Read its existing configuration before editing. "
                               "Use the workspace below to configure, verify and measure.", dashboard);
    guidance->setWordWrap(true);
    auto target = new QLabel("No controller connected", dashboard);
    target->setObjectName("satTargetStatus");
    target->setWordWrap(true);
    target->setStyleSheet("padding:8px;color:#b8d9f3;");
    grid->addWidget(title,0,0,1,2);
    grid->addWidget(subtitle,1,0,1,2);
    grid->addWidget(guidance,2,0,1,2);
    grid->addWidget(target,3,0,1,2);
    const QList<QPair<QString,QString>> destinations = {
        {"1  Connection", "Connection"}, {"2  Motor configuration", "Motor Settings"},
        {"3  Application configuration", "App Settings"}, {"4  Live measurements", "Realtime Data"}};
    int index = 0;
    for (const auto &destination : destinations) {
        auto button = new QPushButton(destination.first, dashboard);
        button->setObjectName(QString("satWorkspace%1").arg(index));
        const QString name = destination.second;
        connect(button, &QPushButton::clicked, this, [this,name]() { emit openWorkspace(name); });
        grid->addWidget(button,4+index/2,index%2);
        ++index;
    }
    ui->verticalLayout->insertWidget(0,dashboard);
    ui->qmlWidget->setMinimumWidth(280);
    ui->splitter->setStretchFactor(0, 3);
    ui->splitter->setStretchFactor(1, 1);
    ui->splitter->setSizes(QList<int>{690,300});
    auto statusTimer = new QTimer(this);
    statusTimer->setInterval(500);
    connect(statusTimer, &QTimer::timeout, this, [this,target]() {
        if (!mVesc || !mVesc->isPortConnected()) {
            target->setText("DISCONNECTED  |  No controller readings. Select Connection to begin.");
        } else if (!mVesc->fwRx()) {
            target->setText("CONNECTED  |  Waiting for firmware and controller identity.");
        } else {
            const auto fw = mVesc->getLastFwRxParams();
            const QString can = mVesc->commands()->getSendCan() ? QString::number(mVesc->commands()->getCanSendId()) : "local";
            target->setText(QString("TARGET  %1  |  FW %2.%3  |  CAN %4\nUUID %5")
                .arg(mVesc->getConnectedPortName()).arg(fw.major).arg(fw.minor,2,10,QChar('0')).arg(can,mVesc->getConnectedUuid()));
        }
    });
    statusTimer->start();

    connect(ui->wizardFocSimpleButton, &QPushButton::clicked, [this]() {
        QMetaObject::invokeMethod(ui->qmlWidget->rootObject(), "setupMotors");
    });

    connect(ui->wizardAppButton, SIGNAL(clicked(bool)),
            this, SLOT(startSetupWizardApp()));

    connect(ui->multiSettingButton, &QPushButton::clicked, [this]() {
        QMetaObject::invokeMethod(ui->qmlWidget->rootObject(), "openMultiSettings");
    });

    connect(ui->invertDirButton, &QPushButton::clicked, [this]() {
        QMetaObject::invokeMethod(ui->qmlWidget->rootObject(), "dirSetup");
    });
}

PageWelcome::~PageWelcome()
{
    delete ui;
}

void PageWelcome::startSetupWizardFocSimple()
{
    if (mVesc) {
        DetectAllFocDialog::showDialog(mVesc, this);
    }
}

void PageWelcome::startSetupWizardFocQml()
{
    if (mVesc) {
        mQmlUi.startCustomGui(mVesc, "qrc:/res/qml/SetupMotorWindow.qml");
    }
}

void PageWelcome::startSetupWizardMotor()
{
    if (mVesc) {
        SetupWizardMotor w(mVesc, this);
        w.exec();
    }
}

void PageWelcome::startSetupWizardApp()
{
    if (mVesc) {
        SetupWizardApp w(mVesc, this);
        w.exec();
    }
}

VescInterface *PageWelcome::vesc() const
{
    return mVesc;
}

void PageWelcome::setVesc(VescInterface *vesc)
{
    mVesc = vesc;

    ui->qmlWidget->engine()->rootContext()->setContextProperty("VescIf", mVesc);
    ui->qmlWidget->engine()->rootContext()->setContextProperty("QmlUi", this);
    ui->qmlWidget->engine()->rootContext()->setContextProperty("Utility", mUtil);

    ui->qmlWidget->setSource(QUrl(QLatin1String("qrc:/res/qml/WelcomeQmlPanel.qml")));
}

void PageWelcome::on_autoConnectButton_clicked()
{
    Utility::autoconnectBlockingWithProgress(mVesc, this);
}

void PageWelcome::on_nrfPairButton_clicked()
{
    QMetaObject::invokeMethod(ui->qmlWidget->rootObject(), "nrfQuickPair");
}

void PageWelcome::on_setupBluetoothButton_clicked()
{
    QMetaObject::invokeMethod(ui->qmlWidget->rootObject(), "openBleSetup");
}

void PageWelcome::on_wizardIMUButton_clicked()
{
    QMetaObject::invokeMethod(ui->qmlWidget->rootObject(), "openWizardIMU");
}

