import QtQuick 2.5

Item {
    signal loaderCall(string name)

    function updateServer() {
        loader.item.updateServer()
    }


    Loader {
        id: loader
        focus: true
        anchors.fill: parent
    }

    Component.onCompleted: {
        loader.source = "SettingsInput.qml"
        loader.item.focus = true
    }

    Connections {
        target: loader.item
        onLoaderCall: {
            loader.source = name
            loader.item.focus = true
        }
    }
}
