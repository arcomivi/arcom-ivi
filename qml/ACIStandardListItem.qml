import QtQuick 2.5

Item {
    id: standardListItem;
    width: Math.floor(parent.width);
    signal itemClicked(int index);


    //===============
    //row: picture; column: text1, text2; text
    //===============
    //smaller text
    Rectangle {
        height:parent.height; width: parent.width;
        color: "#636363"
        Text {
            id: standardListItemText
            renderType: Text.NativeRendering
            text: descr
            font {
                family: "Helvetica";
                pixelSize: 13;
                bold: true
            }
            color: standardListItem.ListView.isCurrentItem ? "#B5C808" : (value2===""?"white": value2 )
            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            standardListItem.itemClicked(index);
        }
    }
}
