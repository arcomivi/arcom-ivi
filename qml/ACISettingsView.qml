import QtQuick 2.5

Item {
    id: settingsView;

    signal goUp
    signal goDown
    signal update

    function handleDirUp(){
        console.log("settingsView.handleDirUp");
        goUp();
    }

    function handleRelease() {
        console.log("settingsView.handleRelease");
        settingsList.model.listClicked(settingsList.currentIndex);
    }

    function handleRot(direction){
        console.log("settingsView.handleRot: "+direction)
        if(direction === 0){
            rotateCW();
        } else {
            rotateCCW();
        }
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
        delegate:
            ACIStandardListItem {
            height: Math.floor(settingsList.height / 5);

            onItemClicked: {
                settingsList.currentIndex = index;
                $settings.listModel.listClicked(settingsList.currentIndex);
            }
        }

        Component.onCompleted: {
            model = $settings.listModel;
        }
    }
}
