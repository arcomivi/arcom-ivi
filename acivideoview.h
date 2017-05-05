#ifndef ACIVIDEOVIEW_H
#define ACIVIDEOVIEW_H

#include <QObject>
#include "aciconfig.h"
#include "aciusbcontroller.h"
#include "acipagenavigation.h"

class ACIVideoView : public QQuickView
{
    Q_OBJECT
public:
    ACIVideoView(QQuickView *parent = 0);
    void setQmlFile(QString qml);
    void keyPressEvent(QKeyEvent *e);
    QString m_sCurrentVideo;
    ACIPageNavigation *getPageNav(){ return m_oPageNavigation; }
public slots:
    void playVideo();
private:
    ACIPageNavigation *m_oPageNavigation;
};

#endif // ACIVIDEOVIEW_H
