#include "aciconfig.h"


ACIConfig* ACIConfig::m_Instance = 0;

/**
 *  desctuctor
 */
ACIConfig::~ACIConfig(){
    delete m_saxhandler;
}

/**
 *   get CSS file
 */
QString ACIConfig::getCssFile(){
    TRACE("get CSS file");
    return m_config.value("cssfile");
}

/**
 *  get console log
 */
bool ACIConfig::getConsoleLog(){
    return m_config.value("logconsole")=="1"?true:false;
}

/**
 * get log level
 */
int ACIConfig::getLogLevel(){
    return m_config.value("loglevel").toInt();
}

/**
 *  initialize configuration
 */
int ACIConfig::initConfig(){

    TRACE(QString("Reading config: %1").arg(ACI_CONFIG_XML));
    QFile file(ACI_CONFIG_XML);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        TRACE(file.errorString());
        return -1;
    }

    QXmlSimpleReader reader;
    m_saxhandler = new ACIConfigSaxHandler;
    reader.setContentHandler(m_saxhandler);
    reader.setErrorHandler(m_saxhandler);
    QXmlInputSource xmlInputSource(&file);
    if (reader.parse(xmlInputSource)){

    }
    file.close();
    TRACE("exit");
    m_config = m_saxhandler->getConfig();
    return 0;
}


/**
 *
 */
bool ACIConfigSaxHandler::startDocument(){
    TRACE(QString("start: %1").arg(QTime::currentTime().toString()));
    m_curentNode = "";
    return true;
}

/**
 *
 */
bool ACIConfigSaxHandler::endDocument(){
    TRACE(QString("end: %1").arg(QTime::currentTime().toString()));
    return true;
}

/**
 *
 */
bool ACIConfigSaxHandler::startElement(const QString & /* namespaceURI */,
                              const QString & /* localName */,
                              const QString &qName,
                              const QXmlAttributes &attributes){
    TRACE(qName);    
    if(qName!=""){
        m_curentNode = qName;
    } else {
        m_curentNode = "";
    }
    return true;
}

/**
 *
 */
bool ACIConfigSaxHandler::endElement(const QString & /* namespaceURI */,
                            const QString & /* localName */,
                            const QString &qName) {
    TRACE(qName);
    m_curentNode = "";
    return true;
}

/**
 *
 */
bool ACIConfigSaxHandler::characters(const QString &str){
    TRACE(str);
    if(!m_curentNode.contains(QRegExp("cssfile|loglevel|logconsole|setOverrideCursor|x|y|w|h"))) return true;
    TRACE(QString("Inserting: %1 - %2 ").arg(m_curentNode).arg(str));
    m_config.insert(m_curentNode, str);
    return true;
}

/**
 *
 */
QString ACIConfigSaxHandler::errorString() const {
    return "";//errorStr;
}

/**
 *
 */
bool ACIConfigSaxHandler::fatalError(const QXmlParseException &exception){   
    TRACE(exception.message());
    return false;
}

/**
 *  TODO: <description>
 *  @arg1: - ...
 *  @arg1: - ...
 *  @ret: - ...
 */
bool ACIConfig::overrideCursor(){
    TRACE(QString("enter"));
    return m_config.value("setOverrideCursor")=="1"?true:false;
}
