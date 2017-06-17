import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Window 2.0
import "."

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Safenet"

    signal loaderCall(string name, string path)

    Loader {
        id: loader
        focus: true
        anchors.fill: parent
    }

    Component.onCompleted: {
        loader.source = "FileBrowser.qml"
        loader.item.show(Props.startPath)
        loader.item.focus = true
    }

    Connections {
        target: loader.item
        onLoaderCall: {
            loader.source = name
            loader.item.show(path)
            loader.item.focus = true
        }
    }

}
