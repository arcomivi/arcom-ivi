import QtQuick 2.5

//import ACIElements 1.0
import ACIStyle 1.0

Item {
    id: mainview

    // ==> properties
    property int m_current: -1
    property string g_cssprefix: Qt.platform.os==="windows"?"file:///D:/temp/ws/arcomivi/arcomivi/":"/usr/share/arcomivi/";

    //  ==> functions
    Connections {
        target: $media
    }

    Connections {
        target: $pageNavigation

        onLoadView: {
            console.log("LoadView: "+aView);
            viewsSwitcher.goToView(0, aView);
            mainview.children[$pageNavigation.current].handleEnter();
        }

        onLoadSteering: {
            viewSteerings.goToSteering(aSteering)
        }

        onHandleRelease: {
            mainview.children[$pageNavigation.current].handleRelease();
        }

        onHandleRot: {
            mainview.children[$pageNavigation.current].handleRot(direction);
        }

        onHandleDirUp: {
            mainview.children[$pageNavigation.current].handleDirUp();
        }

        onHandleDirDown: {
            mainview.children[$pageNavigation.current].handleDirDown();
        }
    }

    function chooseScreen(){

    }

    function watchVideo(name){
        console.log("watchVideo:"+name);
        m_current = 4;
        videoLoader.source = "ACIVideoView.qml";
        videoLoader.item.setVideoSource(name);
    }

    Component.onCompleted: {
        console.log(Qt.platform.os);
        $pageNavigation.init();
//        if(m_current===-1) { m_current = 0; }
//        mainview.children[m_current].handleEnter();
//        viewsSwitcher.goToView(0, "ACIHomeView.qml");
    }

    // ==> UI elements
    // ===> Main Menu (m_current === 0)
    MainMenu {
        id: mainMenu;
        objectName: "mainMenu"
        width: parent.width;
        height: Math.floor(parent.height*0.1);
        anchors.top: mainview.top;

        aGridModel: $mainViewModel.listModel


        function handleDirDown(){
            console.log("mainMenu.handleDirDown");
            handleLeave();
            mainview.m_current = 1;
            viewsSwitcher.handleEnter();
        }
    }

    //(m_current === 1)
    ViewsSwitcher {
        id: viewsSwitcher
        width: parent.width;
        height: Math.floor(parent.height*0.7);
        anchors.top: mainMenu.bottom;        

        function handleDirUp(){
            handleLeave();
            $pageNavigation.current=0;
            mainMenu.handleEnter();
        }

        function handleDirDown(){
            handleLeave();
            $pageNavigation.current=2;
            viewSteerings.goToSteering(0)
        }

        function goToView(view, qmlname){
            if(viewsSwitcher.exists(qmlname)===false){
//                console.log("creating: "+qmlname);
                var cmp = Qt.createComponent(qmlname);
                cmp.createObject(viewsSwitcher.itemObject, {})
            }
            jumpTo(view, qmlname);
        }

        pages: [
            //0
            //ACIHomeView
            //ACIMedia - list: Music, Video, Pictures, Radio, Television
            //ACIMusic - list: all from Music,
            //ACIVideo - list: all from Videos
            //ACIPictures - list: all from Pictures
            //ACIRadio - list: radio stations
            //ACITelevision - list: Streams, DVB

            //0
            Loader { id: homeViewLoader; anchors.fill: parent; },
            //1
            Loader { id: mediaViewLoader; anchors.fill: parent; },
            //2
            Loader { id: settingsViewLoader; anchors.fill: parent; },
            //3
            Loader { id: naviViewLoader; anchors.fill: parent; }
        ]

        Connections {
            target: mediaViewLoader.item;
            onGoUp: {
                mainview.m_current = 0;
                mainview.children[m_current].handleEnter();
            }
        }

        Connections {
            target: settingsViewLoader.item;
            onGoUp: {
                mainview.m_current = 0;
                mainview.children[m_current].handleEnter();
            }
            onUpdate: {
                mainview.update();
            }
        }
    }

    //(m_current === 2)
    Steerings {
        id: viewSteerings
        width: parent.width;
        height: Math.floor(parent.height*0.15);
        anchors {
            bottom: statusBar.top;
        }
        onBackToPrevious: { $pageNavigation.current=1; }
    }

    //status bar
    //(m_current === 3)
    StatusBar {
        id: statusBar;
        width: parent.width;
        height: Math.floor(parent.height*0.05);
        anchors {
            bottom: mainview.bottom;
        }
    }

    //(m_current === 4)
    Loader { id: videoLoader; anchors.fill: parent;
        function handleRot(direction){
            videoLoader.item.handleRot(direction);
        }
    }
    //(m_current === 5)
    Loader { id: genericLoader; anchors.fill: parent;
        function handleRot(direction){
            genericLoader.item.handleRot(direction);
        }
        function handleRelease(){
            genericLoader.item.handleRelease();
        }
    }
    Connections {
        target: genericLoader.item;

    }
}
