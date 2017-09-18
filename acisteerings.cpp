#include "acisteerings.h"

//!
//! \brief ACISteerings::ACISteerings
//! \param parent
//!
ACISteerings::ACISteerings(QObject *parent) : QObject(parent) {
    //Audio Steerings
    m_steerMusic = new ACIListModel();
    Item *item = new Item("volup", "", "", "", "", QUrl("file:///D:/temp/ws/arcomivi/arcomivi/css/media/active/10/volup.png"));
    connect(item, &Item::itemReleased, this, &ACISteerings::volup);
    m_steerMusic->addItem(item);

    item = new Item("voldown", "", "", "", "", QUrl("file:///D:/temp/ws/arcomivi/arcomivi/css/media/active/10/voldown.png"));
    connect(item, &Item::itemReleased, this, &ACISteerings::voldown);
    m_steerMusic->addItem(item);

    item = new Item("last", "", "", "", "", QUrl("file:///D:/temp/ws/arcomivi/arcomivi/css/media/active/10/last.png"));
    connect(item, &Item::itemReleased, this, &ACISteerings::previous);
    m_steerMusic->addItem(item);

    item = new Item("play", "", "", "", "", QUrl("file:///D:/temp/ws/arcomivi/arcomivi/css/media/active/play-small.png"));
    connect(item, &Item::itemReleased, this, &ACISteerings::play);
    m_steerMusic->addItem(item);

    item = new Item("next", "", "", "", "", QUrl("file:///D:/temp/ws/arcomivi/arcomivi/css/media/active/10/next.png"));
    connect(item, &Item::itemReleased, this, &ACISteerings::next);
    m_steerMusic->addItem(item);

    //Video Steerings
    m_steerVideo = new ACIListModel();
    m_steerVideo->addItem(new Item("volup", "", "", "", "", QUrl("file:///D:/temp/ws/arcomivi/arcomivi/css/media/active/10/volup.png")));
    m_steerVideo->addItem(new Item("voldown", "", "", "", "", QUrl("file:///D:/temp/ws/arcomivi/arcomivi/css/media/active/10/voldown.png")));
    m_steerVideo->addItem(new Item("play", "", "", "", "", QUrl("file:///D:/temp/ws/arcomivi/arcomivi/css/media/active/10/play.png")));
}
