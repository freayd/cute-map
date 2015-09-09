import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
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

    statusBar: StatusBar {
        RowLayout {
            anchors.fill: parent
            Label {
                id: statusLabel
            }
            ProgressBar {
                id: statusProgress
                indeterminate: true
                Layout.alignment: Qt.AlignRight
            }
        }
    }

    function updateStatus(text, processed, total) {
        statusLabel.text = text
        if (total === -1) {
            statusProgress.indeterminate = true
        } else {
            statusProgress.indeterminate = false
            statusProgress.value = processed / total
        }
    }

    function showMap() {
        statusBar.visible = false
        map.visible = true
    }

    Map {
        id: map
        visible: false

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
