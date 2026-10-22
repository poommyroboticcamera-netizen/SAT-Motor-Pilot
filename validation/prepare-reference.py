"""Prepare an instrumented pinned-source UI reference; never builds or runs it.
The reference shares transport guards and audit instrumentation with MotorLab.
It is not an official upstream release binary or an independent hardware baseline.
"""
from pathlib import Path
import subprocess
import os
import argparse
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "source"
REFERENCE = SOURCE / "reference"
PIN = "01d5f10901116c311e3fb84d5a1541f663d3ce20"
REFERENCE.mkdir(exist_ok=True)

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("--upstream-repo", type=Path, help="Existing upstream working tree or bare git directory containing the pinned commit")
args = parser.parse_args()
candidates = [args.upstream_repo] if args.upstream_repo else [SOURCE, ROOT / ".upstream-git", SOURCE / ".upstream-git"]
git_prefix = None
for candidate in candidates:
    if not candidate or not candidate.exists():
        continue
    prefix = ["git", "-C", str(candidate)] if (candidate / ".git").exists() else ["git", "--git-dir", str(candidate)]
    if subprocess.run(prefix + ["cat-file", "-e", PIN + "^{commit}"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL).returncode == 0:
        git_prefix = prefix
        break
if git_prefix is None:
    raise SystemExit("Pinned upstream git object unavailable. Supply --upstream-repo PATH to a checkout or bare git directory containing " + PIN + ". Existing generated reference files remain usable. No network fetch was attempted.")

def original(name):
    return subprocess.check_output(git_prefix + ["show", f"{PIN}:{name}"])

def text(name):
    return original(name).decode("utf-8")

current_main = (SOURCE / "main.cpp").read_text(encoding="utf-8")
main = text("main.cpp")
main = main.replace('#include "mainwindow.h"', '#include "mainwindow.h"\n#include "motorlabaudit.h"')
start = current_main.index('    const QString motorLabAuditDirectory =')
end = current_main.index('    QSettings set;', start)
isolation = current_main[start:end]
needle = '    QSettings set;\n    bool isDark'
assert needle in main
main = main.replace(needle, isolation + needle, 1)
needle = '            w = new MainWindow;\n            w->show();'
assert needle in main
main = main.replace(needle, needle + '\n            motorLabAudit(w, motorLabAuditDirectory);', 1)
(REFERENCE / "ref_main.cpp").write_text(main, encoding="utf-8")
window = text("mainwindow.cpp").replace('#include "ui_mainwindow.h"', '#include "ui_ref_mainwindow.h"')
(REFERENCE / "ref_mainwindow.cpp").write_text(window, encoding="utf-8")
(REFERENCE / "ref_mainwindow.ui").write_bytes(original("mainwindow.ui"))
(REFERENCE / "ref_utility.cpp").write_bytes(original("utility.cpp"))

for relative in ["res/logo_vertical.png", "res/+theme_light/logo_vertical.png", "res/symbol_v_wide.png", "res/+theme_light/symbol_v_wide.png"]:
    target = REFERENCE / relative
    target.parent.mkdir(parents=True, exist_ok=True)
    target.write_bytes(original(relative))

(REFERENCE / "ref_pagewelcome.ui").write_bytes(original("pages/pagewelcome.ui"))
(REFERENCE / "ref_pagewelcome.cpp").write_text(text("pages/pagewelcome.cpp").replace('#include "ui_pagewelcome.h"', '#include "ui_ref_pagewelcome.h"'), encoding="utf-8")

# Explicit aliases retain the original resource URLs while paths resolve from reference/.
for source_qrc, output_qrc in [("res.qrc", "ref_res.qrc"), ("res_neutral.qrc", "ref_neutral.qrc")]:
    tree = ET.fromstring(original(source_qrc))
    for entry in tree.iter("file"):
        relative = entry.text
        if "alias" not in entry.attrib:
            entry.set("alias", relative)
        physical = REFERENCE / relative if relative in ["res/logo_vertical.png", "res/+theme_light/logo_vertical.png", "res/symbol_v_wide.png", "res/+theme_light/symbol_v_wide.png"] else SOURCE / relative
        entry.text = Path(os.path.relpath(physical, REFERENCE)).as_posix()
    ET.ElementTree(tree).write(REFERENCE / output_qrc, encoding="utf-8", xml_declaration=True)

# Keep this project at the source root so included upstream relative paths stay valid.
(SOURCE / "reference.pro").write_text("""# Generated instrumented reference: pinned upstream UI, shared guarded transport.
include(motorlab.pro)
TARGET = MotorLabReference
QMAKE_TARGET_PRODUCT = MotorLabReference
QMAKE_TARGET_DESCRIPTION = Instrumented pinned-source UI reference
INCLUDEPATH += $$PWD $$PWD/pages
SOURCES -= main.cpp mainwindow.cpp utility.cpp pages/pagewelcome.cpp $$PWD/pages/pagewelcome.cpp
SOURCES += reference/ref_main.cpp reference/ref_mainwindow.cpp reference/ref_utility.cpp reference/ref_pagewelcome.cpp
FORMS -= mainwindow.ui pages/pagewelcome.ui $$PWD/pages/pagewelcome.ui
FORMS += reference/ref_mainwindow.ui reference/ref_pagewelcome.ui
RESOURCES -= res.qrc res_neutral.qrc
RESOURCES += reference/ref_res.qrc reference/ref_neutral.qrc
""", encoding="utf-8")
print("Prepared reference.pro and reference/ from pinned source", PIN)
