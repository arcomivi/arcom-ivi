#ifndef ACIMAINVIEW_H
#define ACIMAINVIEW_H

#include "aciconfig.h"
#include "acilistmodel.h"
#include "acipagenavigation.h"

class ACIMainViewModel : public QObject {
        Q_OBJECT
        Q_PROPERTY(QObject* listModel READ getModel NOTIFY listModelChanged)
    public:
        ACIMainViewModel(QObject *parent=0);
        virtual ~ACIMainViewModel();

        ACIListModel* getModel() {
            return m_mainMenu;
        }
        void setPageNavigation(ACIPageNavigation *pageNav) {
            m_pageNavigation=pageNav;
        }

    signals:
        void listModelChanged(QObject* aNewModel);
        void loadHome();
        void loadMedia();
        void loadSettings();
    private slots:
        void listModelClicked(Item itemClicked);
    private:
        ACIListModel *m_mainMenu;
        ACIPageNavigation *m_pageNavigation;
};


class ACIMainview : public QQuickView {
        Q_OBJECT
    public:
        explicit ACIMainview(QQuickView *parent = 0);
        void setQmlFile(QString qml);
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
};

#endif // ACIMAINVIEW_H
