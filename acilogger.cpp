#include "acilogger.h"



ACILogger* ACILogger::m_Instance = 0;

ACILogger::~ACILogger(){
    m_LogFile->close();
}

/**
 * writes log into the file
 */
void ACILogger::log(QString log){

    if(m_bConsoleOut) TRACE_CONSOLE(log);

    //for 0 do nothing
    if(m_iLogLevel == 0 || !m_bLogOpened) return;

    *m_TextStream << log << "\n";
    m_TextStream->flush();
}

/**
 * do loggging setup
 */
void ACILogger::setupLog(){
    TRACE_CONSOLE("setupLog");
    m_iLogLevel = 0;
    m_bLogOpened = false;
    m_bConsoleOut = false;
    m_sLogFileName = ACIConfig::instance()->homePath()+".arcomivi/logs/acilog.txt";
    QDir logs(ACIConfig::instance()->homePath()+".arcomivi/logs");
    if(!logs.exists()){
#ifdef Q_OS_LINUX        
        system(QString("mkdir -p %1/.arcomivi/logs").arg(ACIConfig::instance()->homePath()).toLatin1());
#endif

#ifdef Q_OS_WIN
        logs.mkdir(ACIConfig::instance()->homePath()+".arcomivi/logs");
#endif
    }
    rotateLogFiles();
    m_LogFile = new QFile(m_sLogFileName);

    if(!m_LogFile->open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        qDebug() << m_sLogFileName << " could not be opened!";
        m_bLogOpened = false;
        return;
    }
    m_bLogOpened = true;


    TRACE_CONSOLE(m_LogFile->size());
    m_TextStream = new QTextStream(m_LogFile);
    *m_TextStream << "[" << QDate::currentDate().toString("yy-MM-dd") << QTime::currentTime().toString("hh:mm:ss") << __FUNCTION__ << __LINE__  << "]-" << "=====================\n";
    *m_TextStream << "[" << QDate::currentDate().toString("yy-MM-dd") << QTime::currentTime().toString("hh:mm:ss") << __FUNCTION__ << __LINE__  << "]-" << "===> Log Started <===\n";
    m_TextStream->flush();
}

/**
 * set log level
 * 0 - nothing
 * 1 - info
 * 2 - debug( all )
 */
void ACILogger::setLogLevel(int v){
    m_iLogLevel = v;
}

/**
 *
 */
void ACILogger::setConsoleOut(bool v){
    m_bConsoleOut = v;
}

/**
 * rotate log files
 */
void ACILogger::rotateLogFiles(){
    if(m_sLogFileName=="") return;
    TRACE_CONSOLE(QString("Log file %1 exists.").arg(m_sLogFileName));
    if(!QFile::exists(m_sLogFileName)) return;
    //acilog.txt0
    //acilog.txt1
    //acilog.txt2
    //acilog.txt3
    QString ftemp = "";
    QString ftempRotated = "";

    for(int i = 2; i>=0; i--){
        ftemp = "%1%2";
        ftempRotated = "%1%2";
        ftemp = ftemp.arg(m_sLogFileName).arg(i);
        ftempRotated = ftempRotated.arg(m_sLogFileName).arg(i+1);
        if(QFile::exists(ftemp)) {
            //remove acilog.txt3 / 2 / 1
            QFile::remove(ftempRotated);
            //rename acilog.txt2 -> acilog.txt3
            //1 -> 2 ; 0 -> 1;
            QFile::rename(ftemp, ftempRotated);
            TRACE_CONSOLE(QString("Rename %1 to %2.").arg(ftemp).arg(ftempRotated));
        }
    }
    ftempRotated = "%1%2";
    ftempRotated = ftempRotated.arg(m_sLogFileName).arg(0);
    QFile::rename(m_sLogFileName, ftempRotated);
    TRACE_CONSOLE(QString("Rename %1 to %2.").arg(m_sLogFileName).arg(ftempRotated));
}
