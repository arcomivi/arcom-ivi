#ifndef ACIMAINCONTROLLER_H
#define ACIMAINCONTROLLER_H

#include <QObject>
#include "aciconfig.h"
#include "acimainview.h"
#include "acivideoview.h"
#include "acimedia.h"
#include "acisettings.h"
#include "acisteerings.h"
#include "acipagenavigation.h"
#include "aciusbcontroller.h"

class ACIMainController : public QObject
{
    Q_OBJECT
public:
    explicit ACIMainController(QObject *parent = Q_NULLPTR);
    void run();
signals:

public slots:

private slots:
    void screenSelected(int);
    void exitVideo();
    void navigateTo(int widget);
    void onBroadcastCtrlEvent(QString event);

private:
    ACIUsbCtrlSignals m_sCtrl, m_sCtrlPrev;
    QString m_sPreviousSignal;
    bool m_bCtrlFirstRun;
    ACIMainview *m_oMainview;
    ACIVideoView *m_oVideoView;

    ACIMedia *m_oMedia;
    ACISteerings *m_oSteerings;
    ACISettings *m_oSettings;
    ACIPageNavigation *m_oPageNavigation;
};

#endif // ACIMAINCONTROLLER_H
