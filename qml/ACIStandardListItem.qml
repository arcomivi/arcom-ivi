import QtQuick 2.5
import ACIStyle 1.0
Item {
    id: root;
    width: Math.floor(parent.width);
    property var buttonModel;

    Rectangle {
        height:parent.height;
        width: parent.width;
        //color: "#636363"
        color: "transparent"
        Image {
            anchors.verticalCenter: parent.verticalCenter
            source: buttonModel.imageSource
        }

        Text {
            id: text
            renderType: Text.NativeRendering
            text: buttonModel.descr
            font {
                family: "Helvetica";
                pixelSize: ACIStyle.listFontpixelSize
                bold: true
            }
            color: (buttonModel.active===true)?"#B5C808":(buttonModel.selected===false?"white": "black")
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    MouseArea {
        anchors.fill: parent
        onReleased: { list.currentIndex=index; list.model.currentIndex=index; buttonModel.released(); }
    }
}

