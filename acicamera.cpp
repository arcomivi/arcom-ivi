#include "acicamera.h"


ACICamera::ACICamera(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{

    webcam.open(0);
    if(!webcam.isOpened()){
        qDebug() << "camera is not opened!";
        return;
    } else {
        qDebug() << "camera is opened!";
    }

    timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(bar()));
//    timer->start(20);
}

void ACICamera::paint(QPainter *painter)
{
    Mat matOriginal;
    webcam.read(matOriginal);
    QImage img((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);
    img.mirrored(true, false);
    painter->drawImage(boundingRect().adjusted(1, 1, -1, -1), img);
}

void ACICamera::bar()
{
    update();
}
