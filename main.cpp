#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QDesktopWidget>
#include "globalincludes.h"
#include "aciconfig.h"
#include "acimainview.h"
#include <QLabel>
#include <QImage>
#include <QPixmap>


int main(int argc, char *argv[]){


//    qmlRegisterType<ACICamera>("ACIElements", 1, 0, "ACICamera");

    TRACE_CONSOLE("Start...");
    QApplication a(argc, argv);
    a.setApplicationName("ArComIVI");
    QCoreApplication::setOrganizationName("ArCom-IVI");
    QCoreApplication::setOrganizationDomain("arcom-ivi.de");
    QCoreApplication::setApplicationName("ArComIVI");

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

    //w.setAttribute(Qt::WA_TranslucentBackground);
    //w.setAutoFillBackground(false);
    //w.setAttribute(Qt::WA_OpaquePaintEvent, true);
    //w.setAttribute(Qt::WA_NoSystemBackground);


    TRACE_CONSOLE("Start...READY=1");

//    gets information of the available desktops screens
    QDesktopWidget *desktopWidget = QApplication::desktop();

    ACIMainview *oMainview = new ACIMainview();
    oMainview->setQmlFile(ACIConfig::instance()->getQmlPrefix()+"MainView.qml");
    oMainview->setFlags(Qt::FramelessWindowHint);
    oMainview->setResizeMode(QQuickView::SizeRootObjectToView);

    oMainview->setGeometry(ACIConfig::instance()->getx(),
                           ACIConfig::instance()->gety(),
                           ACIConfig::instance()->getw()==0?desktopWidget->screenGeometry(0).width():ACIConfig::instance()->getw(),
                           ACIConfig::instance()->geth()==0?desktopWidget->screenGeometry(0).height():ACIConfig::instance()->geth());
    oMainview->show();
//    qDebug() << "S C R E E N S: " << a.screens().count();
    return a.exec();
}
