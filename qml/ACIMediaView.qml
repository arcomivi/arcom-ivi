import QtQuick 2.5

Item {
    // ==> properties
    id: acimediaview;
    objectName: "ACIMediaView.qml"
    anchors.fill: parent;

    // ==> signals
    signal goUp
    signal goDown

    // ==> functions
    function handleDirUp(){
        goUp();
    }

    function handleRelease() {
        list.currentItem.buttonModel.released();
    }

    function handleRot(direction){
        list.currentItem.buttonModel.active=false;
        if(direction === 0){
            rotateCW();
        } else {
            rotateCCW();
        }
        list.currentItem.buttonModel.active=true;
        $media.listModel.currentIndex=list.currentIndex;
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

    // ==> QML elements
    Rectangle { anchors.fill: parent; color: "#636363" }

    ListView {
        id: list;
        width: Math.floor(parent.width);
        height: Math.floor(parent.height);
        focus: true
        anchors.fill: parent;
        clip: true;
        delegate: ACIStandardListItem {
            buttonModel: model.itemData
            height: Math.floor(list.height / 5);
        }

        Component.onCompleted: {
            model = $media.listModel;
        }
    }
}
