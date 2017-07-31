#ifndef ACIMAINCONTROLLER_H
#define ACIMAINCONTROLLER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonValue>
#include "aciconfig.h"
#include "acimainview.h"
#include "acivideoview.h"
#include "acimedia.h"
#include "acisettings.h"
#include "acisteerings.h"
#include "acipagenavigation.h"
#include "aciusbcontroller.h"

class ACIMainController : public QObject {
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
        ACIUsbCtrlSignals m_ctrlSignal, m_ctrlPreviousSignal;
        QString m_previousSignal;
        bool m_ctrlFirstRun;
        ACIMainview *m_mainView;
        ACIVideoView *m_videoView;

        ACIMedia *m_media;
        ACISteerings *m_steerings;
        ACISettings *m_settings;
        ACIPageNavigation *m_pageNavigation;
};

#endif // ACIMAINCONTROLLER_H
