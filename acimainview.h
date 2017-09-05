#ifndef ACIMAINVIEW_H
#define ACIMAINVIEW_H

#include "aciconfig.h"
#include "acilistmodel.h"
#include "acipagenavigation.h"
#include "acisettings.h"
#include "acisteerings.h"
#include "acimedia.h"

class ACIMainViewModel : public QObject {
        Q_OBJECT
        Q_PROPERTY(QObject* listModel READ getModel NOTIFY listModelChanged)
    public:
        ACIMainViewModel(QObject *parent=0);
        virtual ~ACIMainViewModel();

        ACIListModel* getModel() {
            return m_mainMenu;
        }

    signals:
        void listModelChanged(QObject* aNewModel);
        void loadHome();
        void loadMedia();
        void loadSettings();
    private slots:

    private:
        ACIListModel *m_mainMenu;
};


class ACIMainview : public QQuickView {
        Q_OBJECT
    public:
        explicit ACIMainview(QQuickView *parent = 0);
        void setQmlFile(QString qml);
        void setup(ACISettings *settings, ACIMedia *media, ACISteerings *steerings);
        ACIPageNavigation *getPageNav() {
            return m_pageNavigation;
        }
        void keyPressEvent(QKeyEvent *e);
        void mousePressEvent(QMouseEvent *e);
        void wheelEvent(QWheelEvent *e);

    signals:
        void loadMedia();
        void navigateToWidget(int);

    public slots:
        void loadHomeView();
        void loadMediaView();
        void loadSettingView();
        void loadVideoView();

    private:
        ACIMainViewModel *m_mainViewModel;
        ACIPageNavigation *m_pageNavigation;
        ACISettings *m_settings;
        ACIMedia *m_media;
        ACISteerings *m_steerings;
};

#endif // ACIMAINVIEW_H
