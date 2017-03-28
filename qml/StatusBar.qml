import QtQuick 2.5

Item {
    id: rootStatusBar

    property string g_cssprefix

    function btDiscovering(flag){
        if(flag===1){
            bt_timer.running = true;
        } else {
            bt_timer.running = false;
        }
    }

    function setGeometry(vwidth, vheight){
        rootStatusBar.width = vwidth-1;
        rootStatusBar.height = vheight-1;
    }

    function setCssPrefix(prefix){
        g_cssprefix = prefix;
    }

    function setNorthInactive(){
        img_gps.source = g_cssprefix + "css/common/inactive/icon_status_north.png"
    }

    function setNorthActive(){
        img_gps.source = g_cssprefix + "css/common/active/icon_status_north.png"
    }

    function setBluetoothInactive(){
        bt_flipable_im_front.source = g_cssprefix + "css/common/inactive/icon_bluetooth.png";
    }

    function setBluetoothActive(){
        bt_flipable_im_front.source = g_cssprefix + "css/common/active/icon_bluetooth.png";
    }

    function setCtrlInactive(){
        img_ctrl.source = g_cssprefix + "css/common/inactive/icon_controller_inactive.png"
    }

    function setCtrlActive(){
        img_ctrl.source = g_cssprefix + "css/common/active/icon_controller_active.png"
    }

    function setTime(time){
        txt_song_time.text = time;
    }

    function setProgress(progress){
        mediaProgress.width = rootStatusBar_rect.width * ( progress / 100 );
    }

    function setAlbumArtist(albumArtist){
        txt_album_artist.text = albumArtist;
    }

    function setClock(clock){
        txt_time.text = clock;
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
            text: "00:00"
            color: "black"
            renderType: Text.NativeRendering
            font {
                family: "Helvetica";
                pixelSize: 16
                bold: true
            }
            anchors {
                left: parent.left; leftMargin: 5
                verticalCenter: parent.verticalCenter
            }
        }

        Text {
            id: txt_album_artist
            renderType: Text.NativeRendering
            text: "<artist> - <album/song>"
            font.bold: true
            font.family: "Helvetica"
            anchors.left: txt_song_time.right
            anchors.leftMargin: 6
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 16
        }

        Image {
            id: img_ctrl
            width: 16
            height: 16
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            source: g_cssprefix + "css/common/inactive/icon_controller_inactive.png"
        }


        Text {
            id: txt_time
            renderType: Text.NativeRendering
            text: "10:43"
            anchors.right: img_gps.left
            anchors.rightMargin: 5
            font.bold: true
            font.family: "Helvetica"

            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 16
        }



        Timer {
            id: bt_timer
            interval: 1500; running: false; repeat: true;
            onTriggered: bt_flipable.flipped = !bt_flipable.flipped;
        }

        Flipable {
            id: bt_flipable
            anchors.right: img_ctrl.left
            anchors.rightMargin: 5
            width: 16
            height: 16
            anchors.verticalCenter: parent.verticalCenter
            property bool flipped: false

            front: Image {
                id: bt_flipable_im_front
                x: 0
                y: 0
                width: 16
                height: 16
                source: g_cssprefix + "css/common/inactive/icon_bluetooth.png"
            }

            back: Image {
                id: bt_flipable_im_back
                x: 0
                y: 0
                width: 16
                height: 16
                source: g_cssprefix + "css/common/inactive/icon_bluetooth.png"
            }

            transform: Rotation {
                id: rotation
                origin.x: bt_flipable.width/2
                origin.y: bt_flipable.height/2
                axis.x: 0; axis.y: 1; axis.z: 0     // set axis.y to 1 to rotate around y-axis
                angle: 0    // the default angle
            }

            states: State {
                name: "back"
                PropertyChanges { target: rotation; angle: 180 }
                when: bt_flipable.flipped
            }

            transitions: Transition {
                NumberAnimation { target: rotation; property: "angle"; duration: 1000 }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    //bt_flipable.flipped = !bt_flipable.flipped
                    //                    bt_timer.running = !bt_timer.running;
                }
            }
        }

        Image {
            id: img_gps
            width: 16
            height: 16
            anchors.right: bt_flipable.left
            anchors.rightMargin: 5
            anchors.verticalCenter: bt_flipable.verticalCenter
            source: g_cssprefix + "css/common/inactive/icon_status_north.png"
        }
    }
}
