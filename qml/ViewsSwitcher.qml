import QtQuick 2.5

Item {
    id: viewsSwitcherContainer

    property alias pages: viewsSwitcherItem.children
    property Item currentPage
    property alias itemObject: viewsSwitcherItem
    property int visibleIndex: -1;

    function handleEnter(){
    }

    function handleLeave(){
    }

    function handleRot(direction){
        currentPage.handleRot(direction);
    }

    function handleRelease() {
        currentPage.handleRelease();
    }

    function exists(qmlname){
        for(var i = 0; i < pages.length; i++){
            if(pages[i].objectName===qmlname){
                return true;
            }
        }
        return false;
    }

    function jumpTo(idx, qmlname){
        console.log("jumpTo: "+idx);
        for(var i = 0; i < pages.length; i++)
        {
            pages[i].x = 1024;
            pages[i].visible = false;

            if(pages[i].objectName===qmlname){
                console.log("jumpTo-objectName: "+pages[i].objectName)
                visibleIndex = i;
            }

            //!!!do not initialize source!!!
            //once loaded, keep in memory
//            pages[i].source = "";
//            console.log("jumpTo-move X to: "+pages[i].x);
        }
        pages[visibleIndex].x=0;
        pages[visibleIndex].visible=true;
//        pages[idx].x = 0;
//        pages[idx].visible = true;
//        if(pages[idx].status === Loader.Null){
//            console.log("Loader source empty(status Null) -> set: " + qmlname)
//            pages[idx].source = qmlname;
//        }
        currentPage = pages[visibleIndex];
//        console.log("jumpTo: "+idx +", x: "+pages[idx].x +", visible:"+pages[idx].visible +", source: "+pages[idx].source);

    }

    Item {
        id: viewsSwitcherItem
        anchors.fill: parent
    }
}
