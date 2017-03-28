#ifndef ACICONFIG_H
#define ACICONFIG_H

#include "globalincludes.h"
#include "acilogger.h"

#define ACI_VERSION "0.9.3-0.12"

#ifdef Q_OS_WIN
#define ACI_CONFIG_XML "C:/Qt/ws/Test/Test/aciconfig.xml"
#define ACI_QML "file:///C:/Qt/ws/Test/Test/qml/"
#define ACI_CSS "file:///C:/temp/arcom/"
#endif

#ifdef Q_OS_LINUX
#define ACI_CONFIG_XML "/usr/share/arcomivi/aciconfig.xml"
#define ACI_QML "file:///usr/share/arcomivi/qml/"
#define ACI_CSS "file:///usr/share/arcomivi/"
#endif

class ACIConfigSaxHandler : public QXmlDefaultHandler
{
//    Q_OBJECT
public:
    explicit ACIConfigSaxHandler(){ }

    bool startDocument();
    bool endDocument();

    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);
    QString errorString() const;
    QMap<QString, QString> getConfig() { return m_config; }

private:
    QString m_curentNode;
    QMap<QString, QString> m_config;
};

class ACIConfig : public QObject
{
    Q_OBJECT
public:
    static ACIConfig* instance(){
        static QMutex mutex;
        if (!m_Instance)
        {
            mutex.lock();

            if (!m_Instance)
                m_Instance = new ACIConfig(0);

            mutex.unlock();
        }

        return m_Instance;
    }


    ~ACIConfig();

    QString getCssFile();
    bool overrideCursor();
    bool getConsoleLog();
    int getLogLevel();
    int initConfig();
    void setHomePath(const QString &homePath) {
        m_sHomePath = homePath;
        m_sHomePath = (m_sHomePath == "/"?m_sHomePath:m_sHomePath+"/");
        TRACE_CONSOLE(m_sHomePath);
    }
    const QString &homePath() { return m_sHomePath; }
    QString getQmlPrefix() {         return ACI_QML;    }
    QString getCssPrefix() {         return ACI_CSS;    }
    QString version() { return ACI_VERSION; }
    int getx(){ return m_config.value("x").toInt(); }
    int gety(){ return m_config.value("y").toInt(); }
    int getw(){ return m_config.value("w").toInt(); }
    int geth(){ return m_config.value("h").toInt(); }
signals:

public slots:

private:
    static ACIConfig* m_Instance;
    ACIConfig(QObject *parent = 0) :
            QObject(parent) {
    }
    QString m_cssFile;
    ACIConfigSaxHandler *m_saxhandler;
    QMap<QString, QString> m_config;
    QString m_sHomePath;
};

#endif // ACICONFIG_H
