#include "qfoo.h"
#include <QTransform>



QFoo::QFoo(int device) {

    webcam.open(device);
    if(!webcam.isOpened()){
        qDebug() << "camera is not opened!";
    } else {
        qDebug() << "camera is opened!";
    }

    timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(bar()));
    timer->start(20);
}

void QFoo::bar() {
    Mat matOriginal;
    webcam.read(matOriginal);
    QImage img((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);
    img.mirrored(true, false);
    this->setPixmap(QPixmap::fromImage(img));
}
