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
    property variant center

    signal countryChanged(string country);
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
            title: "Country"
            ExclusiveGroup {
                id: countryGroup
                onCurrentChanged: appWindow.countryChanged(current.text)
            }
            MenuItem {
                text: "Liechtenstein"
                exclusiveGroup: countryGroup
                checkable: true
                checked: true
            }
            MenuItem {
                text: "South Korea"
                exclusiveGroup: countryGroup
                checkable: true
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
        if (map)
            map.destroy()
        else
            hideStatusBarTimer.start()

        map = Qt.createQmlObject('import QtLocation 5.5; Map {}', appWindow);
        map.plugin = Qt.createQmlObject('import QtLocation 5.5; Plugin {name: "osm"; PluginParameter {name: "osm.mapping.host"; value: "' + url + '"} }', map)
        for (var i = 0; i < map.supportedMapTypes.length; ++i) {
            if (map.supportedMapTypes[i].style === MapType.CustomMap) {
                map.activeMapType = map.supportedMapTypes[i]
                break
            }
        }
        map.center = center
        map.zoomLevel = 15
        map.anchors.fill = map.parent
        map.visible = true
    }

}
