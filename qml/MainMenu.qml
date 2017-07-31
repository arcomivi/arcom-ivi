import QtQuick 2.5

Item {
    id: rootMainMenu

    // ==> properties
    property alias gridModel: mainMenuGrid.model    
    property int m_current: -1

    // ==> signals
    //...

    // ==> functions
    function handleLeave() {
        if(m_current===-1) { m_current = 0; }
        mainMenuRow.children[m_current].setButtonInactive();
    }

    function handleDirUp(){
    }

    function handleEnter() {
        if(m_current===-1) { m_current = 0; }
//        mainMenuRow.children[m_current].setButtonActive();
    }

    function handleRelease(){
        gridModel.listClicked(mainMenuGrid.currentIndex);
    }

    function handlePush(){
        switch(m_current){
        case 0:
            mainMenuRow.children[m_current].setButtonClicked();
            break;
        case 1:
            mainMenuRow.children[m_current].setButtonClicked();
            break;
        case 2:
            mainMenuRow.children[m_current].setButtonClicked();
            break;
        case 3:
            mainMenuRow.children[m_current].setButtonClicked();
            break;
        }
    }

    function handleRot(direction){
        if(m_current===-1) { m_current = 0; }

        if(direction===0){
            handleRotCw();
        } else {
            handleRotCcw();
        }
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
        delegate: ACIButton {
            height: parent.height
            width: mainMenuGrid.cellWidth
            pngname: name
            text: index!=0?"":"HOME"
            borderWidth.width: GridView.isCurrentItem ? 1:0;
            btnImg: g_cssprefix + "css/mainmenu/active/"+ pngname +"013.png"
            btnImgPressed: g_cssprefix + "css/mainmenu/inactive/"+pngname+".png"

            onClicked: {
                mainMenuGrid.currentIndex = index;
                gridModel.listClicked(mainMenuGrid.currentIndex);
            }
        }
    }
}
