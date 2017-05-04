#ifndef ACIPAGENAVIGATION_H
#define ACIPAGENAVIGATION_H

#include "acilogger.h"

#include <QObject>

class ACIPageNavigation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int current READ current WRITE setCurrent NOTIFY currentChanged)
public:
    ACIPageNavigation(QObject *parent=Q_NULLPTR);
    virtual ~ACIPageNavigation();
    int current() {return m_current;}


signals:
    void loadView(QString aView);
    void loadSteering(QString aSteering);
    void currentChanged();
    void handleRelease();
    void handleRot(int direction);
    void handleDirUp();
    void handleDirDown();

public slots:
    void init();
    void setCurrent(int value) { m_current = value; }
private:
    int m_current;
};

#endif // ACIPAGENAVIGATION_H
