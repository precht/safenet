import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Window 2.0
import CryptoImage 1.0
import "."

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Safenet"

    PageSwitcher {
        anchors.fill: parent
        Component.onCompleted: setIndex(1)
    }

}
