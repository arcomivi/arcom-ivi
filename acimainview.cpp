#include "acimainview.h"

//!
//! \brief ACIMainview::ACIMainview
//! \param parent
//!
ACIMainview::ACIMainview(QQuickView *parent) :
    QQuickView(parent) {
    m_mainViewModel = new ACIMainViewModel();
    m_pageNavigation = new ACIPageNavigation();
    this->rootContext()->setContextProperty("$mainViewModel", m_mainViewModel);
    this->rootContext()->setContextProperty("$pageNavigation", m_pageNavigation);
    m_mainViewModel->setPageNavigation(m_pageNavigation);
}

//!
//! \brief ACIMainview::setQmlFile
//! \param qml
//!
void ACIMainview::setQmlFile(QString qml) {
    this->setSource(QUrl(qml));
    connect(m_mainViewModel, SIGNAL(loadMedia()), this, SLOT(loadMediaView()));
    connect(m_mainViewModel, SIGNAL(loadSettings()), this, SLOT(loadSettingView()));
}

//!
//! \brief ACIMainview::keyPressEvent
//! \param e
//!
void ACIMainview::keyPressEvent(QKeyEvent *e) {
    TRACE(e->key());
    //(push) (rot rot) (dirl dirr dird diru)
    //    //0 00 0000 000
    //    if(e->key() == Qt::Key_E){ //E-for emergency
    //        QMetaObject::invokeMethod(m_oCurrentObject, "handleDemoEmergencyCustOrder", Qt::DirectConnection);
    //        return;
    //    }

    if(e->key() == Qt::Key_W) { //UP
        //        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleDirUp", Qt::DirectConnection);
        emit m_pageNavigation->handleDirUp();
        return;
    }
    if(e->key() == Qt::Key_Z) { //DOWN
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleDirDown", Qt::DirectConnection);
        emit m_pageNavigation->handleDirDown();
        return;
    }
    if(e->key() == Qt::Key_A) { //LEFT
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleDirLeft", Qt::DirectConnection);
        return;
    }
    if(e->key() == Qt::Key_D) { //RIGHT
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleDirRight", Qt::DirectConnection);
        return;
    }
    if(e->key() == Qt::Key_P) { //PUSHB
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handlePush", Qt::DirectConnection);
        return;
    }
    if(e->key() == Qt::Key_R) { //release PUSHB
        //QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleRelease", Qt::DirectConnection);
        emit m_pageNavigation->handleRelease();
        return;
    }
    if(e->key() == Qt::Key_M) { //ROT1
        //QMetaObject::invokeMethod(m_oCurrentView, "handleRot", Qt::DirectConnection, Q_ARG(QVariant, 0));
        emit m_pageNavigation->handleRot(0);
        return;
    }
    if(e->key() == Qt::Key_N) { //ROT2
        //QMetaObject::invokeMethod(m_oCurrentView, "handleRot", Qt::DirectConnection, Q_ARG(QVariant, 1));
        emit m_pageNavigation->handleRot(1);
        return;
    }

    QQuickView::keyPressEvent(e);
}

