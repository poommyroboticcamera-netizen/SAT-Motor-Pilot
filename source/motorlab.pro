# MotorLab Windows desktop derivative, 2026-10-22.
# Upstream release: 01d5f10901116c311e3fb84d5a1541f663d3ce20 (7.00).
include(vesc_tool.pro)
TARGET = MotorLab
VERSION = 0.1.0
QMAKE_TARGET_PRODUCT = MotorLab
QMAKE_TARGET_DESCRIPTION = MotorLab motor configuration application
QMAKE_TARGET_COMPANY = MotorLab
DEFINES += MOTORLAB_BUILD
win32: RC_ICONS = res/motorlab/icon.ico
