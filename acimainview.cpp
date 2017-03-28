#include <QGuiApplication>
#include <QWindow>
#include <QMainWindow>
#include <QScreen>
#include <QQmlContext>
#include "acimainview.h"


ACIMainview::ACIMainview(QQuickView *parent) :
    QQuickView(parent)
{
    m_bCtrlFirstRun=true;
}

void ACIMainview::setQmlFile(QString qml){
    m_oMedia = new ACIMedia();
    m_oVideoView = 0;

    this->setSource(QUrl(qml));
    m_oCurrentView = (QObject*)this->rootObject();

    //signals from objects:
    connect(m_oMedia, SIGNAL(mediaChanged()), this , SLOT(loadMedia()));
    connect(m_oMedia, SIGNAL(sendProgress(int)), this , SLOT(sendProgress(int)));
    connect(m_oMedia, SIGNAL(watchVideo(QString)), this , SLOT(watchVideo(QString)));

    //signals from QML UI:
    QObject::connect((QObject*)this->rootObject(), SIGNAL(volup()), m_oMedia , SLOT(volup()));
    QObject::connect((QObject*)this->rootObject(), SIGNAL(voldown()), m_oMedia , SLOT(voldown()));
    QObject::connect((QObject*)this->rootObject(), SIGNAL(screenSelected(int)), this , SLOT(screenSelected(int)));
    QObject::connect((QObject*)this->rootObject(), SIGNAL(loadMedia()), this , SLOT(loadMedia()));
    QObject::connect((QObject*)this->rootObject(), SIGNAL(update()), this , SLOT(updateMe()));
    QObject::connect((QObject*)this->rootObject(), SIGNAL(navigateTo(int)), this , SLOT(navigateTo(int)));

    QObject::connect(ACIUsbController::getInstance(), SIGNAL(broadcastCtrlEvent(QString)), this, SLOT(onBroadcastCtrlEvent(QString)));
    QTimer::singleShot(500, ACIUsbController::getInstance(), SLOT(connectCtrlSignal()));
}

void ACIMainview::loadMedia(){
    m_oMedia->loadMedia();
    this->rootContext()->setContextProperty(QLatin1String("listModel"), QVariant::fromValue(m_oMedia->getModel()));
}

void ACIMainview::sendProgress(int progress){
    QMetaObject::invokeMethod((QObject*)this->rootObject(), "sendProgress", Q_ARG(QVariant, progress));
}

void ACIMainview::watchVideo(QString video){
    QMetaObject::invokeMethod((QObject*)this->rootObject(), "chooseScreen", Qt::DirectConnection);
}

void ACIMainview::screenSelected(int screen){
    if(!m_oVideoView){
        m_oVideoView  = new ACIVideoView();
        m_oVideoView->setQmlFile(ACIConfig::instance()->getQmlPrefix()+"ACIVideoView.qml");
        m_oVideoView->setFlags(Qt::FramelessWindowHint);
        m_oVideoView->setResizeMode(QQuickView::SizeRootObjectToView);
        m_oVideoView->m_sCurrentVideo = m_oMedia->getCurrentVideo();

        connect((QObject*)m_oVideoView->rootObject(), SIGNAL(exitVideo()), this , SLOT(exitVideo()));

    }
    m_oVideoView->setGeometry(650,150,640,480);
    m_oVideoView->show();
    m_oVideoView->setScreen(qApp->screens()[screen]);
}


void ACIMainview::exitVideo(){
    m_oVideoView->destroy();
    m_oVideoView = 0;
}

