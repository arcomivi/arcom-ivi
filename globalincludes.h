#ifndef GLOBALINCLUDES_H
#define GLOBALINCLUDES_H

#include <QtQuick/QQuickView>
#include <QWidget>
#include <QObject>
#include <QProcess>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QTimer>
#include <QDir>
#include <QGridLayout>
#include <QWindow>
#include <QtMultimedia/QMediaContent>
#include <QtXml/QXmlDefaultHandler>
#include <QStorageInfo>

#ifdef Q_OS_LINUX
#include <QtDBus/QDBusConnection>
#endif

#include <math.h>

#define TRACE_CONSOLE(lg) qDebug() << QString("[%0 %1 %2 %3 %4]- %5").arg(QDate::currentDate().toString("yy-MM-dd")).arg(QTime::currentTime().toString("hh:mm:ss:z")).arg(__FILE__).arg(__FUNCTION__).arg(__LINE__).arg(lg);

#define QT_USE_FAST_CONCATENATION

enum ACILatLonBookmark {
    NAVIT_BOOKMARK_OK = 1,
    NAVIT_LAT_SMALLER_MINUS_90 = 2,
    NAVIT_LAT_BIGGER_90 = 3,
    NAVIT_LON_SMALLER_MINUS_180 = 4,
    NAVIT_LON_BIGGER_180 = 5,
    NAVIT_BOOKMARK_ERR = 6

} ;

enum ACIWidgets {
    MainView = 0,
    MenuMedia, //1
    SteerMusicWidget,//2
    ViewNavigation,//3
    MenuNavigation,//4
    SteerNavigation,//5
    SteerNavigationSearch,//6
    NavigationOptions,//7
    MenuMain,//8
    ViewSettings,//9
    ViewSync,
    SteerSyncCustOrdersWidget,
    SteerSyncQuotationsWidget,
    Keyboard,
    VideoPlayer
};

enum ACISteer {
    SteerMusic = 0,
    //SteerVideo,
    //SteerPictures,
    //SteerRadio,
    SteerNavi,
    SteerNavi2,
    SteerNaviOptions,
//    SteerNaviBookmarks,
//    SteerLayouts,
//    SteerTracker,
//    SteerInternet,
    SteerNaviSearch,
    SteerSyncCustOrders,
    SteerSyncCustOrder,
    SteerSyncQuotations,
    SteerSyncQuotation,
    SteerLogoBack = 98,
    SteerEmpty //=>99
};


//=============================
//===search methods============
//=============================
////for a{sv}
//typedef QMap<QString, QVariant> Dict; //added manualy
//Q_DECLARE_METATYPE (Dict) //added manualy

//for a{sv}
typedef QMap<QString, QVariant> Dict;
Q_DECLARE_METATYPE (Dict) //added manually

//for a{sa{sv}}
typedef QMap<QString, Dict> SearchResDict;
Q_DECLARE_METATYPE (SearchResDict) //added manualy

struct Coords
{
    int coord1;
    int coord2;
    int coord3;
};
Q_DECLARE_METATYPE(Coords)
#endif // GLOBALINCLUDES_H
