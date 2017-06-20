import QtQuick 2.0
import "."

Item {
    signal loaderCall(string name, string path)

    Loader {
        id: loader
        focus: true
        anchors.fill: parent
    }

    Component.onCompleted: {
        loader.source = "FileBrowser.qml"
        loader.item.show(manager.picturesFolder)
        loader.item.focus = true
        console.log("starting file browser in: " + manager.picturesFolder)
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
