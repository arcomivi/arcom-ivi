#ifndef ACIUSBCONTROLLER_H
#define ACIUSBCONTROLLER_H

#include <QObject>
#include <QMutex>
#include <QFile>

#ifdef Q_OS_LINUX
#include <QtDBus/QDBusConnection>
#endif

#include <QDebug>
#include "aciconfig.h"

struct ACIUsbCtrlSignals {
    int push;
    int rot1;
    int rot2;
    int dir_up;
    int dir_down;
    int dir_left;
    int dir_right;
    int vol_rot1;
    int vol_rot2;
    int vol_push;
};

class ACIUsbController : public QObject
{
    Q_OBJECT
public:    
    static ACIUsbController* getInstance(){
        static QMutex mutex;
        if (!m_Instance){
            mutex.lock();

            if (!m_Instance)
                m_Instance = new ACIUsbController(0);

            mutex.unlock();
        }
        return m_Instance;
    }

Q_SIGNALS:
    void ctrlDisconnected();
    void ctrlActivated();
    void broadcastCtrlEvent(QString);

public Q_SLOTS:
    void aciCheckController();
    void aciCtrlDisconnected(QString);
    void aciEventSent(QString event);
    void connectCtrlSignal();

private:
    static ACIUsbController* m_Instance;
    ACIUsbController(QObject *parent) :
        QObject(parent){
        m_bCtrlActive = false;
    }
    bool m_bCtrlActive;
};

#endif // ACIUSBCONTROLLER_H
