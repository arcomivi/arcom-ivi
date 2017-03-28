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
    function handleRelease(){
        update();
    }

    Rectangle { color: "#636363"; anchors.fill: parent;
        Text {
            id: settingsViewText
            renderType: Text.NativeRendering
            text: qsTr("Settings")
            color: "white"
            font.pixelSize: 60;
            anchors.centerIn: parent;
        }
    }
}
