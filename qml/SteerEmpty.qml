import QtQuick 2.5

Item {
    id: rootSteerEmpty
    width: parent.width
    height: parent.height

    //properties
    property int noOfElements: 0
    property string btnPrefix: "empty-bt-"
    property int m_current: -1

    Rectangle { color: "#636363"; anchors.fill: parent;
        Image {
            id: logo
            height: parent.height / 2;
            source: g_cssprefix + "css/common/inactive/enter.png"
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
            smooth: false;
        }
    }
}
