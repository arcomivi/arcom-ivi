import QtQuick 2.5

Rectangle {
    id: progressBar
    color: "#636363";
    property int borderWidth: 0

    signal seek(int percentage)


    function setMediaProgress(progr){
        mediaProgress.width = mediaProgressBar.width * ( progr / 100 );
    }


    Rectangle {
        id: mediaProgressBar;
        visible: true;
        width: parent.width;
        height: parent.height;
        color: Qt.rgba(0.0, 0.0, 0.0, 0.0);
        radius: 0;
        border.color: "orange"
        border.width: borderWidth
        Rectangle {
            id: mediaProgress;
            x: 0;
            y: 0;
            height: parent.height;
            width: 0;
            color: "#B5C808";
            Behavior on width { SmoothedAnimation { velocity: 800 } }
        }

        MouseArea {
            anchors.fill: parent;
            onClicked: {
                seek(mouseX / mediaProgressBar.width * 100);
                mediaProgress.width = mouseX;
            }
        }
    }
}
