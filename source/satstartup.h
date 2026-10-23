// Somboon startup presentation. GPL-3.0-or-later.
#ifndef SATSTARTUP_H
#define SATSTARTUP_H
#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QElapsedTimer>
#include <QScreen>
#include <QApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QDir>
#include <functional>

class SatStartupScreen : public QWidget {
public:
    explicit SatStartupScreen(const QString &auditDirectory) :
        QWidget(nullptr,Qt::SplashScreen | Qt::FramelessWindowHint), mAuditDirectory(auditDirectory) {
        setObjectName("satStartupScreen");
        setWindowTitle("SAT Motor Pilot");
        setFixedSize(760,420);
        setWindowIcon(QIcon(":/res/icon.svg"));
        if(auto screen=QApplication::primaryScreen()) move(screen->availableGeometry().center()-rect().center());
        QPixmap master(":/res/icon.svg");
        mEmblem=master.copy(QRect(master.width()*0.305,master.height()*0.18,master.width()*0.39,master.height()*0.45));
        mTimer.setInterval(33);
        QObject::connect(&mTimer,&QTimer::timeout,this,[this]() {
            const int milliseconds=int(mElapsed.elapsed());
            repaint();
            if(!mAuditDirectory.isEmpty() && milliseconds>=180 && mFrames<42){
                QDir().mkpath(mAuditDirectory+"/startup");
                const QString name=QString("startup/frame-%1.png").arg(mFrames,3,10,QChar('0'));
                QImage frame=grab().toImage();
                const bool saved=frame.save(mAuditDirectory+"/"+name);
                const QByteArray pixels(reinterpret_cast<const char*>(frame.constBits()),int(frame.sizeInBytes()));
                mSamples.append(QJsonObject{{"elapsedMs",milliseconds},{"path",name},{"saved",saved},
                                           {"pixelSha256",QString::fromLatin1(QCryptographicHash::hash(pixels,QCryptographicHash::Sha256).toHex())}});
                ++mFrames;
            }
            if(milliseconds>=1600){
                mTimer.stop();
                if(mReady) mReady();
            }
        });
    }
    void start(std::function<void()> ready){
        mReady=std::move(ready);
        mElapsed.start();
        show();
        mTimer.start();
    }
    QJsonObject auditReport() const {
        return {{"durationMs",int(mElapsed.elapsed())},{"paintedFrames",mPaintedFrames},
                {"samples",mSamples},{"hardwareActions",0},{"scope","Brand animation only; no device discovery or hardware commands"}};
    }
protected:
    void paintEvent(QPaintEvent *) override {
        ++mPaintedFrames;
        const qreal time=qMin(1.0,mElapsed.elapsed()/650.0);
        const qreal reveal=1.0-(1.0-time)*(1.0-time);
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.fillRect(rect(),QColor("#090a0c"));
        p.setPen(QColor("#353a43"));
        p.drawRect(rect().adjusted(0,0,-1,-1));
        p.setPen(QColor("#6c7785"));
        p.setFont(QFont("Segoe UI",9));
        p.drawText(QRect(28,24,704,24),Qt::AlignLeft,"SOMBOON ADVANCE TECHNOLOGY");
        p.drawText(QRect(28,24,704,24),Qt::AlignRight,"MOTOR CONTROL LAB");
        p.save();
        p.setOpacity(reveal);
        const int growth=int((1.0-reveal)*10);
        p.drawPixmap(QRect(126+growth,125+growth,92-2*growth,106-2*growth),mEmblem);
        p.setPen(QColor("#e6e9ef"));
        p.setFont(QFont("Segoe UI",30,QFont::DemiBold));
        p.drawText(QRect(240,137,480,60),Qt::AlignLeft|Qt::AlignVCenter,"SAT Motor Pilot");
        p.setPen(QColor("#779fc9"));
        p.setFont(QFont("Segoe UI",10));
        p.drawText(QRect(242,202,460,26),Qt::AlignLeft,"PRECISION  /  CONFIGURATION  /  MEASUREMENT");
        p.restore();
        p.fillRect(QRect(126,278,508,2),QColor("#282d34"));
        // An indeterminate loading sweep, not a percentage or hardware status.
        const int sweep=int((mElapsed.elapsed()%1000)/1000.0*430);
        p.fillRect(QRect(126+sweep,278,78,2),QColor("#4e96df"));
        p.setFont(QFont("Segoe UI",10));
        p.setPen(QColor("#909aa7"));
        const int dots=int(mElapsed.elapsed()/300)%4;
        p.drawText(QRect(126,295,508,28),Qt::AlignLeft,"Starting workspace"+QString(dots,'.'));
        p.setFont(QFont("Segoe UI",9));
        p.setPen(QColor("#606b79"));
        p.drawText(QRect(28,367,704,24),Qt::AlignLeft,"SAT  /  MOTOR SYSTEMS");
        p.drawText(QRect(28,367,704,24),Qt::AlignRight,"0.3.0");
    }
private:
    QString mAuditDirectory;
    QPixmap mEmblem;
    QTimer mTimer;
    QElapsedTimer mElapsed;
    std::function<void()> mReady;
    QJsonArray mSamples;
    int mFrames=0;
    int mPaintedFrames=0;
};
#endif
