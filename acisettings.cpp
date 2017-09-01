#include "acisettings.h"
#include <QApplication>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>


ACISettings::ACISettings(QObject *parent) : QObject(parent) {
    m_screens = Q_NULLPTR;
    m_settingsModel = new ACIListModel();
    connect(m_settingsModel, SIGNAL(itemClicked(Item)), this, SLOT(settingsModelClicked(Item)));
    m_settingsModel->addItem(new Item("SETTINGS_UPDATE", "Update", "Update", "folder"));
    m_settingsModel->addItem(new Item("SETTINGS_QUIT", "Quit", "Quit", "folder"));
}

//!
//! \brief ACISettings::getModel
//! \return
//!
ACIListModel *ACISettings::getModel() {
    return m_settingsModel;
}

//!
//! \brief ACISettings::getScreens
//! \return
//!
ACIListModel *ACISettings::getScreens() {
    if(m_screens==Q_NULLPTR) {
        m_screens=new ACIListModel();
        connect(m_screens, SIGNAL(itemClicked(Item)), this, SLOT(screensModelClicked(Item)));
    }
    m_screens->removeRows(0, m_screens->rowCount());
    m_screens->addItem(new Item("SCREEN_EXIT", "<- Exit / Back"));

    QDesktopWidget *desktopWidget = QApplication::desktop();
    for (int screen = 0; screen < desktopWidget->screenCount(); ++screen) {
        m_screens->addItem(new Item(QString("SCREEN_%1").arg(screen), QString("SCREEN - %1").arg(screen), QString("%1").arg(screen)));
    }
    return m_screens;
}

void ACISettings::settingsModelClicked(Item *itemClicked) {
    TRACE(QString("Name: %1, Descr: %2, Value: %3")
          .arg(itemClicked->name())
          .arg(itemClicked->descr())
          .arg(itemClicked->value()));
    QString name = itemClicked->name();

    if(name.compare("SETTINGS_QUIT")==0) {
        QApplication::quit();
    } else if(name.compare("SETTINGS_UPDATE")==0) {
        emit update();
        //        foreach (QSerialPortInfo sp, QSerialPortInfo::availablePorts()) {
        //            qDebug() << "sp.portName():" << sp.portName();
        //            qDebug() << "sp.hasProductIdentifier(): " << sp.hasProductIdentifier();
        //            qDebug() << "sp.hasVendorIdentifier(): " << sp.hasVendorIdentifier();
        //            qDebug() << "sp.isBusy():" << sp.isBusy();
        //            qDebug() << "sp.isNull():" << sp.isNull();
        //            qDebug() << "sp.manufacturer():" << sp.manufacturer();
        //            qDebug() << "sp.productIdentifier():" << sp.productIdentifier();
        //            qDebug() << "sp.serialNumber():" << sp.serialNumber();
        //            qDebug() << "sp.systemLocation():" << sp.systemLocation();
        //            qDebug() << "sp.vendorIdentifier():" << sp.vendorIdentifier();
        //            foreach (quint32 br, QSerialPortInfo::standardBaudRates()) {
        //                qDebug() << "br: " << br;
        //            }
        //            QSerialPort serial;
        //            serial.setPort(sp);
        //            if(serial.open(QIODevice::ReadOnly)){
        //                qDebug() << "opened....";
        //                int numRead = 0, numReadTotal = 0;
        //                char buffer[50];

        //                for (;;) {
        //                    numRead  = serial.read(buffer, 50);

        //                    // Do whatever with the array
        //                    qDebug() << "Read: " << buffer << ", num: " << numRead;
        //                    numReadTotal += numRead;
        //                    if (numRead == 0 && !serial.waitForReadyRead(5000))
        //                        break;
        //                }
        //                serial.close();
        //            } else {
        //                qDebug() << "Error: " << serial.errorString();
        //            }
        //        }
    }
}

//!
//! \brief ACISettings::screensModelClicked
//! \param itemClicked - item that was clicked
//!
void ACISettings::screensModelClicked(Item *itemClicked) {
    TRACE(QString("Name: %1, Descr: %2, Value: %3")
          .arg(itemClicked->name())
          .arg(itemClicked->descr())
          .arg(itemClicked->value()));
    QString name = itemClicked->name();
    if(name.compare("SCREEN_EXIT")==0) {
        emit screenExit();
        //        emit m_pageNavigation->loadView("ACIMediaView.qml");
        //        emit m_pageNavigation->loadSteering("0");
    } else {
        emit screenSelected(itemClicked->value().toInt());
    }
}

//!
//! \brief ACIMainview::updateMe
//!
void ACISettings::updateMe() {
    //    //TODO: move to separate helper class
    TRACE("updating1...");

    //    QString update_dir="not-found";
    TRACE("updating2...");
    //#ifdef Q_OS_LINUX
    //    QList<QStorageInfo> logicalDrives = QStorageInfo::mountedVolumes();
    TRACE("updating3...");
    QString update_dir="";
    foreach (const QStorageInfo &storage, QStorageInfo::mountedVolumes()) {
        if (storage.isValid() && storage.isReady()) {
            if (!storage.isReadOnly()) {
                // ...
            }
            if(storage.rootPath() == "/") {
                update_dir = storage.rootPath() + "update";
            } else {
                update_dir = storage.rootPath() + "/update";
            }
        }

        //    foreach(QString drive, logicalDrives){
        //        if(drive=="/"){
        //            update_dir = drive + "update";
        //        } else {
        //            update_dir = drive + "/update";
        //        }
        TRACE(QString("usb: %1").arg(update_dir));
        QDir dir(update_dir);
        if(dir.exists()) {
            //check if there is an updater.sh script as well
            QString updateScript("%1/updater.sh");
            QFile updateFile(updateScript.arg(update_dir));
            if(updateFile.exists()) {
                break;
            }
        }
        TRACE(QString("updater.sh not found under %1").arg(update_dir));
        update_dir="not-found";
    }
    TRACE("updating4...");

    TRACE(QString("done searching: %1").arg(update_dir));
    //check if equals to: not-found
    if(update_dir.compare("not-found", Qt::CaseInsensitive)==0) {
        TRACE("no updates found");
        ////        emit displayNotification("No update folder found!");

        return;
    }


    //    //0. do some checks
    if(update_dir=="") {
        TRACE("update_dir empty");
        return;
    }
    QString updateScript(update_dir+"/updater.sh "+update_dir+"/repo");
    TRACE("start "+updateScript);
    system(updateScript.toLatin1().data());
    TRACE("end "+updateScript);
    QGuiApplication::quit();
    //#endif
    ////    emit displayNotification(QString("... update done!"));
    TRACE("exit");
}
