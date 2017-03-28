#ifndef ACIVIDEOVIEW_H
#define ACIVIDEOVIEW_H

#include <QObject>
#include "aciconfig.h"
#include "aciusbcontroller.h"

class ACIVideoView : public QQuickView
{
    Q_OBJECT
public:
    ACIVideoView(QQuickView *parent = 0);
    void setQmlFile(QString qml);
    void keyPressEvent(QKeyEvent *e);
    QString m_sCurrentVideo;
public slots:
    void playVideo();
private:

};

#endif // ACIVIDEOVIEW_H
