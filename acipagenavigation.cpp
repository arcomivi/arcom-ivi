#include "acipagenavigation.h"

//!
//! \brief ACIPageNavigation::ACIPageNavigation
//! \param parent
//!
ACIPageNavigation::ACIPageNavigation(QObject *parent) : QObject(parent) {

}

//!
//! \brief ACIPageNavigation::~ACIPageNavigation
//!
ACIPageNavigation::~ACIPageNavigation() {

}



//!
//! \brief ACIPageNavigation::init
//!
void ACIPageNavigation::init()
{
    TRACE("init page navigation...");
    m_current=0;
    emit loadView("ACIHomeView.qml");
}
