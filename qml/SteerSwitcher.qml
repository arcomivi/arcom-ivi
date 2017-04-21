import QtQuick 2.5

Item {
    id: steeringsSwitcherContainer

    property alias pages: steeringsSwitcherItem.children
    property Item currentPage

    function jumpTo(idx){
        console.log("[steeringsSwitcherContainer] jumpTo: "+idx);
        for(var i = 0; i < pages.length; i++)
        {
            pages[i].x = 1024;
//            console.log("jumpTo-move X to: "+pages[i].x);
        }
        pages[idx].x = 0;
        currentPage = pages[idx];
//        console.log("jumpTo:"+idx +", "+pages[idx].x);
    }

    Item {
        id: steeringsSwitcherItem
        anchors.fill: parent
    }
}

