#include "acimaincontroller.h"

//!
//! \brief ACIMainController::ACIMainController
//! \param parent
//!
ACIMainController::ACIMainController(QObject *parent) : QObject(parent){
    m_oVideoView = Q_NULLPTR;
    m_bCtrlFirstRun=false;
}

//!
//! \brief ACIMainController::run - start the main application
//!
void ACIMainController::run(){

    m_oPageNavigation = new ACIPageNavigation();

    m_oSettings=Q_NULLPTR;
    m_oSettings = new ACISettings();
    m_oSettings->setPageNavigation(m_oPageNavigation);

    m_oMedia = new ACIMedia();
    m_oMedia->setPageNavigation(m_oPageNavigation);

    m_oSteerings = new ACISteerings();

    //signals from objects:
    connect(m_oSettings, SIGNAL(update()), m_oSettings, SLOT(updateMe()));
    connect(m_oSettings, SIGNAL(screenSelected(int)), this, SLOT(screenSelected(int)));

    connect(m_oSteerings, SIGNAL(play()), m_oMedia, SLOT(play()));
    connect(m_oSteerings, SIGNAL(volup()), m_oMedia, SLOT(volup()));
    connect(m_oSteerings, SIGNAL(voldown()), m_oMedia, SLOT(voldown()));

    QObject::connect(ACIUsbController::getInstance(), SIGNAL(broadcastCtrlEvent(QString)), this, SLOT(onBroadcastCtrlEvent(QString)));
    QTimer::singleShot(500, ACIUsbController::getInstance(), SLOT(connectCtrlSignal()));

    //    gets information of the available desktops screens
    QDesktopWidget *desktopWidget = QApplication::desktop();

    oMainview = new ACIMainview(m_oPageNavigation);

    //w.setAttribute(Qt::WA_TranslucentBackground);
    //w.setAutoFillBackground(false);
    //w.setAttribute(Qt::WA_OpaquePaintEvent, true);
    //w.setAttribute(Qt::WA_NoSystemBackground);

    connect(oMainview, SIGNAL(loadMedia()), m_oMedia, SLOT(loadMedia()));
    //context properties must be loaded before loading the QML, if possible ;-)
    oMainview->rootContext()->setContextProperty("$settings", m_oSettings);
    oMainview->rootContext()->setContextProperty("$media", m_oMedia);
    oMainview->rootContext()->setContextProperty("$pageNavigation", m_oPageNavigation);

    oMainview->rootContext()->setContextProperty("$steerings", m_oSteerings);



    oMainview->setQmlFile(ACIConfig::instance()->getQmlPrefix()+"MainView.qml");
    oMainview->setFlags(Qt::FramelessWindowHint);
    oMainview->setResizeMode(QQuickView::SizeRootObjectToView);

    oMainview->setGeometry(ACIConfig::instance()->getx(),
                           ACIConfig::instance()->gety(),
                           ACIConfig::instance()->getw()==0?desktopWidget->screenGeometry(0).width():ACIConfig::instance()->getw(),
                           ACIConfig::instance()->geth()==0?desktopWidget->screenGeometry(0).height():ACIConfig::instance()->geth());
    oMainview->show();
}

//!
//! \brief ACIMainController::screenSelected
//! \param screen
//!
void ACIMainController::screenSelected(int screen){
    if(!m_oVideoView){
        m_oVideoView = new ACIVideoView();

        m_oVideoView->rootContext()->setContextProperty("$steerings", m_oSteerings);
        m_oVideoView->rootContext()->setContextProperty("$pageNavigation", m_oPageNavigation);

        m_oVideoView->setQmlFile(ACIConfig::instance()->getQmlPrefix()+"ACIVideoView.qml");
        m_oVideoView->setFlags(Qt::FramelessWindowHint);
        m_oVideoView->setResizeMode(QQuickView::SizeRootObjectToView);

        m_oVideoView->m_sCurrentVideo = m_oMedia->getCurrentVideo();

        connect((QObject*)m_oVideoView->rootObject(), SIGNAL(exitVideo()), this , SLOT(exitVideo()));
    }

    QDesktopWidget *desktopWidget = QApplication::desktop();
    m_oVideoView->setGeometry(ACIConfig::instance()->getx(),
                              ACIConfig::instance()->gety(),
                              ACIConfig::instance()->getw()==0?desktopWidget->screenGeometry(screen).width():ACIConfig::instance()->getw(),
                              ACIConfig::instance()->geth()==0?desktopWidget->screenGeometry(screen).height():ACIConfig::instance()->geth());
    m_oVideoView->show();
    m_oVideoView->setScreen(qApp->screens()[screen]);
}

