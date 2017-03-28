#ifndef ACICAMERA_H
#define ACICAMERA_H

#include <QObject>
#include <QtQuick/QQuickPaintedItem>
#include <QQuickItem>
#include <QPainter>
#include <QTimer>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class ACICamera : public QQuickPaintedItem
{
    Q_OBJECT

public:
    ACICamera(QQuickItem *parent = 0);

//    QString name() const;
//    void setName(const QString &name);

//    QColor color() const;
//    void setColor(const QColor &color);

    void paint(QPainter *painter);
public Q_SLOTS:
    void bar();
private:
    QTimer *timer;
    VideoCapture webcam;
};

#endif // ACICAMERA_H
