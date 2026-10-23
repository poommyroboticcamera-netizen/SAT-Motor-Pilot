# SAT Motor Pilot Windows company application, 2026-10-23.
# The upstream project remains the compatible engine and expert tools.
include(vesc_tool.pro)
TARGET = SATMotorPilot
VERSION = 0.3.0
QMAKE_TARGET_PRODUCT = SAT Motor Pilot
QMAKE_TARGET_DESCRIPTION = Somboon Advance Technology motor commissioning workspace
QMAKE_TARGET_COMPANY = Somboon Advance Technology
DEFINES += MOTORLAB_BUILD SAT_MOTOR_PILOT
HEADERS += satshell.h satproduct.h motorlabaudit.h motorlabconfigwrite.h
win32: RC_ICONS = res/motorlab/icon.ico
