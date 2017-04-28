#ifndef ACIMAINVIEW_H
#define ACIMAINVIEW_H

#include "aciconfig.h"
#include "aciusbcontroller.h"
#include "acimedia.h"
#include "acivideoview.h"
#include "acisettings.h"
#include "acilistmodel.h"
#include <QMouseEvent>
#include <QWheelEvent>

class ACIPageNavigation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int current READ current WRITE setCurrent NOTIFY currentChanged)
public:
    ACIPageNavigation(QObject *parent=0);
    virtual ~ACIPageNavigation();
    int current() {return m_current;}

//    void handleRelease();

signals:
    void loadView(QString aView);
    void currentChanged();
    void handleRelease();

public slots:
    void init();
    void setCurrent(int value) { m_current = value; }
private:
    int m_current;
};

class ACIMainViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* listModel READ getModel NOTIFY listModelChanged)
public:
    ACIMainViewModel(QObject *parent=0);
    virtual ~ACIMainViewModel();

    ACIListModel* getModel(){ return m_mainMenu; }

signals:
    void listModelChanged(QObject* aNewModel);
private:
    ACIListModel *m_mainMenu;
};


class ACIMainview : public QQuickView
{
    Q_OBJECT
public:
    explicit ACIMainview(QQuickView *parent = 0);
    void setQmlFile(QString qml);
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);

Q_SIGNALS:
    void navigateToWidget(int);
    void navigateToPreviousWidget();


public slots:
    void onBroadcastCtrlEvent(QString event);
    void enterNavigation();    
    void navigateTo(int widget);
    void handleRot(int direction);
    void updateMe();
    void screenSelected(int);
    void exitVideo();
    void loadSettings();



private:
    //structure with current and previous USB controller signal
    ACIUsbCtrlSignals m_sCtrl, m_sCtrlPrev;
    bool m_bCtrlFirstRun;
    QString m_sPreviousSignal;
    ACIMedia *m_oMedia;
    ACIVideoView *m_oVideoView;    
    ACISettings *m_oSettings;
    QObject *m_oCurrentView;
    ACIPageNavigation *m_oPageNavigation;
    ACIMainViewModel *m_oMainViewModel;
};

#endif // ACIMAINVIEW_H