void ACIMainview::keyPressEvent(QKeyEvent *e){
    TRACE(e->key());
    //(push) (rot rot) (dirl dirr dird diru)
    //    //0 00 0000 000
    //    if(e->key() == Qt::Key_E){ //E-for emergency
    //        QMetaObject::invokeMethod(m_oCurrentObject, "handleDemoEmergencyCustOrder", Qt::DirectConnection);
    //        return;
    //    }

    if(e->key() == Qt::Key_W){ //UP
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleDirUp", Qt::DirectConnection);
        return;
    }
    if(e->key() == Qt::Key_Z){ //DOWN
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleDirDown", Qt::DirectConnection);
        return;
    }
    if(e->key() == Qt::Key_A){ //LEFT
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleDirLeft", Qt::DirectConnection);
        return;
    }
    if(e->key() == Qt::Key_D){ //RIGHT
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleDirRight", Qt::DirectConnection);
        return;
    }
    if(e->key() == Qt::Key_P){ //PUSHB
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handlePush", Qt::DirectConnection);
        return;
    }
    if(e->key() == Qt::Key_R){ //release PUSHB
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleRelease", Qt::DirectConnection);
        return;
    }
    if(e->key() == Qt::Key_M){ //ROT1
        QMetaObject::invokeMethod(m_oCurrentView, "handleRot", Qt::DirectConnection, Q_ARG(QVariant, 0));
        return;
    }
    if(e->key() == Qt::Key_N){ //ROT2
        QMetaObject::invokeMethod(m_oCurrentView, "handleRot", Qt::DirectConnection, Q_ARG(QVariant, 1));
        return;
    }

    QQuickView::keyPressEvent(e);
}

void ACIMainview::onBroadcastCtrlEvent(QString event){

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
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handlePush", Qt::DirectConnection);
        return;
    }

    //push button released
    if(m_sCtrl.push == 0 && m_sCtrlPrev.push == 1){
        m_sCtrlPrev.push = m_sCtrl.push;
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleRelease", Qt::DirectConnection);
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
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleRot", Qt::DirectConnection, Q_ARG(QVariant, 0));

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
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleRot", Qt::DirectConnection, Q_ARG(QVariant, 1));

        m_sCtrlPrev.rot1 = m_sCtrl.rot1;
        m_sCtrlPrev.rot2 = m_sCtrl.rot2;
        return;
    }
    //===>directions:
    //down
    if(m_sCtrl.dir_down == 1 && m_sCtrlPrev.dir_down==0){
        m_sCtrlPrev.dir_down = m_sCtrl.dir_down;
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleDirDown", Qt::DirectConnection);
        return;
    }
    m_sCtrlPrev.dir_down = m_sCtrl.dir_down;
    //up
    if(m_sCtrl.dir_up == 1 && m_sCtrlPrev.dir_up==0){
        m_sCtrlPrev.dir_up = m_sCtrl.dir_up;
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleDirUp", Qt::DirectConnection);
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

void ACIMainview::enterNavigation(){
    emit navigateToWidget(ViewNavigation);
}

void ACIMainview::navigateTo(int widget){
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

//    qDebug() << window->windowFlags();
    navWindow->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);// | Qt::BypassWindowManagerHint);
    navWindow->move(0,75);
    navWindow->setFixedSize(
            QGuiApplication::screens().at(0)->availableGeometry().width(),
            350);
    navWindow->setCentralWidget(navWidget);
    navWindow->show();
}


/** ===============================
 *  handleRot(int direction) - rotation handler
 */
void ACIMainview::handleRot(int direction) {
    if(direction==0){
        QMetaObject::invokeMethod((QObject*)rootObject(), "rotateCW");
    }else {
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "rotateCCW");
    }
}

#include <QStorageInfo>
void ACIMainview::updateMe(){
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
            if(storage.rootPath() == "/"){
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
        if(dir.exists()){
            //check if there is an updater.sh script as well
            QString updateScript("%1/updater.sh");
            QFile updateFile(updateScript.arg(update_dir));
            if(updateFile.exists()){
                break;
            }
        }
        TRACE(QString("updater.sh not found under %1").arg(update_dir));
        update_dir="not-found";
    }
    TRACE("updating4...");

    TRACE(QString("done searching: %1").arg(update_dir));
    //check if equals to: not-found
    if(update_dir.compare("not-found", Qt::CaseInsensitive)==0){
        TRACE("no updates found");
////        emit displayNotification("No update folder found!");

        return;
    }


//    //0. do some checks
    if(update_dir==""){
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
