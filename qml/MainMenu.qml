import QtQuick 2.5

Item {
    id: rootMainMenu
    objectName: "rootMainMenu"

    // ==> properties
    property int noOfElements: 4
    property string btnPrefix: "internet-bt-"
    property int m_current: -1
//    property string g_cssprefix

    // ==> signals
    signal enterMedia
    signal enterNavigation
    signal enterSync
    signal enterSettings


    // ==> functions
//    function setGeometry(vwidth, vheight){
//        rootMainMenu.width = vwidth-1;
//        rootMainMenu.height = vheight-1;
//    }

    function setCssPrefix(prefix){
        g_cssprefix = prefix;
    }

    function handleLeave() {
        if(m_current===-1) { m_current = 0; }
        mainMenuRow.children[m_current].setButtonInactive();
    }

    function handleEnter() {
        if(m_current===-1) { m_current = 0; }
        mainMenuRow.children[m_current].setButtonActive();
    }

    function handleRelease(){
        switch(m_current){
        case 0:
            mainMenuRow.children[m_current].setButtonReleased();
            rootMainMenu.enterMedia();
            break;
        case 1:
            mainMenuRow.children[m_current].setButtonReleased();
            rootMainMenu.enterNavigation();
            break;
        case 2:
            mainMenuRow.children[m_current].setButtonReleased();
            rootMainMenu.enterSync();
            break;
        case 3:
            mainMenuRow.children[m_current].setButtonReleased();
            rootMainMenu.enterSettings();
            break;
        }
    }

    function handlePush(){
        switch(m_current){
        case 0:
            mainMenuRow.children[m_current].setButtonClicked();
            break;
        case 1:
            mainMenuRow.children[m_current].setButtonClicked();
            break;
        case 2:
            mainMenuRow.children[m_current].setButtonClicked();
            break;
        case 3:
            mainMenuRow.children[m_current].setButtonClicked();
            break;
        }
    }

    function handleRot(direction){
        if(m_current===-1) { m_current = 0; }
        console.log("rootMainMenu-handleRot"+ direction);
        if(direction===0){
            handleRotCw();
        } else {
            handleRotCcw();
        }
    }

    function handleRotCw(){
        console.log("handleRotCw");
        mainMenuRow.children[m_current].setButtonInactive();
        m_current++;
        if(m_current >= noOfElements) m_current = noOfElements - 1;
        console.log("handleRotCw"+m_current);
        mainMenuRow.children[m_current].setButtonActive();
    }

    function handleRotCcw(){
        mainMenuRow.children[m_current].setButtonInactive();
        m_current--;
        if(m_current < 0) m_current = 0;
        console.log("handleRotCcw"+m_current);
        mainMenuRow.children[m_current].setButtonActive();
    }



    Row {
        id: mainMenuRow;
        width: parent.width; height: parent.height;

        ACIButton {
            width: parent.width / noOfElements;
            height: parent.height
            id: mainmenu_bt_media
            keyUsing: true;
            pngname: "media"
            text: ""
            btnImg: g_cssprefix + "css/mainmenu/active/"+ pngname +"013.png"
            btnImgPressed: g_cssprefix + "css/mainmenu/inactive/"+pngname+".png"

            function setActive()   { setButtonActive();   }
            function setInactive() { setButtonInactive(); }
            function setClicked()  { setButtonClicked(); }
            function setReleased() { setButtonReleased(); }
            onClicked: {
                wasClicked = false;
                enterMedia();
            }
        }

        ACIButton {
            width: parent.width / noOfElements;
            height: parent.height
            id: mainmenu_bt_navi
            objectName: btnPrefix+"navi"
            //imgScale: 0.25
            keyUsing: true;
            opacity: 1
            pngname: "navi"
            text: ""
            btnImg: g_cssprefix + "css/mainmenu/active/"+ pngname +"013.png"
            btnImgPressed: g_cssprefix + "css/mainmenu/inactive/"+pngname+".png"

            function setActive()   { setButtonActive();   }
            function setInactive() { setButtonInactive(); }
            function setClicked()  { setButtonClicked(); }
            function setReleased() { setButtonReleased(); }
            onClicked: {
                wasClicked = false;
                enterNavigation();
            }
        }

        ACIButton {
            width: parent.width / noOfElements;
            height: parent.height
            id: mainmenu_bt_sync
            objectName: btnPrefix+"sync"
            //imgScale: 0.25
            keyUsing: true;
            opacity: 1
            pngname: "sync"
            text: ""
            btnImg: g_cssprefix + "css/mainmenu/active/"+ pngname +"013.png"
            btnImgPressed: g_cssprefix + "css/mainmenu/inactive/"+pngname+".png"
            onClicked: { wasClicked = false; }
        }

        ACIButton {
            width: parent.width / noOfElements;
            height: parent.height
            id: mainmenu_bt_options
            objectName: btnPrefix+"options"
            //imgScale: 0.25
            keyUsing: true;
            opacity: 1
            pngname: "options"
            text: ""
            btnImg: g_cssprefix + "css/mainmenu/active/"+ pngname +"013.png"
            btnImgPressed: g_cssprefix + "css/mainmenu/inactive/"+pngname+".png"

//            function setActive()   { setButtonActive();   }
//            function setInactive() { setButtonInactive(); }
//            function setClicked()  { setButtonClicked(); }
//            function setReleased() { setButtonReleased(); }
            onClicked: { wasClicked = false; enterSettings(); }
        }
    }
}
