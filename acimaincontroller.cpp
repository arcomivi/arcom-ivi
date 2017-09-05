#include "acimaincontroller.h"

//!
//! \brief ACIMainController::ACIMainController
//! \param parent
//!
ACIMainController::ACIMainController(QObject *parent) : QObject(parent) {
    m_videoView = Q_NULLPTR;
    m_ctrlFirstRun=false;
    m_pageNavigation = Q_NULLPTR;
    m_settings = new ACISettings();
    m_media = new ACIMedia();
    m_steerings = new ACISteerings();

    m_mainView = new ACIMainview();
    m_mainView->setup(m_settings, m_media, m_steerings);
    m_pageNavigation=m_mainView->getPageNav();
}

//!
//! \brief ACIMainController::run - start the main application
//!
void ACIMainController::run() {
    //signals from objects:
    connect(m_settings, SIGNAL(screenSelected(int)), this, SLOT(screenSelected(int)));

    QObject::connect(ACIUsbController::getInstance(), SIGNAL(broadcastCtrlEvent(QString)), this, SLOT(onBroadcastCtrlEvent(QString)));
    QTimer::singleShot(500, ACIUsbController::getInstance(), SLOT(connectCtrlSignal()));

    //    gets information of the available desktops screens
    QDesktopWidget *desktopWidget = QApplication::desktop();
    //w.setAttribute(Qt::WA_TranslucentBackground);
    //w.setAutoFillBackground(false);
    //w.setAttribute(Qt::WA_OpaquePaintEvent, true);
    //w.setAttribute(Qt::WA_NoSystemBackground);

    m_mainView->setQmlFile(ACIConfig::instance()->getQmlPrefix()+"MainView.qml");
    m_mainView->setFlags(Qt::FramelessWindowHint);
    m_mainView->setResizeMode(QQuickView::SizeRootObjectToView);
    m_mainView->setGeometry(ACIConfig::instance()->getx(),
                            ACIConfig::instance()->gety(),
                            ACIConfig::instance()->getw()==0?desktopWidget->screenGeometry(0).width():ACIConfig::instance()->getw(),
                            ACIConfig::instance()->geth()==0?desktopWidget->screenGeometry(0).height():ACIConfig::instance()->geth());
    m_mainView->show();
}

//!
//! \brief ACIMainController::screenSelected
//! \param screen
//!
void ACIMainController::screenSelected(int screen) {
    if(!m_videoView) {
        m_videoView = new ACIVideoView();
        m_pageNavigation = m_videoView->getPageNav();
        m_videoView->rootContext()->setContextProperty("$steerings", m_steerings);
        m_videoView->setQmlFile(ACIConfig::instance()->getQmlPrefix()+"ACIVideoView.qml");
        m_videoView->setFlags(Qt::FramelessWindowHint);
        m_videoView->setResizeMode(QQuickView::SizeRootObjectToView);

        m_videoView->m_sCurrentVideo = m_media->getCurrentVideo();

        connect((QObject*)m_videoView->rootObject(), SIGNAL(exitVideo()), this, SLOT(exitVideo()));
    }

    QDesktopWidget *desktopWidget = QApplication::desktop();
    m_videoView->setGeometry(ACIConfig::instance()->getx(),
                             ACIConfig::instance()->gety(),
                             ACIConfig::instance()->getw()==0?desktopWidget->screenGeometry(screen).width():ACIConfig::instance()->getw(),
                             ACIConfig::instance()->geth()==0?desktopWidget->screenGeometry(screen).height():ACIConfig::instance()->geth());
    qDebug() << "desktopWidget->screenGeometry(screen).width(): " << desktopWidget->screenGeometry(screen).width();
    qDebug() << "desktopWidget->screenGeometry(screen).height(): " << desktopWidget->screenGeometry(screen).height();
    m_videoView->show();
    m_videoView->setScreen(qApp->screens()[screen]);
}

//!
//! \brief ACIMainController::exitVideo
//!
void ACIMainController::exitVideo() {
    //TODO: investivate hide/show/focus
    //    m_oVideoView->hide();
    //    this->show();
    m_videoView->destroy();
    m_videoView = Q_NULLPTR;
    m_pageNavigation=m_mainView->getPageNav();
}

