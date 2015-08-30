import QtQuick 2.5
import QtQuick.Controls 1.4
import QtLocation 5.5

ApplicationWindow {
    title: qsTr("Cute map")
    width: 640
    height: 480
    visible: true

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }

    Map {
        center {
            latitude: 47.141
            longitude: 9.521
        }
        zoomLevel: 15

        anchors.fill: parent
        activeMapType: supportedMapTypes[7] // FIXME: Remove dirty hard-coded index
        plugin: Plugin {
            name: "osm"
            PluginParameter { name: "osm.mapping.host"; value: "http://localhost:8080/" }
        }
    }

}
