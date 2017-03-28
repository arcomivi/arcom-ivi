import QtQuick 2.5
//import QtQuick.Controls 1.4

Item {
    id: rootSteerMusic
    width: parent.width
    height: parent.height

    property int noOfElements: 7
    property int m_iCurrent: -1;
    property string m_sPrefix: "css/media/active/10/"

    signal goRight
    signal goLeft

    signal volup
    signal voldown
    signal prev
    signal playpause
    signal next
    signal seek(int percentage)

    function handleEnter(){
        console.log("handleEnter");
        if(m_iCurrent===-1) { m_iCurrent = 0; }
        steerMusicRow.children[m_iCurrent].setButtonActive();
    }

    function handleLeave() {
        if(m_iCurrent===-1) { m_iCurrent = 0; }
        steerMusicRow.children[m_iCurrent].setButtonInactive();
    }

    function handleRotCw(){
        console.log("handleRotCw");
        steerMusicRow.children[m_iCurrent].setButtonInactive();
        m_iCurrent++;
        if(m_iCurrent >= noOfElements) m_iCurrent = noOfElements - 1;
        steerMusicRow.children[m_iCurrent].setButtonActive();
    }

    function handleRotCcw(){
        console.log("handleRotCcw");
        steerMusicRow.children[m_iCurrent].setButtonInactive();
        m_iCurrent--;
        if(m_iCurrent < 0) m_iCurrent = 0;
        steerMusicRow.children[m_iCurrent].setButtonActive();
    }

    function setMediaProgress(progr){
        music_progress.setMediaProgress(progr);
    }

    function toggleProgress(){
        if(music_progress.borderWidth==0) music_progress.borderWidth=1;
        else if(music_progress.borderWidth==1) music_progress.borderWidth=0;
    }


    function handleRelease(){
        console.log("SteerNavi: handleRelease "+m_iCurrent);
        switch(m_iCurrent){
        case 1:
            steerMusicRow.children[m_iCurrent].setButtonReleased();
            rootSteerMusic.volup();
            break;
        case 2:
            steerMusicRow.children[m_iCurrent].setButtonReleased();
            rootSteerMusic.voldown();
            break;
        case 3:
            steerMusicRow.children[m_iCurrent].setButtonReleased();
            rootSteerMusic.prev();
            break;
        case 4:
            steerMusicRow.children[m_iCurrent].setButtonReleased();
            rootSteerMusic.playpause();
            break;
        case 5:
            steerMusicRow.children[m_iCurrent].setButtonReleased();
            rootSteerMusic.next();
            break;
        }
    }

    Column {
        width: parent.width
        height: parent.height

        Row {
            id: steerMusicRow;
            width: parent.width
            height: parent.height * 2 / 3;

            ACIButton {
                width: parent.width / 12
                height: parent.height
                id: audio_bt_left
                objectName: "audio-bt-left"
                keyUsing: true;
                opacity: 1
                pngname: "left"
                text: ""
                btnImg: g_cssprefix + m_sPrefix + pngname +".png"
                btnImgPressed: g_cssprefix + "css/common/inactive/"+pngname+".png"

                function setActive()   { setButtonActive();   }
                function setInactive() { setButtonInactive(); }
                function setClicked()  { setButtonClicked(); }
                onClicked: { rootSteerMusic.goLeft(); wasClicked = false; }
            }

            ACIButton {
                width: parent.width / 6
                height: parent.height
                id: audio_bt_volup
                //objectName: "audio-bt-volup"
                keyUsing: true;
                opacity: 1
                pngname: "volup"
                text: ""
                btnImg: g_cssprefix + m_sPrefix + pngname +".png"
                btnImgPressed: g_cssprefix + "css/media/inactive/"+pngname+".png"
                function setActive()   { setButtonActive();   }
                function setInactive() { setButtonInactive(); }
                function setClicked()  { setButtonClicked();  }
                onClicked: { rootSteerMusic.volup(); wasClicked = false; }
            }

            ACIButton {
                width: parent.width / 6
                height: parent.height
                id: audio_bt_voldown
                //objectName: "audio-bt-voldown"
                keyUsing: true;
                opacity: 1
                pngname: "voldown"
                text: ""
                btnImg: g_cssprefix + m_sPrefix + pngname +".png"
                btnImgPressed: g_cssprefix + "css/media/inactive/"+pngname+".png"
                function setActive()   { setButtonActive();   }
                function setInactive() { setButtonInactive(); }
                function setClicked()  { setButtonClicked();  }
                onClicked: { rootSteerMusic.voldown(); wasClicked = false; }
            }

            ACIButton {
                width: parent.width / 6
                height: parent.height
                id: audio_bt_last
                //objectName: "audio-bt-last"
                keyUsing: true;
                opacity: 1
                pngname: "last"
                text: ""
                btnImg: g_cssprefix + m_sPrefix + pngname +".png"
                btnImgPressed: g_cssprefix + "css/media/inactive/"+pngname+".png"
                function setActive()   { setButtonActive();   }
                function setInactive() { setButtonInactive(); }
                function setClicked()  { setButtonClicked(); }
                onClicked: { rootSteerMusic.prev(); wasClicked = false; }
            }

            ACIButton {
                width: parent.width / 6
                height: parent.height
                id: audio_bt_play
                //objectName: "audio-bt-play"
                keyUsing: true;
                opacity: 1
                pngname: "play"
                text: ""
                btnImg: g_cssprefix + m_sPrefix + pngname +".png"
                btnImgPressed: g_cssprefix + "css/media/inactive/"+pngname+".png"
                function setActive()   { setButtonActive();   }
                function setInactive() { setButtonInactive(); }
                function setClicked()  { setButtonClicked();  }
                onClicked: { rootSteerMusic.playpause(); wasClicked = false; }
            }

            ACIButton {
                width: parent.width / 6
                height: parent.height
                id: audio_bt_next
                objectName: "audio-bt-next"
                keyUsing: true;
                opacity: 1
                pngname: "next"
                text: ""
                btnImg: g_cssprefix + m_sPrefix + pngname +".png"
                btnImgPressed: g_cssprefix + "css/media/inactive/"+pngname+".png"
                function setActive()   { setButtonActive();   }
                function setInactive() { setButtonInactive(); }
                function setClicked()  { setButtonClicked();  }
                onClicked: { rootSteerMusic.next(); wasClicked = false; }
            }

            ACIButton {
                width: parent.width / 12
                height: parent.height
                id: audio_bt_right
                objectName: "audio-bt-right"
                keyUsing: true;
                opacity: 1
                pngname: "right"
                text: ""
                btnImg: g_cssprefix + "css/common/active/13/"+ pngname +".png"
                btnImgPressed: g_cssprefix + "css/common/inactive/"+pngname+".png"

                function setActive()   { setButtonActive();   }
                function setInactive() { setButtonInactive(); }
                function setClicked()  { setButtonClicked();  }
                onClicked: { rootSteerMusic.goRight(); wasClicked = false; }
            }
        }
        ACIProgressBar {
            id: music_progress;

            width: parent.width;
            height: parent.height / 3;
            onSeek: { rootSteerMusic.seek(percentage); }
        }
    }
}
