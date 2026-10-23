// Somboon SAT workspace shell. GPL-3.0-or-later.
#ifndef SATSHELL_H
#define SATSHELL_H
#include <QMainWindow>
#include <QFrame>
#include <QLabel>
#include <QToolButton>
#include <QToolBar>
#include <QTabWidget>
#include <QMenu>
#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <functional>
#include "ui_mainwindow.h"
#include "vescinterface.h"
#include "sattypography.h"
#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace SatShell {
// Black instrument surfaces, square controls and restrained company-blue accents.
inline void install(QMainWindow *window, Ui::MainWindow *ui, VescInterface *vesc,
                    std::function<void(QString)> navigate) {
    window->setStyleSheet(
        "QMainWindow,QWidget#centralWidget {background:#0b0c0e;color:#d2d5da;}"
        "QMenuBar,QMenu {background:#15171a;color:#b9bec6;padding:3px;}"
        "QMenuBar::item:selected,QMenu::item:selected {background:#244d83;color:#edf6ff;}"
        "QWidget#satAppBar {background:#17191c;border:1px solid #363b42;border-radius:2px;}"
        "QWidget#satCommandBar {background:#17191c;border:1px solid #363b42;border-radius:2px;}"
        "QWidget#satManualPanel {background:#191c20;border:1px solid #41474f;border-radius:2px;}"
        "QListWidget#pageList {background:#101214;border:1px solid #30343a;border-radius:2px;outline:0;}"
        "QListWidget#pageList::item {border:0;border-radius:2px;margin:1px 5px;}"
        "QListWidget#pageList::item:selected {background:#224e87;border-left:3px solid #58b7ee;}"
        "QListWidget#canList {background:#131619;border:1px solid #343a42;border-radius:2px;}"
        "QToolButton {background:#24282d;color:#dde2e8;border:1px solid #424a54;border-radius:2px;padding:7px 10px;}"
        "QToolButton:hover,QPushButton:hover {background:#303c4b;border-color:#5d9bc5;}"
        "QToolButton:checked {background:#285c98;border-color:#78b9e6;}"
        "QToolButton:disabled,QPushButton:disabled {color:#5e718c;background:#17191c;border-color:#2c3036;}"
        "QPushButton {background:#252a30;color:#d8dde4;border:1px solid #46515e;border-radius:2px;padding:6px 10px;}"
        "QLineEdit,QSpinBox,QDoubleSpinBox,QComboBox,QTextEdit,QPlainTextEdit {background:#101214;color:#d2d5da;border:1px solid #3b4149;border-radius:2px;padding:3px;}"
        "QGroupBox {border:1px solid #363c44;border-radius:2px;margin-top:12px;padding:12px 6px 5px;}"
        "QGroupBox::title {subcontrol-origin:margin;left:12px;color:#86a8cf;padding:0 5px;}"
        "QTabWidget::pane {border:1px solid #373e47;background:#14171a;}"
        "QTabBar::tab {background:#171a1e;color:#a4acb7;padding:7px 12px;border:0;border-bottom:2px solid #171a1e;margin-right:2px;}"
        "QTabBar::tab:selected {background:#252d38;color:#e6e9ee;border-bottom-color:#62b9ec;}"
        "QStatusBar {background:#0b0c0e;color:#8d98a6;}"
        "QSplitter::handle {background:#0b0c0e;}"
        "QScrollBar:vertical {background:#131518;width:8px;}"
        "QScrollBar::handle:vertical {background:#444b55;min-height:24px;border-radius:2px;}"
        "QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical {height:0;}"
    );
    for(auto tabs:window->findChildren<QTabWidget*>()){tabs->setTabShape(QTabWidget::Rounded);tabs->setDocumentMode(true);}
    ui->mainToolBar->hide();
    ui->verticalLayout_3->setContentsMargins(10,8,10,6);
    ui->verticalLayout_3->setSpacing(6);
    ui->pageLabel->setPixmap(QPixmap());
    ui->pageLabel->setText("SAT  /  LAB WORKSPACES");
    ui->pageLabel->setStyleSheet("color:#6f99c8;font-size:9pt;font-weight:600;padding:8px;");
    ui->pageLabel->setMaximumHeight(36);
    ui->pageList->setMinimumWidth(244);
    ui->pageList->setMaximumWidth(244);
    ui->pageList->setGridSize(QSize(234,34));
    for (int i=0;i<ui->pageList->count();++i) ui->pageList->item(i)->setSizeHint(QSize(234,34));
    ui->canList->setMinimumWidth(244); ui->canList->setMaximumWidth(244);
    ui->scanCanButton->setMaximumWidth(244);
    ui->scanCanButton->setText("Discover CAN targets");
    ui->scanCanButton->setIcon(QIcon());
    ui->label->setText("CONNECTED TARGETS");
    ui->label->setStyleSheet("color:#6d91bb;font-size:9pt;padding:5px;");
    auto header = new QFrame(window);
    header->setObjectName("satAppBar");
    auto headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(12,7,12,7);
    auto mark = new QLabel(header);
    QPixmap companyMark(":/res/icon.svg");
    // Native viewport framing of the corporate emblem in the transparent master.
    QRect emblem(companyMark.width()*0.305,companyMark.height()*0.18,
                 companyMark.width()*0.39,companyMark.height()*0.45);
    mark->setPixmap(companyMark.copy(emblem).scaled(46,50,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    headerLayout->addWidget(mark);
    auto identity = new QLabel("<span style='font-size:16pt;font-weight:600;color:#e2e6ec'>SAT Motor Pilot</span>"
                               "<br><span style='font-size:9pt;color:#6ea4d4'>SOMBOON ADVANCE TECHNOLOGY · MOTOR CONTROL LAB</span>",header);
    headerLayout->addWidget(identity);
    identity->setFont(QFont(SatTypography::displayFamily()));
    window->setProperty("satDisplayFont",SatTypography::displayFamily());
    headerLayout->addStretch();
    auto session = new QLabel("NO DEVICE SESSION",header);
    session->setObjectName("satSessionStatus");
    session->setStyleSheet("color:#b0bac7;background:#20252b;border-radius:2px;padding:9px;");
    headerLayout->addWidget(session);
    auto deviceLink = new QToolButton(header);
    deviceLink->setText("Device link");
    deviceLink->setObjectName("satDeviceLink");
    QObject::connect(deviceLink,&QToolButton::clicked,window,[=]() { navigate("Connection"); });
    headerLayout->addWidget(deviceLink);
    ui->verticalLayout_3->insertWidget(0,header);
    auto commands = new QFrame(window);
    commands->setObjectName("satCommandBar");
    auto commandsLayout = new QHBoxLayout(commands);
    commandsLayout->setContentsMargins(10,6,10,6);
    auto caption = new QLabel("CONFIGURATION",commands);
    caption->setStyleSheet("color:#6992bf;font-size:8pt;font-weight:600;padding-right:8px;");
    commandsLayout->addWidget(caption);
    auto actionButton=[&](QAction *action,QString text) {
        auto button=new QToolButton(commands);
        button->setDefaultAction(action);
        button->setToolButtonStyle(Qt::ToolButtonTextOnly);
        action->setText(text);
        commandsLayout->addWidget(button);
        return button;
    };
    actionButton(ui->actionReadMcconf,"Read motor");
    actionButton(ui->actionWriteMcconf,"Write motor");
    actionButton(ui->actionReadAppconf,"Read inputs");
    actionButton(ui->actionWriteAppconf,"Write inputs");
    auto stream = new QToolButton(commands);
    stream->setText("Session tools");
    stream->setPopupMode(QToolButton::InstantPopup);
    auto menu = new QMenu(stream);
    for (auto action : ui->mainToolBar->actions()) if(!action->isSeparator()) menu->addAction(action);
    stream->setMenu(menu);
    commandsLayout->addWidget(stream);
    commandsLayout->addStretch();
    auto manualToggle = new QToolButton(commands);
    manualToggle->setText("Manual drive");
    manualToggle->setObjectName("satManualToggle");
    manualToggle->setCheckable(true);
    commandsLayout->addWidget(manualToggle);
    auto stop=new QToolButton(commands);
    stop->setText("Output off");
    stop->setToolTip("Switch off output on the selected controller. This is not a bus-wide emergency stop.");
    stop->setStyleSheet("color:#ffb5bc;background:#3a2232;border:1px solid #9a4759;");
    QObject::connect(stop,&QToolButton::clicked,ui->stopButton,&QPushButton::click);
    commandsLayout->addWidget(stop);
    ui->verticalLayout_3->insertWidget(1,commands);
    ui->verticalLayout_3->removeItem(ui->gridLayout_3);
    ui->gridLayout_3->setParent(nullptr);
    auto manual=new QFrame(window);
    manual->setObjectName("satManualPanel");
    auto manualLayout=new QVBoxLayout(manual);
    auto manualNote=new QLabel("MANUAL DRIVE  ·  Commands act on the selected controller. Review the target and limits before enabling output.",manual);
    manualNote->setWordWrap(true);
    manualNote->setStyleSheet("color:#aab4c2;padding:4px;");
    manualLayout->addWidget(manualNote);
    manualLayout->addLayout(ui->gridLayout_3);
    ui->verticalLayout_3->addWidget(manual);
    manual->hide();
    QObject::connect(manualToggle,&QToolButton::toggled,manual,&QWidget::setVisible);
    const QList<QPair<QPushButton*,QString>> driveButtons={
        {ui->dutyButton,"Set duty"},{ui->currentButton,"Set current"},{ui->speedButton,"Set RPM"},
        {ui->posButton,"Set position"},{ui->brakeCurrentButton,"Brake"},{ui->handbrakeButton,"Hold"},
        {ui->fullBrakeButton,"Full brake"},{ui->stopButton,"Output off"}};
    for(const auto &pair:driveButtons){pair.first->setIcon(QIcon());pair.first->setText(pair.second);}
    auto timer=new QTimer(window);
    QObject::connect(timer,&QTimer::timeout,window,[=]() {
        session->setText(!vesc->isPortConnected()?"NO DEVICE SESSION":!vesc->fwRx()?"NEGOTIATING FIRMWARE":QString("LINKED · %1").arg(vesc->getConnectedPortName()));
        session->setToolTip(vesc->getConnectedUuid());
    });
    timer->start(500);
#ifdef Q_OS_WIN
    HMODULE dwm=LoadLibraryW(L"dwmapi.dll");
    if(dwm){
        typedef HRESULT (WINAPI *SetAttribute)(HWND,DWORD,LPCVOID,DWORD);
        auto apply=reinterpret_cast<SetAttribute>(GetProcAddress(dwm,"DwmSetWindowAttribute"));
        if(apply){BOOL dark=TRUE;HRESULT result=apply(reinterpret_cast<HWND>(window->winId()),20,&dark,sizeof(dark));window->setProperty("satDarkTitlebarApplied",SUCCEEDED(result));}
        FreeLibrary(dwm);
    }
#endif
}
}
#endif
