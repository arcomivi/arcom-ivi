#include <QApplication>
#include <QGuiApplication>
#include "globalincludes.h"
#include "aciconfig.h"
#include "acimainview.h"
#include "acimaincontroller.h"
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QNetworkProxy>


int main(int argc, char *argv[]){

    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName("194.145.60.1");
    proxy.setPort(9400);
    QNetworkProxy::setApplicationProxy(proxy);

//    qmlRegisterType<ACICamera>("ACIElements", 1, 0, "ACICamera");

    TRACE_CONSOLE("Start...");
    QApplication a(argc, argv);
    a.setApplicationName("ArComIVI");
    QCoreApplication::setOrganizationName("ArCom-IVI");
    QCoreApplication::setOrganizationDomain("arcom-ivi.de");
    QCoreApplication::setApplicationName("ArComIVI");

    ACIMainController *controller = new ACIMainController();

//    Test with 2 cameras:
//    QFoo *bar = new QFoo(0);
//    bar->setGeometry(10,10,640,480);
//    bar->show();

//    QFoo *boo = new QFoo(1);
//    boo->setGeometry(700,10,640,480);
//    boo->show();

//    return a.exec();

    //==> set-up home path in config
    ACIConfig::instance()->setHomePath(QDir::homePath());


    //==> setup directory: $USER/.arcomivi
    //check if .arcomivi exists and if not, create it
    QDir arcom(ACIConfig::instance()->homePath()+".arcomivi");
    if(!arcom.exists()){
        QDir home(QDir::homePath());
        if(!home.mkdir(".arcomivi")){
            exit(-1);
        }
    }

    //==> set up initial logging
    //set log level to log everything
    ACILogger::getInstance()->setupLog();
    ACILogger::getInstance()->setLogLevel(3);
    ACILogger::getInstance()->setConsoleOut(true);

    ACIConfig::instance()->initConfig();
    qmlRegisterSingletonType(ACIConfig::instance()->getQmlPrefix()+"ACIStyle.qml", "ACIStyle", 1, 0, "ACIStyle");



    TRACE_CONSOLE("Start...READY=1");

    controller->run();
    return a.exec();
}
