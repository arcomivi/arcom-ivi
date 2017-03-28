#ifndef ACILOGGER_H
#define ACILOGGER_H

#include <QObject>
#include "globalincludes.h"
#include "aciconfig.h"

#define TRACE(lg) ACILogger::getInstance()->log(QString("[%0 %1 %2 %3 %4]- %5").arg(QDate::currentDate().toString("yy-MM-dd")).arg(QTime::currentTime().toString("hh:mm:ss:z")).arg(__FILE__).arg(__FUNCTION__).arg(__LINE__).arg(lg));
#define TO_BE_IMPLEMENTED ACILogger::getInstance()->log(QString("to be implemented ..."));

class ACILogger : public QObject
{
    Q_OBJECT
public:
    static ACILogger* getInstance(){
        static QMutex mutex;
        if (!m_Instance)
        {
            mutex.lock();

            if (!m_Instance)
                m_Instance = new ACILogger(0);

            mutex.unlock();
        }

        return m_Instance;
    }
    ~ACILogger();

    void setupLog();
    void log(QString);
    void setLogLevel(int v);
    void setConsoleOut(bool v);

Q_SIGNALS:

public Q_SLOTS:

private:
    QString m_sLogFileName;
    QFile   *m_LogFile;
    QTextStream *m_TextStream;
    int m_iLogLevel;
    bool m_bLogOpened;
    bool m_bConsoleOut;


    void rotateLogFiles();

    static ACILogger* m_Instance;
    ACILogger(QObject *parent = 0) :
            QObject(parent) {

    }
};

#endif // ACILOGGER_H
