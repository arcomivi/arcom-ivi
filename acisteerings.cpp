#include "acisteerings.h"

//!
//! \brief ACISteerings::ACISteerings
//! \param parent
//!
ACISteerings::ACISteerings(QObject *parent) : QObject(parent) {
    //Audio Steerings
    m_steerMusic = new ACIListModel();
    Item *item = new Item("volup", "", "", "", "", QUrl("file:///D:/temp/ws/arcomivi/arcomivi/css/media/active/10/volup.png"));
    m_steerMusic->addItem(item);

    m_steerMusic->addItem(new Item("voldown", "", "", "", "", QUrl("file:///D:/temp/ws/arcomivi/arcomivi/css/media/active/10/voldown.png")));
    m_steerMusic->addItem(new Item("last", "", "", "", "", QUrl("file:///D:/temp/ws/arcomivi/arcomivi/css/media/active/10/last.png")));

    item = new Item("play", "", "", "", "", QUrl("file:///D:/temp/ws/arcomivi/arcomivi/css/media/active/10/play.png"));
    connect(item, &Item::itemReleased, this, &ACISteerings::play);
    m_steerMusic->addItem(item);

    m_steerMusic->addItem(new Item("next", "", "", "", "", QUrl("file:///D:/temp/ws/arcomivi/arcomivi/css/media/active/10/next.png")));

    //Video Steerings
    m_steerVideo = new ACIListModel();
    m_steerVideo->addItem(new Item("volup", "", "", "", "", QUrl("file:///D:/temp/ws/arcomivi/arcomivi/css/media/active/10/volup.png")));
    m_steerVideo->addItem(new Item("voldown", "", "", "", "", QUrl("file:///D:/temp/ws/arcomivi/arcomivi/css/media/active/10/voldown.png")));
    m_steerVideo->addItem(new Item("play", "", "", "", "", QUrl("file:///D:/temp/ws/arcomivi/arcomivi/css/media/active/10/play.png")));
}
