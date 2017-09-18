import QtQuick 2.6
import QtQml 2.2

Item {
    id: root
    property var locale: Qt.locale();
    property string g_cssprefix

    Timer {
        running: true
        repeat: true
        onTriggered: {
            txt_time.text = (new Date()).toLocaleTimeString(locale, "hh:mm:ss");
        }
    }

    Rectangle {
        id: status_bar_rect
        width: parent.width; height: parent.height;
        color: "#636363"

        Text {
            id: txt_song_time
            text: $media.songPosition
            color: "#D0D0D0"
            renderType: Text.NativeRendering
            font {
                family: "Helvetica";
                pixelSize: 14
                bold: false
            }
            anchors.verticalCenter: parent.verticalCenter
            anchors {
                left: parent.left; leftMargin: 5
            }
        }

        Text {
            id: txt_album_artist
            renderType: Text.NativeRendering
            text: $media.songTitle
            color: "#D0D0D0"
            font.bold: false
            font.family: "Helvetica"
            anchors.left: txt_song_time.right
            anchors.leftMargin: 6
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 14
        }


        Text {
            id: txt_time
            renderType: Text.NativeRendering
            text: ""
            color: "#D0D0D0"
            anchors.right: parent.right
            anchors.rightMargin: 5
            font.bold: false
            font.family: "Helvetica"

            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 14
        }
    }
}
