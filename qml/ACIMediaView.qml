import QtQuick 2.5

Item {
    id: acimediaview;
    signal goUp
    signal goDown



    function handleDirUp(){
        console.log("acimediaview.handleDirUp");
        goUp();
    }    

    function handleRelease() {
        console.log("acimediaview.handleRelease");
        list.model.listClicked(list.currentIndex);
    }

    function handleRot(direction){
        console.log("acimediaview.handleRot: "+direction)
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
                listModel.listClicked(list.currentIndex);
                console.log("onItemClicked:"+descr);
            }
        }

        Component.onCompleted: {
            console.log("list.count1 = "+list.count);
            model = listModel;
            console.log("list.count2 = "+list.count);

        }
    }
}
