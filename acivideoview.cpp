#include "acivideoview.h"

ACIVideoView::ACIVideoView(QQuickView *parent) :
    QQuickView(parent){

    m_oPageNavigation = new ACIPageNavigation();
    this->rootContext()->setContextProperty("$pageNavigation", m_oPageNavigation);
}

void ACIVideoView::setQmlFile(QString qml){
    this->setSource(QUrl(qml));

    connect((QObject*)this->rootObject(), SIGNAL(completedLoading()), this , SLOT(playVideo()));
}

void ACIVideoView::keyPressEvent(QKeyEvent *e){
    if(e->key() == Qt::Key_W){ //UP
        //QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleDirUp", Qt::DirectConnection);
        emit m_oPageNavigation->handleDirUp();
        return;
    }
    if(e->key() == Qt::Key_M){ //ROT1
        //QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleRot", Qt::DirectConnection, Q_ARG(QVariant, 0));
        emit m_oPageNavigation->handleRot(0);
        return;
    }
    if(e->key() == Qt::Key_N){ //ROT2
        //QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleRot", Qt::DirectConnection, Q_ARG(QVariant, 1));
        emit m_oPageNavigation->handleRot(1);
        return;
    }
    if(e->key() == Qt::Key_R){ //release PUSHB
        //QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleRelease", Qt::DirectConnection);
        emit m_oPageNavigation->handleRelease();
        return;
    }

    if(e->key() == Qt::Key_Z){ //DOWN
        //QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleDirDown", Qt::DirectConnection);
        emit m_oPageNavigation->handleDirDown();
        return;
    }
    QQuickView::keyPressEvent(e);
}

void ACIVideoView::playVideo(){
    TRACE("playVideo:");
    TRACE(m_sCurrentVideo);
    QMetaObject::invokeMethod((QObject*)this->rootObject(), "setVideoSource", Q_ARG(QVariant, m_sCurrentVideo));
    QMetaObject::invokeMethod((QObject*)this->rootObject(), "playVideo", Qt::DirectConnection);
}
