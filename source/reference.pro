# Generated instrumented reference: pinned upstream UI, shared guarded transport.
include(motorlab.pro)
TARGET = MotorLabReference
QMAKE_TARGET_PRODUCT = MotorLabReference
QMAKE_TARGET_DESCRIPTION = Instrumented pinned-source UI reference
INCLUDEPATH += $$PWD
SOURCES -= main.cpp mainwindow.cpp utility.cpp
SOURCES += reference/ref_main.cpp reference/ref_mainwindow.cpp reference/ref_utility.cpp
FORMS -= mainwindow.ui
FORMS += reference/ref_mainwindow.ui
RESOURCES -= res.qrc res_neutral.qrc
RESOURCES += reference/ref_res.qrc reference/ref_neutral.qrc

INCLUDEPATH += $$PWD/pages
SOURCES -= pages/pagewelcome.cpp $$PWD/pages/pagewelcome.cpp
SOURCES += reference/ref_pagewelcome.cpp
FORMS -= pages/pagewelcome.ui $$PWD/pages/pagewelcome.ui
FORMS += reference/ref_pagewelcome.ui
