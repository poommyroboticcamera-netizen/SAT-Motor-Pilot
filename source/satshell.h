// Somboon SAT workspace shell. GPL-3.0-or-later.
#ifndef SATSHELL_H
#define SATSHELL_H
#include <QMainWindow>
#include <QFrame>
#include <QLabel>
#include <QToolButton>
#include <QToolBar>
#include <QMenu>
#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <functional>
#include "ui_mainwindow.h"
#include "vescinterface.h"
#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace SatShell {
inline void install(QMainWindow *window, Ui::MainWindow *ui, VescInterface *vesc,
                    std::function<void(QString)> navigate) {
    window->setStyleSheet(
        "QMainWindow,QWidget#centralWidget {background:#0c1422;color:#d7e3f4;}"
        "QMenuBar,QMenu {background:#111d30;color:#b7c8df;padding:3px;}"
        "QMenuBar::item:selected,QMenu::item:selected {background:#244d83;color:#edf6ff;}"
        "QWidget#satAppBar {background:#111f34;border:1px solid #243752;border-radius:9px;}"
        "QWidget#satCommandBar {background:#111f34;border:1px solid #243752;border-radius:8px;}"
        "QWidget#satManualPanel {background:#142238;border:1px solid #31517a;border-radius:8px;}"
        "QListWidget#pageList {background:#0f1c2e;border:1px solid #20344e;border-radius:8px;outline:0;}"
        "QListWidget#pageList::item {border:0;border-radius:5px;margin:1px 5px;}"
        "QListWidget#pageList::item:selected {background:#224e87;border-left:3px solid #58b7ee;}"
        "QListWidget#canList {background:#101f32;border:1px solid #243b58;border-radius:6px;}"
        "QToolButton {background:#1a304c;color:#d8eaff;border:1px solid #315070;border-radius:5px;padding:7px 10px;}"
        "QToolButton:hover,QPushButton:hover {background:#284c76;border-color:#5d9bc5;}"
        "QToolButton:checked {background:#285c98;border-color:#78b9e6;}"
        "QToolButton:disabled,QPushButton:disabled {color:#5e718c;background:#101e30;border-color:#20324a;}"
        "QPushButton {background:#1c3554;color:#d9e7f7;border:1px solid #365474;border-radius:5px;padding:6px 10px;}"
        "QLineEdit,QSpinBox,QDoubleSpinBox,QComboBox,QTextEdit,QPlainTextEdit {background:#101c2d;color:#d7e3f4;border:1px solid #2a4260;border-radius:4px;padding:3px;}"
        "QGroupBox {border:1px solid #2a3c56;border-radius:7px;margin-top:12px;padding:12px 6px 5px;}"
        "QGroupBox::title {subcontrol-origin:margin;left:12px;color:#86a8cf;padding:0 5px;}"
        "QTabWidget::pane {border:1px solid #29415e;background:#111f32;}"
        "QTabBar::tab {background:#14243b;color:#96acc8;padding:7px 12px;border-bottom:2px solid #14243b;}"
        "QTabBar::tab:selected {background:#203b5f;color:#e5f1ff;border-bottom-color:#62b9ec;}"
        "QStatusBar {background:#0c1422;color:#7d9abb;}"
        "QSplitter::handle {background:#0c1422;}"
        "QScrollBar:vertical {background:#101d2f;width:8px;}"
        "QScrollBar::handle:vertical {background:#365273;min-height:24px;border-radius:4px;}"
        "QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical {height:0;}"
    );
    ui->mainToolBar->hide();
    ui->verticalLayout_3->setContentsMargins(14,12,14,8);
    ui->verticalLayout_3->setSpacing(10);
    ui->pageLabel->setPixmap(QPixmap());
    ui->pageLabel->setText("SAT  /  WORKSPACES");
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
    headerLayout->setContentsMargins(16,10,16,10);
    auto mark = new QLabel(header);
    QPixmap companyMark(":/res/icon.svg");
    // Native viewport framing of the corporate emblem in the transparent master.
    QRect emblem(companyMark.width()*0.305,companyMark.height()*0.18,
                 companyMark.width()*0.39,companyMark.height()*0.45);
    mark->setPixmap(companyMark.copy(emblem).scaled(46,50,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    headerLayout->addWidget(mark);
    auto identity = new QLabel("<span style='font-size:18pt;font-weight:600;color:#e3efff'>SAT Motor Pilot</span>"
                               "<br><span style='font-size:9pt;color:#6ea4d4'>SOMBOON ADVANCE TECHNOLOGY · MOTOR SYSTEMS</span>",header);
    headerLayout->addWidget(identity);
    headerLayout->addStretch();
    auto session = new QLabel("NO DEVICE SESSION",header);
    session->setObjectName("satSessionStatus");
    session->setStyleSheet("color:#8ba7c8;background:#172c46;border-radius:5px;padding:9px;");
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
    commandsLayout->setContentsMargins(12,8,12,8);
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
    manualNote->setStyleSheet("color:#a0bad7;padding:4px;");
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
