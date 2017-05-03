#include "acisteerings.h"

//!
//! \brief ACISteerings::ACISteerings
//! \param parent
//!
ACISteerings::ACISteerings(QObject *parent) : QObject(parent){

    m_steerMusic = new ACIListModel();
    m_steerMusic->addItem(Item("volup", ""));
    m_steerMusic->addItem(Item("voldown", ""));
    m_steerMusic->addItem(Item("last", ""));
    m_steerMusic->addItem(Item("play", ""));
    m_steerMusic->addItem(Item("next", ""));
    connect(m_steerMusic, SIGNAL(itemClicked(Item)), this, SLOT(steerMusicModelClicked(Item)));

    m_steerVideo = new ACIListModel();
    m_steerVideo->addItem(Item("volup", ""));
    m_steerVideo->addItem(Item("voldown", ""));
    m_steerVideo->addItem(Item("play", ""));
}

//!
//! \brief ACISteerings::steerMusicModelClicked
//! \param itemClicked
//!
void ACISteerings::steerMusicModelClicked(Item itemClicked){
    TRACE(QString("Name: %1, Descr: %2, Value: %3")
          .arg(itemClicked.name())
          .arg(itemClicked.descr())
          .arg(itemClicked.value()));
    QString name = itemClicked.name();
    if(name.compare("play")==0){
        emit play();
    } else if(name.compare("volup")==0){
        emit volup();
    } else if(name.compare("voldown")==0){
        emit voldown();
    } else if(name.compare("last")==0){

    } else if(name.compare("next")==0){

    }
}
