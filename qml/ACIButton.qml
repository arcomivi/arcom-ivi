import QtQuick 2.5

Item {
    id: root
    width: parent.width
    height: parent.height
    //signals

    //properties
    property var buttonModel
    property alias btnOpacity: buttonBorder.opacity
    property bool keyUsing: false
    property color textColor: "#D0D0D0"

    //functions
    function setButtonReleased(){
    }

    function setButtonClicked() {
    }

    function setButtonInactive() {
    }

    function setButtonActive(){
    }

    //border with rounded corners
    Rectangle {
        id: buttonBorder
        color: "transparent"//"#636363";
        anchors.fill: parent
        border.color: "orange"
        border.width: buttonModel.active===true?1:0;
        radius: 0
    }

    Image {
        id: buttonImage
        source: buttonModel.imageSource
        anchors.centerIn: parent
        fillMode: Image.Pad;
        smooth: false;
        asynchronous: true;
    }

    Text {
        id: buttonText
        anchors.centerIn: parent
        text: buttonModel.text
        renderType: Text.NativeRendering
        color: textColor
        style: Text.Normal
        font.pixelSize: 15;
        font.bold: false
    }

    MouseArea {
        hoverEnabled: true
        anchors.fill: parent
        onReleased: buttonModel.released();
        onPressed: { }
    }

}
