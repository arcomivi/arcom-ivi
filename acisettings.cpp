#include "acisettings.h"
#include <QApplication>

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
    }
}
