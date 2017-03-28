import QtQuick 2.0

Item {
    id: screenSelector
    signal screenSelected(int screen)

    function handleRelease(){
        screenSelected(0);
    }

    Rectangle { color: "#636363"; anchors.fill: parent; }
    Text {
        id: screenSelectorText
        renderType: Text.NativeRendering
        text: qsTr("Select Screen");
        color: "white";
        width: parent.width;
        height: 100;
        anchors.centerIn: parent;
        anchors.top: parent.top;
    }
    MouseArea{
        anchors.fill: parent;
        onClicked: screenSelected(0);
    }
}
