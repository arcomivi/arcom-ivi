#ifndef QFOO_H
#define QFOO_H

#include <QObject>
#include <QTimer>
#include <QLabel>
#include <QDebug>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class QFoo : public QLabel
{
    Q_OBJECT
//    Q_PROPERTY(int device READ device WRITE setDevice)
public:
    QFoo(int device);
//    void setDevice(const int device){
//        m_device = device; }

    QTimer *timer;
    VideoCapture webcam;
public Q_SLOTS:
    void bar();
private:
//    int m_device;
};

#endif // QFOO_H
