import QtQuick 2.5

//import ACIElements 1.0

Item {
    id: mainview

    // ==> properties
    property int m_current: -1
    property string g_cssprefix: Qt.platform.os==="windows"?"file:///C:/Qt/ws/Test/Test/":"/usr/share/arcomivi/";

    signal update
    signal restart
    signal navigateTo(int widget);
    signal zoominNavi;
    signal zoomoutNavi;

    signal loadMedia
    signal volup
    signal voldown
    signal screenSelected(int screen)


    //  ==> functions
    function chooseScreen(){
        console.log("chooseScreen..........................");
        m_current = 5;
        genericLoader.source = "";
        genericLoader.source = "ACISelectScreen.qml";
    }

    function watchVideo(name){
        console.log("watchVideo:"+name);
        m_current = 4;
        videoLoader.source = "ACIVideoView.qml";
        videoLoader.item.setVideoSource(name);
    }

    function sendProgress(progress){
        viewSteerings.setMusicProgress(progress);
    }

    function handleRot(direction){
        if(m_current===-1) { m_current = 0; }
        mainview.children[m_current].handleRot(direction);
    }

    function handlePush(){
        mainview.children[m_current].handlePush();
    }

    function handleRelease() {
        mainview.children[m_current].handleRelease();
    }

    function handleDirUp(){
        mainview.children[m_current].handleDirUp();
    }

    function handleDirDown(){
        mainview.children[m_current].handleDirDown();
    }

    Component.onCompleted: {
        console.log(Qt.platform.os);
        if(m_current===-1) { m_current = 0; }
        mainview.children[m_current].handleEnter();
        viewsSwitcher.goToView(0, "ACIHomeView.qml");
    }

    // ==> UI elements
    // ===> Main Menu (m_current === 0)
    MainMenu {
        id: mainMenu;
        width: parent.width;
        height: Math.floor(parent.height*0.1);
        anchors.top: mainview.top;

        function handleDirUp(){}
        function handleDirDown(){
            console.log("mainMenu.handleDirDown");
            handleLeave();
            mainview.m_current = 1;
            viewsSwitcher.handleEnter();
        }

        onEnterMedia: {
            handleLeave();
            mainview.m_current = 1;
            viewsSwitcher.enterMedia();
        }

        onEnterSettings: {
            handleLeave();
            mainview.m_current = 1;
            viewsSwitcher.enterSettings();
        }
        onEnterNavigation: {
            handleLeave();
            mainview.m_current = 1;
            viewsSwitcher.enterNavigation();
        }
    }

    //(m_current === 1)
    ViewsSwitcher {
        id: viewsSwitcher
        width: parent.width;
        height: Math.floor(parent.height*0.7);
        anchors.top: mainMenu.bottom;

        property int m_viewsSwitcher_current: -1;

        function handleDirUp(){
            handleLeave();
            mainview.m_current = 0;
            mainMenu.handleEnter();
        }

        function handleDirDown(){
            handleLeave();
            mainview.m_current = 2;
            viewSteerings.handleEnter();
        }

        function enterNavigation(){
            viewsSwitcher.goToView(3, "ACINaviView.qml");
            viewSteerings.goToSteering(1);
        }

        function enterMedia(){
            loadMedia();
            viewsSwitcher.goToView(1, "ACIMediaView.qml");
            viewSteerings.goToSteering(0);
        }
        function enterSettings(){
            viewsSwitcher.goToView(2, "ACISettingsView.qml");
            viewSteerings.goToSteering(1);
        }

        function handleEnter(){
            //console.log("viewsSwitcher.handleEnter");
        }

        function handleLeave(){

        }

        function goToView(view, qmlname){
            m_viewsSwitcher_current = view;
            //console.log("viewsSwitcher-goToView: " + view +" ("+m_current+")");
            //console.log(pages[view]);
            jumpTo(view, qmlname);
        }

        function handleRot(direction){
            //console.log("ViewsSwitcher.handleRot"+ direction);
            pages[m_viewsSwitcher_current].item.handleRot(direction);
        }

        function handleRelease() {
            pages[m_viewsSwitcher_current].item.handleRelease();
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
                console.log("go up");
                mainview.m_current = 0;
                mainview.children[m_current].handleEnter();
            }            
        }
        Connections {
            target: settingsViewLoader.item;
            onGoUp: {
                console.log("go up");
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

        onVolup: mainview.volup();
        onVoldown: mainview.voldown();
        onPrevMusic: mediaViewLoader.item.handlePrevious();
        onPlaypauseMusic: mediaViewLoader.item.handleRelease();
        onNextMusic: mediaViewLoader.item.handleNext();
        onBackToPrevious: { mainview.m_current = 1; }
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
        onScreenSelected: {
            console.log("onScreenSelected: "+screen);
            mainview.screenSelected(screen);
            genericLoader.source = "";
            m_current = 1;
        }
    }
}
