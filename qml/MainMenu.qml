import QtQuick 2.5

Item {
    id: rootMainMenu

    // ==> properties
    property var gridModel;
    property int m_current: -1

    // ==> signals
    //...

    // ==> functions
    function handleLeave() {
        mainMenuGrid.currentItem.buttonModel.active=false;
        if(m_current===-1) { m_current = 0; }
    }

    function handleDirUp(){
    }

    function handleEnter() {
        console.log("mainMenuGrid.currentIndex:"+mainMenuGrid.currentIndex);
        console.log("mainMenuGrid.currentItem:"+mainMenuGrid.currentItem);
        mainMenuGrid.currentItem.buttonModel.active=true;
        if(m_current===-1) { m_current = 0; }

    }

    function handleRelease(){
        mainMenuGrid.currentItem.buttonModel.released();
        mainMenuGrid.currentItem.buttonModel.active=false;
    }

    function handlePush(){
        switch(m_current){
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        }
    }

    function handleRot(direction){
        if(m_current===-1) { m_current = 0; }
        mainMenuGrid.currentItem.buttonModel.active=false;
        if(direction===0){
            handleRotCw();
        } else {
            handleRotCcw();
        }
        mainMenuGrid.currentItem.buttonModel.active=true;
    }

    function handleRotCw(){
        if((mainMenuGrid.currentIndex + 1) < mainMenuGrid.count){
            mainMenuGrid.currentIndex = mainMenuGrid.currentIndex + 1;
        }
        mainMenuGrid.positionViewAtIndex(mainMenuGrid.currentIndex, GridView.Visible);
    }

    function handleRotCcw(){
        if((mainMenuGrid.currentIndex-1)  >=0){
            mainMenuGrid.currentIndex = mainMenuGrid.currentIndex-1;
        }
        mainMenuGrid.positionViewAtIndex(mainMenuGrid.currentIndex, GridView.Visible);
    }


    GridView {
        id: mainMenuGrid;
        width: parent.width; height: parent.height;
        cellHeight: parent.height;
        cellWidth: parent.width / mainMenuGrid.count
        model: gridModel
        delegate: ACIButton {
            height: parent.height
            width: mainMenuGrid.cellWidth
            buttonModel: model.itemData
        }
    }
}
