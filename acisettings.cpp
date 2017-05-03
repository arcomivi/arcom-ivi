#include "acisettings.h"
#include <QApplication>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

ACISettings::ACISettings(QObject *parent) : QObject(parent)
{
    m_oSettingsModel = new ACIListModel();
    connect(m_oSettingsModel, SIGNAL(itemClicked(Item)), this, SLOT(settingsModelClicked(Item)));
    m_oSettingsModel->addItem(Item("SETTINGS_UPDATE", "Update", "Update", "folder"));
    m_oSettingsModel->addItem(Item("SETTINGS_QUIT", "Quit", "Quit", "folder"));
}

ACIListModel *ACISettings::getModel()
{
    return m_oSettingsModel;
}

void ACISettings::settingsModelClicked(Item itemClicked){
    TRACE(QString("Name: %1, Descr: %2, Value: %3")
          .arg(itemClicked.name())
          .arg(itemClicked.descr())
          .arg(itemClicked.value()));
    QString name = itemClicked.name();

    if(name.compare("SETTINGS_QUIT")==0){
        QApplication::quit();
    } else if(name.compare("SETTINGS_UPDATE")==0){
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
