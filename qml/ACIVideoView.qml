import QtQuick 2.5
import QtMultimedia 5.5

Item {
    id: videoView;
    signal exitVideo
    signal completedLoading

    function playVideo(){
        video.play();
    }

    function setVideoSource(name){
        video.source = name;        
    }

    function handleDirUp(){
        video.stop();
        exitVideo();
    }

    function handleDirDown(){
        videoSteering.visible = true;
        videoSteeringTimer.restart();
    }

    function handleRelease(){
        completedLoading();
    }

    function handleRot(direction){
        console.log("videoView.handleRot"+direction)
        if(direction===0){
            video.seek(video.position + 2000)
        } else {
            video.seek(video.position - 2000)
        }
    }
    Timer { id: videoSteeringTimer; repeat: false; interval: 3000;
        onTriggered: videoSteering.visible = false;
    }
    Rectangle { anchors.fill: parent; color: "black" }

    Video {
        id: video
        anchors.fill: parent;
    }

    Rectangle {
        id: videoSteering;
        color: Qt.rgba(0.0,0.0,0.0,0.8);

        Text {
            id: foo
            renderType: Text.NativeRendering
            text: qsTr("Steering Video");
            color: "white"
            anchors.centerIn: parent;
        }
        anchors.bottom: parent.bottom;
        width: parent.width;
        height: 50;
        visible: false;

    }
    MouseArea {
        anchors.fill: parent;
        onClicked: {
            console.log("parent video clicked")
            videoSteering.visible = true;
            videoSteeringTimer.restart();
            video.playbackState == MediaPlayer.PlayingState ? video.pause() : video.play();
        }
    }
}
