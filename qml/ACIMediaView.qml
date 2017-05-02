import QtQuick 2.5

Item {
    // ==> properties
    id: acimediaview;
    objectName: "ACIMediaView.qml"
    anchors.fill: parent;

    // ==> signals
    signal goUp
    signal goDown

    // ==> functions
    function handleDirUp(){
        goUp();
    }    

    function handleRelease() {
        list.model.listClicked(list.currentIndex);
    }

    function handleRot(direction){
        if(direction === 0){
            rotateCW();
        } else {
            rotateCCW();
        }
    }

    function rotateCW(){
        if((list.currentIndex + 1) < list.count){
            list.currentIndex = list.currentIndex + 1;
        }
        list.positionViewAtIndex(list.currentIndex, ListView.Center);
    }

    function rotateCCW(){
        if((list.currentIndex-1)  >=0){
            list.currentIndex = list.currentIndex-1;
        }
        list.positionViewAtIndex(list.currentIndex, ListView.Center);
    }

    function handlePrevious(){
        rotateCCW();
    }

    function handleNext(){
        rotateCW();
    }

    // ==> QML elements
    Rectangle { id: acimediaviewbackground; anchors.fill: parent; color: "#636363" }

    ListView {
        id: list;
        width: Math.floor(parent.width);
        height: Math.floor(parent.height);
        focus: true
        anchors.fill: parent;
        clip: true;
        delegate:
            ACIStandardListItem {
            height: Math.floor(list.height / 5);

            onItemClicked: {
                list.currentIndex = index;
                $media.listModel.listClicked(list.currentIndex);
//                console.log("onItemClicked:"+descr);
            }
        }

        Component.onCompleted: {
            model = $media.listModel;
        }
    }
}
