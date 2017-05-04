import QtQuick 2.5

Item {
    id: screenSelector
    objectName: "ACISelectScreen.qml"
    anchors.fill: parent;

    // ==> functions
    function handleDirUp(){
        goUp();
    }

    function handleRelease() {
        list.model.listClicked(list.currentIndex);
    }

    function handleRot(direction){
        if(direction === 0){
            rotateCW();
        } else {
            rotateCCW();
        }
    }

    function rotateCW(){
        if((list.currentIndex + 1) < list.count){
            list.currentIndex = list.currentIndex + 1;
        }
        list.positionViewAtIndex(list.currentIndex, ListView.Center);
    }

    function rotateCCW(){
        if((list.currentIndex-1)  >=0){
            list.currentIndex = list.currentIndex-1;
        }
        list.positionViewAtIndex(list.currentIndex, ListView.Center);
    }

    //==> QML Elements
    Rectangle {
        color: "#636363";
        anchors.fill: parent;
    }

    ListView {
        id: list;
        width: Math.floor(parent.width);
        height: Math.floor(parent.height);
        focus: true
        anchors.fill: parent;
        clip: true;
        delegate:
            ACIStandardListItem {
            height: Math.floor(list.height / 5);

            onItemClicked: {
                list.currentIndex = index;
                $settings.listModel.listClicked(list.currentIndex);
            }
        }

        Component.onCompleted: {
            model = $settings.screens;
        }
    }
}
