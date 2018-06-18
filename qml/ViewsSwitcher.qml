import QtQuick 2.5

Item {
    id: viewsSwitcherContainer

    property alias pages: viewsSwitcherItem.children
    property Item currentPage
    property alias itemObject: viewsSwitcherItem

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

//    function exists(qmlname){
//        for(var i = 0; i < pages.length; i++){
//            if(pages[i].objectName===qmlname){
//                return true;
//            }
//        }
//        return false;
//    }

    function jumpTo(qmlname){
        var visibleIndex = -1;
        var exists = false;
        console.log("jumpTo: "+qmlname);

        for(var i = 0; i < pages.length; i++) {
            pages[i].x = 1024;
            pages[i].visible = false;

            if(pages[i].objectName===qmlname){
                console.log("jumpTo-objectName: "+pages[i].objectName)
                visibleIndex = i;
                exists = true;
            }
        }
        //page not found
        if(!exists || visibleIndex === -1){
            console.log(qmlname + " not found. Creating...");
            var cmp = Qt.createComponent(qmlname);
            currentPage = cmp.createObject(itemObject, {});
        } else {
            currentPage = pages[visibleIndex];
        }
        currentPage.x=0;
        currentPage.visible=true;
    }

    Item {
        id: viewsSwitcherItem
        anchors.fill: parent
    }
}
