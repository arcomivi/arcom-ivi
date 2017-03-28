#include "aciusbcontroller.h"

ACIUsbController* ACIUsbController::m_Instance = 0;

/*
 * connect signals from controller to slots
 */
void ACIUsbController::connectCtrlSignal(){
    TRACE("enter");
#ifdef Q_OS_LINUX
    //connect D-Bus slots to signals
    QDBusConnection sysBus = QDBusConnection::systemBus();

    sysBus.connect("", "/aci/ctrl/egen", "aci.ctrl.input", "CtrlEvent",
                   this, SLOT(aciEventSent(QString)));

#endif
    aciCheckController();
}


/*********************************************
 * checks controller
 * if /dev/aci0 exists and events not active
 * just use restart script
 *********************************************/
void ACIUsbController::aciCheckController(){
    TRACE("enter");
    QFile aci("/dev/aci0");
    if(aci.exists() && !m_bCtrlActive){
//        system("/usr/bin/aci-ctrl-start.sh");
    }
}

/***
 * controller disconnected
 */
void ACIUsbController::aciCtrlDisconnected(QString signal){
    TRACE(signal);
    emit ctrlDisconnected();
    m_bCtrlActive = false;
}

/***
 * event receiver
 */
void ACIUsbController::aciEventSent(QString event){
//    TRACE(event);
    if (event=="ACIConnected") {
        emit ctrlActivated();
        system("/usr/bin/aci-ctrl-start.sh");
        TRACE("ACIConnected-done!");
        return;
    } else if(event=="ACIDisconnected"){
        emit ctrlDisconnected();
        system("/usr/bin/aci-ctrl-stop.sh");
        TRACE("ACIDisconnected-done!");
        return;
    }

    if(!m_bCtrlActive){
        m_bCtrlActive = true;
        emit ctrlActivated();
    }
    emit broadcastCtrlEvent(event);
}
