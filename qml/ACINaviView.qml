import QtQuick 2.5
import QtPositioning 5.3
import QtLocation 5.6
//QGeoTileRequestManager: Failed to fetch tile (542,512,10) 5 times, giving up. Last error message was: 'Host orm.openstreetmap.org10 not found'
//http://c.tile.thunderforest.com/outdoors/10/512/542.png
Item {
    id: acinaviview;
    anchors.fill: parent;

    function handleRot(direction){
        //console.log("acinaviview.handleRot: "+direction)
        if(direction === 0){
            map.zoomLevel = map.zoomLevel + 1;
        } else {
            map.zoomLevel = map.zoomLevel - 1;
        }
    }

    Rectangle {
        color: "#636363"
        anchors.fill: parent;
        Text {
            id: navi
            renderType: Text.NativeRendering
            text: qsTr("navi")
            color: "white"
            anchors.centerIn: parent;
        }
    }


    Plugin {
        id: myPlugin
        name: "osm"
//        PluginParameter { name: "proxy"; value: "system" }
        PluginParameter { name: "osm.mapping.host"; value: "http://orm.openstreetmap.org/" }
//        PluginParameter { name: "osm.mapping.host"; value: "http://a.tile.openstreetmap.org/" }
        //PluginParameter { name: "osm.mapping.host"; value: "http://c.tile.thunderforest.com/outdoors/" }
        PluginParameter { name: "osm.mapping.copyright"; value: "ArCom-IVI" }
        PluginParameter { name: "osm.mapping.cache.directory"; value:  "/home/simon/Development/osmcache" }
        PluginParameter { name: "osm.mapping.offline.directory"; value: "/home/simon/Development/osmcache" }
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: myPlugin
        center: QtPositioning.coordinate(59.91, 10.75) // Oslo
//        center {
//            latitude: -27
//            longitude: 153
//        }
        zoomLevel: map.maximumZoomLevel

        Component.onCompleted: {
            for( var i_type in supportedMapTypes ) {
                console.log("supported type:"+supportedMapTypes[i_type].name);
                if( supportedMapTypes[i_type].name.localeCompare( "Custom URL Map" ) === 0 ) {
                    activeMapType = supportedMapTypes[i_type]
                }
            }
        }
    }
}
