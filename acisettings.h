#ifndef ACISETTINGS_H
#define ACISETTINGS_H

#include <QObject>
#include "acilistmodel.h"
#include "acipagenavigation.h"

class ACISettings : public QObject {
        Q_OBJECT
        Q_PROPERTY(QObject* listModel READ getModel NOTIFY listModelChanged)
        Q_PROPERTY(QObject* screens READ getScreens NOTIFY screensChanged)
    public:
        explicit ACISettings(QObject *parent = nullptr);
        void setPageNavigation(ACIPageNavigation *pageNav) {
            m_pageNavigation=pageNav;
        }

        ACIListModel *getModel();
        ACIListModel *getScreens();

    signals:
        void screenExit();
        void listModelChanged(QObject* aNewModel);
        void screensChanged(QObject* aNewModel);
        void screenSelected(int aScreen);
        void update();

    public slots:
        void updateMe();
    private slots:
        void settingsModelClicked(Item);
        void screensModelClicked(Item);


    private:
        ACIListModel *m_settingsModel;
        ACIListModel *m_screens;
        ACIPageNavigation *m_pageNavigation;
};

#endif // ACISETTINGS_H