//!
//! \brief ACIMainController::navigateTo
//! \param widget
//!
void ACIMainController::navigateTo(int widget) {
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

void ACIMainController::onBroadcastCtrlEvent(QString event) {

    //if it's not the first run, compare with previous signal
    //if equals, do nothing
    if(!m_ctrlFirstRun) {
        if(event.compare(m_previousSignal)==0) {
            return;
        }
    }
    TRACE(QString("%1 -> %2").arg(m_previousSignal).arg(event));

    //read event signals:
    //volume rotation signals
    m_ctrlSignal.vol_rot1 = event.at(0).digitValue();
    m_ctrlSignal.vol_rot2 = event.at(1).digitValue();

    //push signal
    m_ctrlSignal.push = event.at(2).digitValue();

    //direction signals
    m_ctrlSignal.dir_left = event.at(3).digitValue();
    m_ctrlSignal.dir_right = event.at(4).digitValue();
    m_ctrlSignal.dir_down = event.at(5).digitValue();
    m_ctrlSignal.dir_up = event.at(6).digitValue();

    //rotation signals
    m_ctrlSignal.rot1 = event.at(7).digitValue();
    m_ctrlSignal.rot2 = event.at(8).digitValue();

    //push button signal
    m_ctrlSignal.vol_push = event.at(10).digitValue();

    //first run
    if(m_ctrlFirstRun) {
        TRACE("first run");
        m_ctrlFirstRun = false;
        m_ctrlPreviousSignal = m_ctrlSignal;
        TRACE(QString("rotation: %1%2 -> %3%4").arg(m_ctrlPreviousSignal.rot1).arg(m_ctrlPreviousSignal.rot2).arg(m_ctrlSignal.rot1).arg(m_ctrlSignal.rot2));
        return;
    }
    m_previousSignal=event;


    //now do the comparison
    //    TRACE("volume push");
    if(m_ctrlSignal.vol_push == 1 && m_ctrlPreviousSignal.vol_push == 0) {
        m_ctrlPreviousSignal.vol_push = m_ctrlSignal.vol_push;
        //        QMetaObject::invokeMethod(m_wMedia, "handleVolPush", Qt::DirectConnection);
        return;
    }
    m_ctrlPreviousSignal.vol_push = m_ctrlSignal.vol_push;

    //    TRACE("push");
    //push is pressed and last was not pressed
    if(m_ctrlSignal.push == 1 && m_ctrlPreviousSignal.push == 0) {
        m_ctrlPreviousSignal.push = m_ctrlSignal.push;
        //QMetaObject::invokeMethod((QObject*)m_oMainview->rootObject(), "handlePush", Qt::DirectConnection);
        emit m_pageNavigation->handlePush();
        return;
    }

    //push button released
    if(m_ctrlSignal.push == 0 && m_ctrlPreviousSignal.push == 1) {
        m_ctrlPreviousSignal.push = m_ctrlSignal.push;
        //QMetaObject::invokeMethod((QObject*)m_oMainview->rootObject(), "handleRelease", Qt::DirectConnection);
        emit m_pageNavigation->handleRelease();
        return;
    }
    //if not push or release, assign the last value
    m_ctrlPreviousSignal.push = m_ctrlSignal.push;

    //    TRACE(QString("rotation: %1%2 -> %3%4").arg(m_sCtrlPrev.rot1).arg(m_sCtrlPrev.rot2).arg(m_sCtrl.rot1).arg(m_sCtrl.rot2));
    //===>rotation clockwise
    //00 -> 10, 10->11, 11->01, 01->00
    if((m_ctrlPreviousSignal.rot1 == 0 && m_ctrlPreviousSignal.rot2 == 0 &&
            m_ctrlSignal.rot1 == 1 && m_ctrlSignal.rot2 == 0) ||
            (m_ctrlPreviousSignal.rot1 == 1 && m_ctrlPreviousSignal.rot2 == 0 &&
             m_ctrlSignal.rot1 == 1 && m_ctrlSignal.rot2 == 1) ||
            (m_ctrlPreviousSignal.rot1 == 1 && m_ctrlPreviousSignal.rot2 == 1 &&
             m_ctrlSignal.rot1 == 0 && m_ctrlSignal.rot2 == 1) ||
            (m_ctrlPreviousSignal.rot1 == 0 && m_ctrlPreviousSignal.rot2 == 1 &&
             m_ctrlSignal.rot1 == 0 && m_ctrlSignal.rot2 == 0) ) {
        TRACE("rotation clockwise");
        //QMetaObject::invokeMethod((QObject*)m_oMainview->rootObject(), "handleRot", Qt::DirectConnection, Q_ARG(QVariant, 0));
        emit m_pageNavigation->handleRot(0);

        m_ctrlPreviousSignal.rot1 = m_ctrlSignal.rot1;
        m_ctrlPreviousSignal.rot2 = m_ctrlSignal.rot2;
        return;
    }

    //===>rotation counter clockwise
    //00<-10, 10<-11, 11<-01, 01<-00
    if((m_ctrlPreviousSignal.rot1 == 0 && m_ctrlPreviousSignal.rot2 == 0 &&
            m_ctrlSignal.rot1 == 0 && m_ctrlSignal.rot2 == 1) ||
            (m_ctrlPreviousSignal.rot1 == 0 && m_ctrlPreviousSignal.rot2 == 1 &&
             m_ctrlSignal.rot1 == 1 && m_ctrlSignal.rot2 == 1) ||
            (m_ctrlPreviousSignal.rot1 == 1 && m_ctrlPreviousSignal.rot2 == 1 &&
             m_ctrlSignal.rot1 == 1 && m_ctrlSignal.rot2 == 0) ||
            (m_ctrlPreviousSignal.rot1 == 1 && m_ctrlPreviousSignal.rot2 == 0 &&
             m_ctrlSignal.rot1 == 0 && m_ctrlSignal.rot2 == 0) ) {
        TRACE("rotation counter clockwise");
        //QMetaObject::invokeMethod((QObject*)m_oMainview->rootObject(), "handleRot", Qt::DirectConnection, Q_ARG(QVariant, 1));
        emit m_pageNavigation->handleRot(1);

        m_ctrlPreviousSignal.rot1 = m_ctrlSignal.rot1;
        m_ctrlPreviousSignal.rot2 = m_ctrlSignal.rot2;
        return;
    }
    //===>directions:
    //down
    if(m_ctrlSignal.dir_down == 1 && m_ctrlPreviousSignal.dir_down==0) {
        m_ctrlPreviousSignal.dir_down = m_ctrlSignal.dir_down;
        //QMetaObject::invokeMethod((QObject*)m_oMainview->rootObject(), "handleDirDown", Qt::DirectConnection);
        emit m_pageNavigation->handleDirDown();
        return;
    }
    m_ctrlPreviousSignal.dir_down = m_ctrlSignal.dir_down;
    //up
    if(m_ctrlSignal.dir_up == 1 && m_ctrlPreviousSignal.dir_up==0) {
        m_ctrlPreviousSignal.dir_up = m_ctrlSignal.dir_up;
        //QMetaObject::invokeMethod((QObject*)m_oMainview->rootObject(), "handleDirUp", Qt::DirectConnection);
        emit m_pageNavigation->handleDirUp();
        return;
    }
    m_ctrlPreviousSignal.dir_up = m_ctrlSignal.dir_up;
    //left
    if(m_ctrlSignal.dir_left == 1 && m_ctrlPreviousSignal.dir_left==0) {
        m_ctrlPreviousSignal.dir_left = m_ctrlSignal.dir_left;
        //        QMetaObject::invokeMethod(m_wCurrentWidget, "handleDirLeft", Qt::DirectConnection);
        return;
    }
    m_ctrlPreviousSignal.dir_left = m_ctrlSignal.dir_left;
    //right
    if(m_ctrlSignal.dir_right == 1 && m_ctrlPreviousSignal.dir_right==0) {
        m_ctrlPreviousSignal.dir_right = m_ctrlSignal.dir_right;
        //        QMetaObject::invokeMethod(m_wCurrentWidget, "handleDirRight", Qt::DirectConnection);
        return;
    }
    m_ctrlPreviousSignal.dir_right = m_ctrlSignal.dir_right;


    //===>rotation clockwise
    //00 -> 10, 10->11, 11->01, 01->00
    if((m_ctrlPreviousSignal.vol_rot1 == 0 && m_ctrlPreviousSignal.vol_rot2 == 0 &&
            m_ctrlSignal.vol_rot1 == 1 && m_ctrlSignal.vol_rot2 == 0) ||
            (m_ctrlPreviousSignal.vol_rot1 == 1 && m_ctrlPreviousSignal.vol_rot2 == 0 &&
             m_ctrlSignal.vol_rot1 == 1 && m_ctrlSignal.vol_rot2 == 1) ||
            (m_ctrlPreviousSignal.vol_rot1 == 1 && m_ctrlPreviousSignal.vol_rot2 == 1 &&
             m_ctrlSignal.vol_rot1 == 0 && m_ctrlSignal.vol_rot2 == 1) ||
            (m_ctrlPreviousSignal.vol_rot1 == 0 && m_ctrlPreviousSignal.vol_rot2 == 1 &&
             m_ctrlSignal.vol_rot1 == 0 && m_ctrlSignal.vol_rot2 == 0) ) {


        m_ctrlPreviousSignal.vol_rot1 = m_ctrlSignal.vol_rot1;
        m_ctrlPreviousSignal.vol_rot2 = m_ctrlSignal.vol_rot2;
        return;
    }

    //===>rotation counter clockwise
    //00<-10, 10<-11, 11<-01, 01<-00
    if((m_ctrlPreviousSignal.vol_rot1 == 0 && m_ctrlPreviousSignal.vol_rot2 == 0 &&
            m_ctrlSignal.vol_rot1 == 0 && m_ctrlSignal.vol_rot2 == 1) ||
            (m_ctrlPreviousSignal.vol_rot1 == 0 && m_ctrlPreviousSignal.vol_rot2 == 1 &&
             m_ctrlSignal.vol_rot1 == 1 && m_ctrlSignal.vol_rot2 == 1) ||
            (m_ctrlPreviousSignal.vol_rot1 == 1 && m_ctrlPreviousSignal.vol_rot2 == 1 &&
             m_ctrlSignal.vol_rot1 == 1 && m_ctrlSignal.vol_rot2 == 0) ||
            (m_ctrlPreviousSignal.vol_rot1 == 1 && m_ctrlPreviousSignal.vol_rot2 == 0 &&
             m_ctrlSignal.vol_rot1 == 0 && m_ctrlSignal.vol_rot2 == 0) ) {


        m_ctrlPreviousSignal.vol_rot1 = m_ctrlSignal.vol_rot1;
        m_ctrlPreviousSignal.vol_rot2 = m_ctrlSignal.vol_rot2;
        return;
    }
}
