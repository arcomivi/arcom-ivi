#include "acivideoview.h"

ACIVideoView::ACIVideoView(QQuickView *parent) :
    QQuickView(parent){

}

void ACIVideoView::setQmlFile(QString qml){
    this->setSource(QUrl(qml));

    connect((QObject*)this->rootObject(), SIGNAL(completedLoading()), this , SLOT(playVideo()));
}

void ACIVideoView::keyPressEvent(QKeyEvent *e){
    if(e->key() == Qt::Key_W){ //UP
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleDirUp", Qt::DirectConnection);
        return;
    }
    if(e->key() == Qt::Key_M){ //ROT1
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleRot", Qt::DirectConnection, Q_ARG(QVariant, 0));
        return;
    }
    if(e->key() == Qt::Key_N){ //ROT2
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleRot", Qt::DirectConnection, Q_ARG(QVariant, 1));
        return;
    }
    if(e->key() == Qt::Key_R){ //release PUSHB
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleRelease", Qt::DirectConnection);
        return;
    }

    if(e->key() == Qt::Key_Z){ //DOWN
        QMetaObject::invokeMethod((QObject*)this->rootObject(), "handleDirDown", Qt::DirectConnection);
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
