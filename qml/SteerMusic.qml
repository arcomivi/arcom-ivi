import QtQuick 2.5
//import QtQuick.Controls 1.4

Item {
    id: rootSteerMusic
    width: parent.width
    height: parent.height

    property int noOfElements: 5
    property int m_iCurrent: -1;
    property string m_sPrefix: "css/media/active/10/"
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
        gridModel.listClicked(steerMusicGrid.currentIndex);
    }

    property var gridModel;


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
            onSeek: { rootSteerMusic.seek(percentage); }
        }


//            ACIButton {
//                width: parent.width / noOfElements
//                height: parent.height
//                id: audio_bt_volup
//                keyUsing: true;
//                opacity: 1
//                pngname: "volup"
//                text: ""
//                btnImg: g_cssprefix + m_sPrefix + pngname +".png"
//                btnImgPressed: g_cssprefix + "css/media/inactive/"+pngname+".png"
//                function setActive()   { setButtonActive();   }
//                function setInactive() { setButtonInactive(); }
//                function setClicked()  { setButtonClicked();  }
//                onClicked: { rootSteerMusic.volup(); wasClicked = false; }
//            }


    }
}
