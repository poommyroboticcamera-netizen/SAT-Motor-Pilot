// Company display typography. GPL-3.0-or-later.
#ifndef SATTYPOGRAPHY_H
#define SATTYPOGRAPHY_H
#include <QFontDatabase>
#include <QCoreApplication>
#include <QFileInfo>
namespace SatTypography {
inline QString displayFamily() {
    static const QString family=[]() {
        // Licensed font files are private installation assets, never bundled in Git.
        for(const QString &name:{QString("Muffie.ttf"),QString("Muffie.otf")}){
            const QString path=QCoreApplication::applicationDirPath()+"/fonts/"+name;
            if(!QFileInfo::exists(path)) continue;
            const int id=QFontDatabase::addApplicationFont(path);
            for(const QString &candidate:QFontDatabase::applicationFontFamilies(id))
                if(candidate.contains("Muffie",Qt::CaseInsensitive)) return candidate;
        }
        const auto installed=QFontDatabase().families();
        for(const QString &candidate:installed)
            if(candidate.contains("Muffie",Qt::CaseInsensitive)) return candidate;
        return installed.contains("Impact",Qt::CaseInsensitive)?QString("Impact"):QString("Segoe UI");
    }();
    return family;
}
}
#endif
