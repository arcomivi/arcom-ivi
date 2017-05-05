import QtQuick 2.5
import QtMultimedia 5.5

Item {
    id: videoView;
    property string g_cssprefix: Qt.platform.os==="windows"?"file:///D:/temp/ws/arcomivi/arcomivi/":"/usr/share/arcomivi/";
    property string m_sPrefix: "css/media/active/10/"

    signal exitVideo
    signal completedLoading

    Connections {
        target: $pageNavigation

        onHandleRelease: {
            videoView.handleRelease();
        }

        onHandleRot: {
            videoView.handleRot(direction);
        }

        onHandleDirUp: {
            videoView.handleDirUp();
        }

        onHandleDirDown: {
            videoView.handleDirDown();
        }
    }

    function playVideo(){
        video.play();
    }



    function setVideoSource(name){
        console.log("Video: "+name);
        video.source = name;        
    }

    function handleDirUp(){
        video.stop();
        exitVideo();
    }

    function handleDirDown(){
        steerVideoGrid.visible = true;
        videoSteeringTimer.restart();
    }

    function handleRelease(){
        completedLoading();
    }

    function handleRot(direction){
        steerVideoGrid.visible = true;
        videoSteeringTimer.restart();

        if(direction===0){
//            video.seek(video.position + 2000)
            if((steerVideoGrid.currentIndex + 1) < steerVideoGrid.count){
                steerVideoGrid.currentIndex = steerVideoGrid.currentIndex + 1;
            }
            steerVideoGrid.positionViewAtIndex(steerVideoGrid.currentIndex, GridView.Visible);
        } else {
            if((steerVideoGrid.currentIndex-1)  >=0){
                steerVideoGrid.currentIndex = steerVideoGrid.currentIndex-1;
            }
            steerVideoGrid.positionViewAtIndex(steerVideoGrid.currentIndex, GridView.Visible);
//            video.seek(video.position - 2000)
        }
    }
    Timer { id: videoSteeringTimer; repeat: false; interval: 4000;
        onTriggered: { steerVideoGrid.visible = false;
        console.log("video timer triggered.....")}
    }

    Rectangle { anchors.fill: parent; color: "black" }

    Video {
        id: video
        anchors.fill: parent; anchors.centerIn: parent;
    }

    property alias aGridModel: steerVideoGrid.model

    GridView {
        id: steerVideoGrid;
        width: parent.width; height: 50;
        cellHeight: 50;
        cellWidth: parent.width / steerVideoGrid.count
        anchors.bottom: parent.bottom;
        model: $steerings.steerVideoModel
        delegate: ACIButton {
            height: parent.height
            width: steerVideoGrid.cellWidth
            btnOpacity: 0.5
            borderWidth.width: GridView.isCurrentItem ? 1:0;
            btnImg: g_cssprefix + m_sPrefix + name +".png"
            btnImgPressed: g_cssprefix + "css/media/inactive/"+name+".png"

            onClicked: {
                steerVideoGrid.currentIndex = index;
                aGridModel.listClicked(steerVideoGrid.currentIndex);
            }
        }
    }
    MouseArea {
        anchors.fill: parent;
        onClicked: {
            console.log("parent video clicked")
            steerVideoGrid.visible = true;
            videoSteeringTimer.restart();
        }
    }
}
