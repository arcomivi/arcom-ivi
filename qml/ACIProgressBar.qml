import QtQuick 2.5

Rectangle {
    id: progressBar
    signal seek(int percentage)
    color: "#636363"; //Qt.rgba(0.0, 0.0, 0.0, 0.0);
    property int borderWidth: 0

    function setMediaProgress(progr){
        mediaProgress.width = mediaProgressRow.width * ( progr / 100 );
    }


    Row {
        id: mediaProgressRow; width: parent.width; height: parent.height;
        Rectangle {
            id: mediaProgressBar;
            visible: true; width: parent.width; height: parent.height;
            color: Qt.rgba(0.0, 0.0, 0.0, 0.0); radius: 2;
            border.color: "orange"
            border.width: borderWidth
            Rectangle {
                id: mediaProgress; x: 0; y: 0; height: parent.height;
                width: 1; color: "#B5C808"; opacity: 0.8;
                radius: 2;
                Behavior on width { SmoothedAnimation { velocity: 1200 } }
            }
            Column {
                Text {
                    id: mediaProgressText;
                    renderType: Text.NativeRendering
                    text: ""
//                    anchors { centerIn: parent; }
                }
            }
            MouseArea {
                anchors.fill: parent;
                onClicked: {
                    seek(mouseX / mediaProgressRow.width * 100);
                    mediaProgress.width = mouseX;
                }
            }
        }
    }
}
