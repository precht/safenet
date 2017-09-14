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
        loader.source = "RemoteFileBrowser.qml"
        loader.item.show()
        loader.item.focus = true
    }

    Connections {
        target: loader.item
        onLoaderCall: {
            console.log("path " + path)
            loader.source = name
            loader.item.show(path)
            loader.item.focus = true
        }
    }
}
