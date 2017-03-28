#ifndef ACIMAINVIEW_H
#define ACIMAINVIEW_H

#include "aciconfig.h"
#include "aciusbcontroller.h"
#include "acimedia.h"
#include "acivideoview.h"

class ACIMainview : public QQuickView
{
    Q_OBJECT
public:
    explicit ACIMainview(QQuickView *parent = 0);
    void setQmlFile(QString qml);
    void keyPressEvent(QKeyEvent *e);

Q_SIGNALS:
    void navigateToWidget(int);
    void navigateToPreviousWidget();

public slots:
    void onBroadcastCtrlEvent(QString event);
    void enterNavigation();    
    void navigateTo(int widget);
    void handleRot(int direction);
    void updateMe();
    void loadMedia();
    void sendProgress(int progress);
    void watchVideo(QString);
    void screenSelected(int);
    void exitVideo();


private:
    //structure with current and previous USB controller signal
    ACIUsbCtrlSignals m_sCtrl, m_sCtrlPrev;
    bool m_bCtrlFirstRun;
    QString m_sPreviousSignal;
    ACIMedia *m_oMedia;
    ACIVideoView *m_oVideoView;
    QObject *m_oCurrentView;
};

#endif // ACIMAINVIEW_H
