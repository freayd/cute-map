import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtLocation 5.5
import QtPositioning 5.5

ApplicationWindow {
    id: appWindow

    title: qsTr("Cute map")
    width: 640
    height: 480
    visible: true

    property Map map

    signal rendererChanged(string renderer);

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
        Menu {
            title: "Renderer"
            ExclusiveGroup {
                id: rendererGroup
                onCurrentChanged: appWindow.rendererChanged(current.text)
            }
            MenuItem {
                text: "Memphis"
                exclusiveGroup: rendererGroup
                checkable: true
                checked: true
            }
            MenuItem {
                text: "MapServer"
                exclusiveGroup: rendererGroup
                checkable: true
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

    Timer {
        id: hideStatusBarTimer
        interval: 1000
        onTriggered: statusBar.visible = false
    }

    function showMap(url) {
        var center, zoomLevel
        if (map) {
            center = map.center
            zoomLevel = map.zoomLevel
            map.destroy()
        } else {
            hideStatusBarTimer.start()
            center = QtPositioning.coordinate(47.141, 9.521)
            zoomLevel = 15
        }

        map = Qt.createQmlObject('import QtLocation 5.5; Map {}', appWindow);
        map.plugin = Qt.createQmlObject('import QtLocation 5.5; Plugin {name: "osm"; PluginParameter {name: "osm.mapping.host"; value: "' + url + '"} }', map)
        for (var i = 0; i < map.supportedMapTypes.length; ++i) {
            if (map.supportedMapTypes[i].style === MapType.CustomMap) {
                map.activeMapType = map.supportedMapTypes[i]
                break
            }
        }
        map.center = center
        map.zoomLevel = zoomLevel
        map.anchors.fill = map.parent
        map.visible = true
    }

}
