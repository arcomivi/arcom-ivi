import QtQuick 2.5

Item {
    id: root;
    objectName: "ACISettingsView.qml"
    anchors.fill: parent
    signal goUp
    signal goDown
    signal update

    function handleDirUp(){
        goUp();
    }

    function handleRelease() {
        settingsList.currentItem.buttonModel.released();
    }

    function handleRot(direction){
        settingsList.currentItem.buttonModel.active=false;
        if(direction === 0){
            rotateCW();
        } else {
            rotateCCW();
        }
        settingsList.currentItem.buttonModel.active=true;
    }

    function rotateCW(){
        if((settingsList.currentIndex + 1) < settingsList.count){
            settingsList.currentIndex = settingsList.currentIndex + 1;
        }
        settingsList.positionViewAtIndex(settingsList.currentIndex, ListView.Center);
    }

    function rotateCCW(){
        if((settingsList.currentIndex-1)  >=0){
            settingsList.currentIndex = settingsList.currentIndex-1;
        }
        settingsList.positionViewAtIndex(settingsList.currentIndex, ListView.Center);
    }

    Rectangle { color: "#636363"; anchors.fill: parent; }

    ListView {
        id: settingsList;
        width: Math.floor(parent.width);
        height: Math.floor(parent.height);
        focus: true
        anchors.fill: parent;
        clip: true;
        delegate: ACIStandardListItem {
            height: Math.floor(settingsList.height / 5);
            buttonModel: model.itemData
        }

        Component.onCompleted: {
            model = $settings.listModel;
        }
    }
}
