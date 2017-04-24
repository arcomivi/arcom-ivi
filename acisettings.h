#ifndef ACISETTINGS_H
#define ACISETTINGS_H

#include <QObject>
#include "acilistmodel.h"

class ACISettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* listModel READ getModel NOTIFY listModelChanged)
public:
    explicit ACISettings(QObject *parent = nullptr);

    ACIListModel *getModel();
signals:
    void listModelChanged(QObject* aNewModel);
    void update();
private slots:
    void settingsModelClicked(Item);
private:
    ACIListModel *m_oSettingsModel;
};

#endif // ACISETTINGS_H
