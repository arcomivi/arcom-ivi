import QtQuick 2.5

Item {
    id: buttonRoot
    width: parent.width
    height: parent.height

    //signals
    signal clicked

    //properties
    property bool active: false;

    property alias text: btnText.text
    property alias imgScale: buttonImage.scale
    property string pngname
    property bool keyUsing: false
    property int borderWidth: 0;
    //property color borderColor: Qt.rgba(0.0, 0.0, 0.0, 0.0)


    property int fontSize: 20
    property int fontBaseSize: 12
    property int fontDiff: 6

    property color textColor: "#D0D0D0"
    property alias btnImgSrc: buttonImage.source;
    property string btnImg: "";
    property string btnImgPressed: "";
    property bool wasClicked: false



    //functions
    function setButtonReleased(){
        if(active){
            buttonBorder.border.width = 1;
        } else {
            buttonBorder.border.width = 0;
        }
    }

    function setButtonClicked() {
        buttonBorder.border.width = 2;
    }

    function setButtonInactive() {
        activate();
    }

    function setButtonActive(){
        activate();
    }
    function activate(){
        console.log("active: "+active);
        active = !active;
        if(active){
            buttonBorder.border.width = 1;
            buttonBorder.border.color = "orange"
//            buttonImage.source = btnImgPressed;
        } else {
            buttonBorder.border.width = 0;
//            buttonImage.source = btnImg;
        }
        console.log("===> buttonBorder.border.width= "+buttonBorder.border.width+" for pngname= "+pngname);
    }



    //border with rounded corners
    Rectangle {
        id: buttonBorder
        color: "#636363";
        anchors.fill: buttonRoot
//        color: buttonRoot.borderColor
        border.color: "orange"
        border.width: borderWidth;
        radius: 2
    }

    MouseArea {
        id: mouseRegion
        hoverEnabled: true
        anchors.fill: parent

        onReleased: {
            setButtonReleased();
            buttonRoot.clicked();
        }

        onPressed: {
            setButtonClicked();
        }
    }

    Image {
        id: buttonImage
        source: btnImg
        anchors.centerIn: parent
        fillMode: Image.Pad;
        smooth: false;
        asynchronous: true;
    }

    Text {
        id: btnText
        renderType: Text.NativeRendering
        color: textColor
        anchors.centerIn: buttonImage
        style: Text.Raised
        font {
            pixelSize: fontSize;
            bold: true
        }
    }


}
