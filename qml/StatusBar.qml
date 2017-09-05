import QtQuick 2.5
import QtQml 2.2

Item {
    id: root
    property var locale: Qt.locale();
    property string g_cssprefix

//    function btDiscovering(flag){
//        if(flag===1){
//            bt_timer.running = true;
//        } else {
//            bt_timer.running = false;
//        }
//    }

//    function setGeometry(vwidth, vheight){
//        root.width = vwidth-1;
//        root.height = vheight-1;
//    }

//    function setCssPrefix(prefix){
//        g_cssprefix = prefix;
//    }

//    function setNorthInactive(){
//        img_gps.source = g_cssprefix + "css/common/inactive/icon_status_north.png"
//    }

//    function setNorthActive(){
//        img_gps.source = g_cssprefix + "css/common/active/icon_status_north.png"
//    }

//    function setBluetoothInactive(){
//        bt_flipable_im_front.source = g_cssprefix + "css/common/inactive/icon_bluetooth.png";
//    }

//    function setBluetoothActive(){
//        bt_flipable_im_front.source = g_cssprefix + "css/common/active/icon_bluetooth.png";
//    }

//    function setCtrlInactive(){
//        img_ctrl.source = g_cssprefix + "css/common/inactive/icon_controller_inactive.png"
//    }

//    function setCtrlActive(){
//        img_ctrl.source = g_cssprefix + "css/common/active/icon_controller_active.png"
//    }

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
        Rectangle {
            id: mediaProgress; x: 0; y: 0; height: parent.height;
            width: 0; color: "#B5C808"; opacity: 0.8;
            radius: 2;
            Behavior on width { SmoothedAnimation { velocity: 1200 } }
        }
        Text {
            id: txt_song_time
            text: $media.songPosition
            color: "#D0D0D0"
            renderType: Text.NativeRendering
            font {
                family: "Helvetica";
                pixelSize: 15
                bold: false
            }
            anchors {
                left: parent.left; leftMargin: 5
//                verticalCenter: parent.verticalCenter
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
            font.pixelSize: 15
        }

//        Image {
//            id: img_ctrl
//            width: 16
//            height: 16
//            anchors.right: parent.right
//            anchors.rightMargin: 5
//            anchors.verticalCenter: parent.verticalCenter
//            source: g_cssprefix + "css/common/inactive/icon_controller_inactive.png"
//        }






//        Timer {
//            id: bt_timer
//            interval: 1500; running: false; repeat: true;
//            onTriggered: bt_flipable.flipped = !bt_flipable.flipped;
//        }

//        Flipable {
//            id: bt_flipable
//            anchors.right: img_ctrl.left
//            anchors.rightMargin: 5
//            width: 16
//            height: 16
//            anchors.verticalCenter: parent.verticalCenter
//            property bool flipped: false

//            front: Image {
//                id: bt_flipable_im_front
//                x: 0
//                y: 0
//                width: 16
//                height: 16
//                source: g_cssprefix + "css/common/inactive/icon_bluetooth.png"
//            }

//            back: Image {
//                id: bt_flipable_im_back
//                x: 0
//                y: 0
//                width: 16
//                height: 16
//                source: g_cssprefix + "css/common/inactive/icon_bluetooth.png"
//            }

//            transform: Rotation {
//                id: rotation
//                origin.x: bt_flipable.width/2
//                origin.y: bt_flipable.height/2
//                axis.x: 0; axis.y: 1; axis.z: 0     // set axis.y to 1 to rotate around y-axis
//                angle: 0    // the default angle
//            }

//            states: State {
//                name: "back"
//                PropertyChanges { target: rotation; angle: 180 }
//                when: bt_flipable.flipped
//            }

//            transitions: Transition {
//                NumberAnimation { target: rotation; property: "angle"; duration: 1000 }
//            }

//            MouseArea {
//                anchors.fill: parent
//                onClicked: {
//                }
//            }
//        }

//        Image {
//            id: img_gps
//            width: 16
//            height: 16
//            anchors.right: txt_time.left
//            anchors.rightMargin: 5
//            anchors.verticalCenter: bt_flipable.verticalCenter
//            source: g_cssprefix + "css/common/inactive/icon_status_north.png"
//        }

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
            font.pixelSize: 15
        }
    }
}