//!
//! \brief ACIMainController::exitVideo
//!
void ACIMainController::exitVideo(){
    //TODO: investivate hide/show/focus
//    m_oVideoView->hide();
//    this->show();
    m_oVideoView->destroy();
    m_oVideoView = 0;
}

//!
//! \brief ACIMainController::navigateTo
//! \param widget
//!
void ACIMainController::navigateTo(int widget){
    TRACE(widget);
    QProcess xwininfo;
    xwininfo.start("sh", QStringList() << "-c" << "xwininfo -int -name navit | grep \"Window id:\"");// | grep -o '[0-9]\{1,10\}'");
    xwininfo.waitForStarted();
    xwininfo.waitForFinished();
    QByteArray output = xwininfo.readAll();
    QString out = QString::fromStdString(output.toStdString());
    QRegExp rx("([0-9]{1,10})");
    if (rx.indexIn(out) == -1) {
        qDebug() << "not found!";
        return;
    }
    QString navitid = QString::fromStdString(rx.cap(1).toStdString());
    qDebug() << "Arguments: " << out << "Navit ID: " << rx.cap(1);

    QWindow *navwin = QWindow::fromWinId(navitid.toInt());
    navwin->setFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::BypassWindowManagerHint);
    QWidget *navWidget = QWidget::createWindowContainer(navwin, nullptr, Qt::Widget);

    QMainWindow *navWindow = new QMainWindow();

    navWindow->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);// | Qt::BypassWindowManagerHint);
    navWindow->move(0,75);
    navWindow->setFixedSize(
                QGuiApplication::screens().at(0)->availableGeometry().width(),
                350);
    navWindow->setCentralWidget(navWidget);
    navWindow->show();
}

