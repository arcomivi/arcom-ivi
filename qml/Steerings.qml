import QtQuick 2.5

Rectangle {
    id: steerings    
    color: Qt.rgba(0.0, 0.0, 0.0, 0.0)
//    property string g_cssprefix
    property Item currentSteering

    signal backToPrevious
    signal steerSwitched(int current)

    //for music, video, radio
    signal volup
    signal voldown

    signal zoominPics
    signal zoomoutPics

    signal zoominNavi
    signal zoomoutNavi

    signal prevMusic
    signal playpauseMusic
    signal nextMusic
    signal seekMusic(int percentage)

    signal prevVideo
    signal playpauseVideo
    signal nextVideo
    signal backToList
    signal seekVideo(int percentage)

    signal zoominPic
    signal zoomoutPic
    signal prevPic
    signal playpausePics
    signal nextPic

    signal prevRadio
    signal playpauseRadio
    signal nextRadio

    signal perspective
    signal northing
    signal options

    signal backToNavi
    signal navigate
    signal cancelNavigation

    signal setLayout

    signal refreshTracker
    signal removeDirectory

    //internet steerings
    signal connect

    Component.onCompleted: {
        steerSwitcher.jumpTo(1);
        currentSteering = steerSwitcher.pages[1];
    }

    //function
    function getCurrentIndex(){
        return currentSteering.getCurrentIndex();
    }

    function handleDirUp(){
        handleLeave();
        steerings.backToPrevious();
    }

    function handleRot(direction){
        if(direction===0){
            handleRotCw();
        } else {
            handleRotCcw();
        }
    }

    function handleRotCw(){
        currentSteering.handleRotCw();
    }

    function handleRotCcw(){
        currentSteering.handleRotCcw();
    }

    function handleEnter(){
        //console.log("steerings.handleEnter");
        currentSteering.handleEnter();
    }

    function handleLeave() {
        currentSteering.handleLeave();
    }

    function handlePush(){
        currentSteering.handlePush();
    }

    function handleRelease(){
        currentSteering.handleRelease();
    }

    function setCssPrefix(prefix){
        g_cssprefix = prefix;
    }

    function switchTo(idx){
        steerSwitcher.jumpTo(idx);
    }

    function goToSteering(category) {
        if(category===99){
            console.log(steerSwitcher.pages.length);
            category = steerSwitcher.pages.length-1;
        }
        if(category===98){
            console.log(steerSwitcher.pages.length);
            category = steerSwitcher.pages.length-2;
        }
        steerSwitcher.jumpTo(category);
        currentSteering = steerSwitcher.pages[category];
    }

    function setVideoProgress(progr){
        steerVideo.setMediaProgress(progr);
    }

    function setMusicProgress(progr){
        steerMusic.setMediaProgress(progr);
    }

    function toggleMusicProgress(){
        steerMusic.toggleProgress();
    }

    function toggleVideoProgress(){
        steerVideo.toggleProgress();
    }

    function setGeometry(vwidth, vheight){
        steerings.width = vwidth;
        steerings.height = vheight;
    }

    SteerSwitcher {
        id: steerSwitcher

        width: parent.width
        height: parent.height-1

        pages: [
            //0
            SteerMusic {
                id: steerMusic;
                onGoRight: { steerSwitched(4); }

                onVolup: steerings.volup();
                onVoldown: steerings.voldown();
                onPrev: steerings.prevMusic();
                onPlaypause: steerings.playpauseMusic();
                onNext: steerings.nextMusic();
                onSeek: steerings.seekMusic(percentage);
            },

//            //1
//            SteerVideo {
//                id: steerVideo
//                onGoLeft: steerings.backToList();
//                onGoRight: steerings.backToList();

//                onVolup: steerings.volup();
//                onVoldown: steerings.voldown();
//                onPrev: steerings.prevVideo();
//                onPlaypause: steerings.playpauseVideo();
//                onNext: steerings.nextVideo();
//                onSeek: steerings.seekVideo(percentage);
//            }

//            //2
//            SteerPictures {
//                id: steerPics
//                onZommin: steerings.zoominPic();
//                onZoomout: steerings.zoomoutPic();
//                onPrev: steerings.prevPic();
//                onPlaypause: steerings.playpausePics();
//                onNext: steerings.nextPic();
//            }

//            //3
//            SteerRadio {
//                id: steerRadio;

//                onVolup: steerings.volup();
//                onVoldown: steerings.voldown();
//                onPrev: steerings.prevRadio();
//                onPlaypause: steerings.playpauseRadio();
//                onNext: steerings.nextRadio();
//            }

//            //4
//            SteerNavi {
//                id: steerNavi;
//                onGoLeft: { steerSwitched(0); }

//                onZoomin: steerings.zoominNavi();
//                onZoomout: steerings.zoomoutNavi();
//                onPerspective: steerings.perspective();
//                onNorthing: steerings.northing();
//                onOptions: steerings.options();
//            }

//            //5 - navi options
//            SteerNaviOptions {
//                id: steerNaviOptions;
//                onGoLeft: steerings.backToNavi();
//                onCancelNavigation: steerings.cancelNavigation();
//                onNavigate: steerings.navigate();
//            }

//            //6 - navi bookmarks
//            SteerNaviBookmarks {
//                id: steerNaviBookmarks;
//                onGoLeft: steerings.backToPrevious();
//                onCancelNavigation: steerings.cancelNavigation();
//                onNavigate: steerings.navigate();
//            }

//            //7 - layouts
//            SteerLayouts {
//                id: steerLayouts;
//                onGoLeft: steerings.backToNavi();
//                onSet: steerings.setLayout();
//            }

//            //8 - Tracker Settings
//            SteerTracker {
//                id: steerTrackerSettings;
//                onRefresh: { steerings.refreshTracker(); }
//                onRemoveDirectory: { steerings.removeDirectory(); }
//            }
//            //9 - Internet
//            SteerInternet {
//                id: steerInternetSettings;
//                onGoBack: steerings.backToPrevious();
//                onEnter: steerings.connect();
//            }
//            //98 - logo with back button
//            SteerLogoBack {
//                id: steerLogoBack;
//                onGoBack: steerings.backToPrevious();
//            }

            //99 - empty
            SteerEmpty {
                id: steerEmpty;
            }
        ]
    }
}