void ACIMainview::mousePressEvent(QMouseEvent *e) {
    qDebug() << "e->button(): " << e->button();
    if(e->button()==Qt::NoButton     ) {
        qDebug() << "button:NoButton      " << e->button();
    }
    if(e->button()==Qt::AllButtons   ) {
        qDebug() << "button:AllButtons    " << e->button();
    }
    if(e->button()==Qt::LeftButton   ) {
        qDebug() << "button:LeftButton    " << e->button();
    }
    if(e->button()==Qt::RightButton  ) {
        qDebug() << "button:RightButton   " << e->button();
    }
    if(e->button()==Qt::MidButton    ) {
        qDebug() << "button:MidButton     " << e->button();
    }
    if(e->button()==Qt::MiddleButton ) {
        qDebug() << "button:MiddleButton  " << e->button();
    }
    if(e->button()==Qt::BackButton   ) {
        qDebug() << "button:BackButton    " << e->button();
    }
    if(e->button()==Qt::XButton1     ) {
        qDebug() << "button:XButton1      " << e->button();
    }
    if(e->button()==Qt::ExtraButton1 ) {
        qDebug() << "button:ExtraButton1  " << e->button();
    }
    if(e->button()==Qt::ForwardButton) {
        qDebug() << "button:ForwardButton " << e->button();
    }
    if(e->button()==Qt::XButton2     ) {
        qDebug() << "button:XButton2      " << e->button();
    }
    if(e->button()==Qt::ExtraButton2 ) {
        qDebug() << "button:ExtraButton2  " << e->button();
    }
    if(e->button()==Qt::TaskButton   ) {
        qDebug() << "button:TaskButton    " << e->button();
    }
    if(e->button()==Qt::ExtraButton3 ) {
        qDebug() << "button:ExtraButton3  " << e->button();
    }
    if(e->button()==Qt::ExtraButton4 ) {
        qDebug() << "button:ExtraButton4  " << e->button();
    }
    if(e->button()==Qt::ExtraButton5 ) {
        qDebug() << "button:ExtraButton5  " << e->button();
    }
    if(e->button()==Qt::ExtraButton6 ) {
        qDebug() << "button:ExtraButton6  " << e->button();
    }
    if(e->button()==Qt::ExtraButton7 ) {
        qDebug() << "button:ExtraButton7  " << e->button();
    }
    if(e->button()==Qt::ExtraButton8 ) {
        qDebug() << "button:ExtraButton8  " << e->button();
    }
    if(e->button()==Qt::ExtraButton9 ) {
        qDebug() << "button:ExtraButton9  " << e->button();
    }
    if(e->button()==Qt::ExtraButton10) {
        qDebug() << "button:ExtraButton10 " << e->button();
    }
    if(e->button()==Qt::ExtraButton11) {
        qDebug() << "button:ExtraButton11 " << e->button();
    }
    if(e->button()==Qt::ExtraButton12) {
        qDebug() << "button:ExtraButton12 " << e->button();
    }
    if(e->button()==Qt::ExtraButton13) {
        qDebug() << "button:ExtraButton13 " << e->button();
    }
    if(e->button()==Qt::ExtraButton14) {
        qDebug() << "button:ExtraButton14 " << e->button();
    }
    if(e->button()==Qt::ExtraButton15) {
        qDebug() << "button:ExtraButton15 " << e->button();
    }
    if(e->button()==Qt::ExtraButton16) {
        qDebug() << "button:ExtraButton16 " << e->button();
    }
    if(e->button()==Qt::ExtraButton17) {
        qDebug() << "button:ExtraButton17 " << e->button();
    }
    if(e->button()==Qt::ExtraButton18) {
        qDebug() << "button:ExtraButton18 " << e->button();
    }
    if(e->button()==Qt::ExtraButton19) {
        qDebug() << "button:ExtraButton19 " << e->button();
    }
    if(e->button()==Qt::ExtraButton20) {
        qDebug() << "button:ExtraButton20 " << e->button();
    }
    if(e->button()==Qt::ExtraButton21) {
        qDebug() << "button:ExtraButton21 " << e->button();
    }
    if(e->button()==Qt::ExtraButton22) {
        qDebug() << "button:ExtraButton22 " << e->button();
    }
    if(e->button()==Qt::ExtraButton23) {
        qDebug() << "button:ExtraButton23 " << e->button();
    }
    if(e->button()==Qt::ExtraButton24) {
        qDebug() << "button:ExtraButton24 " << e->button();
    }

    if(e->button()==Qt::MouseButton::MidButton || e->button()==Qt::MouseButton::MiddleButton) {
        qDebug() << "Qt::MidButton";
        //QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleRelease", Qt::DirectConnection);
        emit m_pageNavigation->handleRelease();
        return;
    }
    QQuickView::mousePressEvent(e);
}

void ACIMainview::wheelEvent(QWheelEvent *e) {
    qDebug() << "e->delta(): " << e->delta();
    if(e->delta()==120) {
        emit m_pageNavigation->handleRot(0);
        return;
    } else if(e->delta() == -120) {
        emit m_pageNavigation->handleRot(1);
        return;
    }
    QQuickView::wheelEvent(e);
}

void ACIMainview::loadMediaView() {
    emit loadMedia();
    m_pageNavigation->setCurrent(1);
    emit m_pageNavigation->loadView("ACIMediaView.qml");
    emit m_pageNavigation->loadSteering("0");
}

void ACIMainview::loadSettingView() {
    m_pageNavigation->setCurrent(1);
    emit m_pageNavigation->loadView("ACISettingsView.qml");
}

void ACIMainview::loadVideoView() {
    emit m_pageNavigation->loadView("ACISelectScreen.qml");
    emit m_pageNavigation->loadSteering("1");
}


//!
//! \brief ACIMainViewModel::ACIMainViewModel
//! \param parent
//!
ACIMainViewModel::ACIMainViewModel(QObject *parent) : QObject(parent) {
    m_mainMenu = new ACIListModel();
    m_mainMenu->addItem(Item("home", ""));
    m_mainMenu->addItem(Item("media", ""));
    m_mainMenu->addItem(Item("navi", ""));
    m_mainMenu->addItem(Item("sync", ""));
    m_mainMenu->addItem(Item("options", ""));
    connect(m_mainMenu, SIGNAL(itemClicked(Item)), this, SLOT(listModelClicked(Item)));
}

//!
//! \brief ACIMainViewModel::~ACIMainViewModel
//!
ACIMainViewModel::~ACIMainViewModel() {

}

//!
//! \brief ACIMainViewModel::listModelClicked
//! \param itemClicked
//!
void ACIMainViewModel::listModelClicked(Item itemClicked) {
    TRACE(QString("Name: %1, Descr: %2, Value: %3")
          .arg(itemClicked.name())
          .arg(itemClicked.descr())
          .arg(itemClicked.value()));

    QString name = itemClicked.name();

    if(name.compare("media")==0) {
        emit loadMedia();
    } else if(name.compare("options")==0) {
        emit loadSettings();
    }
}