void ACIMainController::onBroadcastCtrlEvent(QString event){

    //if it's not the first run, compare with previous signal
    //if equals, do nothing
    if(!m_bCtrlFirstRun){
        if(event.compare(m_sPreviousSignal)==0){
            return;
        }
    }
    TRACE(QString("%1 -> %2").arg(m_sPreviousSignal).arg(event));

    //read event signals:
    //volume rotation signals
    m_sCtrl.vol_rot1 = event.at(0).digitValue();
    m_sCtrl.vol_rot2 = event.at(1).digitValue();

    //push signal
    m_sCtrl.push = event.at(2).digitValue();

    //direction signals
    m_sCtrl.dir_left = event.at(3).digitValue();
    m_sCtrl.dir_right = event.at(4).digitValue();
    m_sCtrl.dir_down = event.at(5).digitValue();
    m_sCtrl.dir_up = event.at(6).digitValue();

    //rotation signals
    m_sCtrl.rot1 = event.at(7).digitValue();
    m_sCtrl.rot2 = event.at(8).digitValue();

    //push button signal
    m_sCtrl.vol_push = event.at(10).digitValue();

    //first run
    if(m_bCtrlFirstRun){
        TRACE("first run");
        m_bCtrlFirstRun = false;
        m_sCtrlPrev = m_sCtrl;
        TRACE(QString("rotation: %1%2 -> %3%4").arg(m_sCtrlPrev.rot1).arg(m_sCtrlPrev.rot2).arg(m_sCtrl.rot1).arg(m_sCtrl.rot2));
        return;
    }
    m_sPreviousSignal=event;


    //now do the comparison
    //    TRACE("volume push");
    if(m_sCtrl.vol_push == 1 && m_sCtrlPrev.vol_push == 0){
        m_sCtrlPrev.vol_push = m_sCtrl.vol_push;
        //        QMetaObject::invokeMethod(m_wMedia, "handleVolPush", Qt::DirectConnection);
        return;
    }
    m_sCtrlPrev.vol_push = m_sCtrl.vol_push;

    //    TRACE("push");
    //push is pressed and last was not pressed
    if(m_sCtrl.push == 1 && m_sCtrlPrev.push == 0){
        m_sCtrlPrev.push = m_sCtrl.push;
        QMetaObject::invokeMethod((QObject*)oMainview->rootObject(), "handlePush", Qt::DirectConnection);
        return;
    }

    //push button released
    if(m_sCtrl.push == 0 && m_sCtrlPrev.push == 1){
        m_sCtrlPrev.push = m_sCtrl.push;
        QMetaObject::invokeMethod((QObject*)oMainview->rootObject(), "handleRelease", Qt::DirectConnection);
        return;
    }
    //if not push or release, assign the last value
    m_sCtrlPrev.push = m_sCtrl.push;

    //    TRACE(QString("rotation: %1%2 -> %3%4").arg(m_sCtrlPrev.rot1).arg(m_sCtrlPrev.rot2).arg(m_sCtrl.rot1).arg(m_sCtrl.rot2));
    //===>rotation clockwise
    //00 -> 10, 10->11, 11->01, 01->00
    if((m_sCtrlPrev.rot1 == 0 && m_sCtrlPrev.rot2 == 0 &&
        m_sCtrl.rot1 == 1 && m_sCtrl.rot2 == 0) ||
            (m_sCtrlPrev.rot1 == 1 && m_sCtrlPrev.rot2 == 0 &&
             m_sCtrl.rot1 == 1 && m_sCtrl.rot2 == 1) ||
            (m_sCtrlPrev.rot1 == 1 && m_sCtrlPrev.rot2 == 1 &&
             m_sCtrl.rot1 == 0 && m_sCtrl.rot2 == 1) ||
            (m_sCtrlPrev.rot1 == 0 && m_sCtrlPrev.rot2 == 1 &&
             m_sCtrl.rot1 == 0 && m_sCtrl.rot2 == 0) ){
        TRACE("rotation clockwise");
        QMetaObject::invokeMethod((QObject*)oMainview->rootObject(), "handleRot", Qt::DirectConnection, Q_ARG(QVariant, 0));

        m_sCtrlPrev.rot1 = m_sCtrl.rot1;
        m_sCtrlPrev.rot2 = m_sCtrl.rot2;
        return;
    }

    //===>rotation counter clockwise
    //00<-10, 10<-11, 11<-01, 01<-00
    if((m_sCtrlPrev.rot1 == 0 && m_sCtrlPrev.rot2 == 0 &&
        m_sCtrl.rot1 == 0 && m_sCtrl.rot2 == 1) ||
            (m_sCtrlPrev.rot1 == 0 && m_sCtrlPrev.rot2 == 1 &&
             m_sCtrl.rot1 == 1 && m_sCtrl.rot2 == 1) ||
            (m_sCtrlPrev.rot1 == 1 && m_sCtrlPrev.rot2 == 1 &&
             m_sCtrl.rot1 == 1 && m_sCtrl.rot2 == 0) ||
            (m_sCtrlPrev.rot1 == 1 && m_sCtrlPrev.rot2 == 0 &&
             m_sCtrl.rot1 == 0 && m_sCtrl.rot2 == 0) ){
        TRACE("rotation counter clockwise");
        QMetaObject::invokeMethod((QObject*)oMainview->rootObject(), "handleRot", Qt::DirectConnection, Q_ARG(QVariant, 1));

        m_sCtrlPrev.rot1 = m_sCtrl.rot1;
        m_sCtrlPrev.rot2 = m_sCtrl.rot2;
        return;
    }
    //===>directions:
    //down
    if(m_sCtrl.dir_down == 1 && m_sCtrlPrev.dir_down==0){
        m_sCtrlPrev.dir_down = m_sCtrl.dir_down;
        QMetaObject::invokeMethod((QObject*)oMainview->rootObject(), "handleDirDown", Qt::DirectConnection);
        return;
    }
    m_sCtrlPrev.dir_down = m_sCtrl.dir_down;
    //up
    if(m_sCtrl.dir_up == 1 && m_sCtrlPrev.dir_up==0){
        m_sCtrlPrev.dir_up = m_sCtrl.dir_up;
        QMetaObject::invokeMethod((QObject*)oMainview->rootObject(), "handleDirUp", Qt::DirectConnection);
        return;
    }
    m_sCtrlPrev.dir_up = m_sCtrl.dir_up;
    //left
    if(m_sCtrl.dir_left == 1 && m_sCtrlPrev.dir_left==0){
        m_sCtrlPrev.dir_left = m_sCtrl.dir_left;
        //        QMetaObject::invokeMethod(m_wCurrentWidget, "handleDirLeft", Qt::DirectConnection);
        return;
    }
    m_sCtrlPrev.dir_left = m_sCtrl.dir_left;
    //right
    if(m_sCtrl.dir_right == 1 && m_sCtrlPrev.dir_right==0){
        m_sCtrlPrev.dir_right = m_sCtrl.dir_right;
        //        QMetaObject::invokeMethod(m_wCurrentWidget, "handleDirRight", Qt::DirectConnection);
        return;
    }
    m_sCtrlPrev.dir_right = m_sCtrl.dir_right;


    //===>rotation clockwise
    //00 -> 10, 10->11, 11->01, 01->00
    if((m_sCtrlPrev.vol_rot1 == 0 && m_sCtrlPrev.vol_rot2 == 0 &&
        m_sCtrl.vol_rot1 == 1 && m_sCtrl.vol_rot2 == 0) ||
            (m_sCtrlPrev.vol_rot1 == 1 && m_sCtrlPrev.vol_rot2 == 0 &&
             m_sCtrl.vol_rot1 == 1 && m_sCtrl.vol_rot2 == 1) ||
            (m_sCtrlPrev.vol_rot1 == 1 && m_sCtrlPrev.vol_rot2 == 1 &&
             m_sCtrl.vol_rot1 == 0 && m_sCtrl.vol_rot2 == 1) ||
            (m_sCtrlPrev.vol_rot1 == 0 && m_sCtrlPrev.vol_rot2 == 1 &&
             m_sCtrl.vol_rot1 == 0 && m_sCtrl.vol_rot2 == 0) ){


        m_sCtrlPrev.vol_rot1 = m_sCtrl.vol_rot1;
        m_sCtrlPrev.vol_rot2 = m_sCtrl.vol_rot2;
        return;
    }

    //===>rotation counter clockwise
    //00<-10, 10<-11, 11<-01, 01<-00
    if((m_sCtrlPrev.vol_rot1 == 0 && m_sCtrlPrev.vol_rot2 == 0 &&
        m_sCtrl.vol_rot1 == 0 && m_sCtrl.vol_rot2 == 1) ||
            (m_sCtrlPrev.vol_rot1 == 0 && m_sCtrlPrev.vol_rot2 == 1 &&
             m_sCtrl.vol_rot1 == 1 && m_sCtrl.vol_rot2 == 1) ||
            (m_sCtrlPrev.vol_rot1 == 1 && m_sCtrlPrev.vol_rot2 == 1 &&
             m_sCtrl.vol_rot1 == 1 && m_sCtrl.vol_rot2 == 0) ||
            (m_sCtrlPrev.vol_rot1 == 1 && m_sCtrlPrev.vol_rot2 == 0 &&
             m_sCtrl.vol_rot1 == 0 && m_sCtrl.vol_rot2 == 0) ){


        m_sCtrlPrev.vol_rot1 = m_sCtrl.vol_rot1;
        m_sCtrlPrev.vol_rot2 = m_sCtrl.vol_rot2;
        return;
    }
}
