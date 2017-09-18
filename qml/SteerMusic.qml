import QtQuick 2.5

Item {
    id: root
    width: parent.width
    height: parent.height
    property var gridModel;
    property Item currentItem

    signal goRight
    signal goLeft
    signal goUp

    signal volup
    signal voldown
    signal prev
    signal playpause
    signal next
    signal seek(int percentage)

    function handleEnter(){
        steerMusicGrid.currentItem.buttonModel.active=true;
    }

    function handleLeave() {
        steerMusicGrid.currentItem.buttonModel.active=false;
    }

    function handleRotCw(){

        if(music_progress.borderWidth===1){ //we are on a progress bar
            return;
        }
        steerMusicGrid.currentItem.buttonModel.active=false;
        if((steerMusicGrid.currentIndex + 1) < steerMusicGrid.count){
            steerMusicGrid.currentIndex = steerMusicGrid.currentIndex + 1;
        }
        steerMusicGrid.positionViewAtIndex(steerMusicGrid.currentIndex, GridView.Visible);
        steerMusicGrid.currentItem.buttonModel.active=true;
    }

    function handleRotCcw(){
        if(music_progress.borderWidth===1){ //we are on a progress bar
            return;
        }
        steerMusicGrid.currentItem.buttonModel.active=false;
        if((steerMusicGrid.currentIndex-1)  >=0){
            steerMusicGrid.currentIndex = steerMusicGrid.currentIndex-1;
        }
        steerMusicGrid.positionViewAtIndex(steerMusicGrid.currentIndex, GridView.Visible);
        steerMusicGrid.currentItem.buttonModel.active=true;
    }

    function handleDirUp(){
        if(music_progress.borderWidth===1){
            music_progress.borderWidth=0;
            handleEnter();
        } else {
            goUp();
        }
    }

    function handleDirDown(){
        currentItem = music_progress;
        if(music_progress.borderWidth===0){
            handleLeave();
            music_progress.borderWidth=1;
        }
    }

    function setMediaProgress(progr){
        music_progress.setMediaProgress(progr);
    }

    function toggleProgress(){
        if(music_progress.borderWidth==0) music_progress.borderWidth=1;
        else if(music_progress.borderWidth==1) music_progress.borderWidth=0;
    }

    function handleRelease(){
        steerMusicGrid.currentItem.buttonModel.released();
    }

    Column {
        width: parent.width
        height: parent.height

        GridView {
            id: steerMusicGrid;
            width: parent.width; height: parent.height * 2 / 3;
            cellHeight: parent.height * 2 / 3;
            cellWidth: parent.width / steerMusicGrid.count
            model: gridModel
            delegate: ACIButton {
                height: parent.height
                width: steerMusicGrid.cellWidth
                buttonModel: model.itemData
            }
        }

        ACIProgressBar {
            id: music_progress;

            width: parent.width;
            height: parent.height / 3;
            onSeek: { root.seek(percentage); }
        }
    }
}
