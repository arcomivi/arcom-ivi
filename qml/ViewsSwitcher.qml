import QtQuick 2.5

Item {
    id: viewsSwitcherContainer

    property alias pages: viewsSwitcherItem.children
    property Item currentPage

    function jumpTo(idx, qmlname){
        console.log("jumpTo: "+idx);
        for(var i = 0; i < pages.length; i++)
        {
            pages[i].x = 1024;
            pages[i].visible = false;
            //!!!do not initialize source!!!
            //once loaded, keep in memory
//            pages[i].source = "";
//            console.log("jumpTo-move X to: "+pages[i].x);
        }
        pages[idx].x = 0;
        pages[idx].visible = true;
        if(pages[idx].status === Loader.Null){
            console.log("Loader source empty(status Null) -> set: " + qmlname)
            pages[idx].source = qmlname;
        }
        currentPage = pages[idx];
        console.log("jumpTo: "+idx +", x: "+pages[idx].x +", visible:"+pages[idx].visible +", source: "+pages[idx].source);
    }

    Item {
        id: viewsSwitcherItem
        anchors.fill: parent
    }
}
