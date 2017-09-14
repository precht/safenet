import QtQuick 2.5
import "."

Rectangle {
    id: authors
    property double sizeCoef: width / (width < height ? Props.lowerSize : Props.higherSize)
    color: Props.firstBgColor

    signal loaderCall(string name)

    function updateServer() {
    }


    Rectangle {
        id: header
        width: parent.width
        height: 55 * sizeCoef
        color: Props.secondBgColor

        Image {
            anchors.verticalCenter: parent.verticalCenter
            x: 13 * sizeCoef
            width: 30 * sizeCoef
            height: 30 * sizeCoef
            source: "image/left.png"
        }
        Rectangle {
            id: textArea
            x: 48 * sizeCoef
            width: parent.width - textArea.x
            height: 54 * sizeCoef
            color: "transparent"
            clip: true
            Text {
                text: ""
                font.family: Props.fontName
                font.pixelSize: Props.fontSize * sizeCoef
                y: 17 * sizeCoef
            }
        }
        Rectangle { // bottom line
            y: parent.height - 1
            width: parent.width;
            height: 1
            color: Props.secondBgColor
        }

        MouseArea {
            anchors.fill: parent
            onPressed: {
                parent.x = 1
                parent.color = Props.pressedBgColor
            }
            onReleased:  {
                restoreOriginal()
                authors.loaderCall("SettingsInput.qml")
            }
            onCanceled: {
                restoreOriginal()
            }
            function restoreOriginal() {
                parent.x = 0;
                parent.color = Props.secondBgColor
            }
        }
    }

    Column {
        anchors.topMargin: header.height
        anchors.verticalCenter: parent.verticalCenter
        y: header.height
        width: parent.width
        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            height: 45 * sizeCoef
            width: 230 * sizeCoef
            color: "transparent"

            Text {
                x: 15 * sizeCoef
                width: parent.width - textArea.x
                height: 54 * sizeCoef
                text: "Authors:"
                font.family: Props.fontName
                font.pixelSize: Props.fontSize * sizeCoef
                y: 17 * sizeCoef
            }
        }
        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            height: 45 * sizeCoef
            width: 230 * sizeCoef
            color: "transparent"

            Image {
                anchors.verticalCenter: parent.verticalCenter
                x: 13 * sizeCoef
                width: 30 * sizeCoef
                height: 30 * sizeCoef
                source: "image/user.png"
            }
            Text {
                x: 48 * sizeCoef
                width: parent.width - textArea.x
                height: 54 * sizeCoef
                text: "Jakub Precht"
                font.family: Props.fontName
                font.pixelSize: Props.fontSize * sizeCoef
                y: 17 * sizeCoef
            }
        }
        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            height: 45 * sizeCoef
            width: 230 * sizeCoef
            color: "transparent"

            Image {
                anchors.verticalCenter: parent.verticalCenter
                x: 13 * sizeCoef
                width: 30 * sizeCoef
                height: 30 * sizeCoef
                source: "image/user.png"
            }
            Text {
                x: 48 * sizeCoef
                width: parent.width - textArea.x
                height: 54 * sizeCoef
                text: "Jakub Krymarys"
                font.family: Props.fontName
                font.pixelSize: Props.fontSize * sizeCoef
                y: 17 * sizeCoef
            }
        }
        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            height: 45 * sizeCoef
            width: 230 * sizeCoef
            color: "transparent"

            Image {
                anchors.verticalCenter: parent.verticalCenter
                x: 13 * sizeCoef
                width: 30 * sizeCoef
                height: 30 * sizeCoef
                source: "image/user.png"
            }
            Text {
                x: 48 * sizeCoef
                width: parent.width - textArea.x
                height: 54 * sizeCoef
                text: "Anna Krasiukianis"
                font.family: Props.fontName
                font.pixelSize: Props.fontSize * sizeCoef
                y: 17 * sizeCoef
            }
        }
        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            height: 45 * sizeCoef
            width: 230 * sizeCoef
            color: "transparent"

            Image {
                anchors.verticalCenter: parent.verticalCenter
                x: 13 * sizeCoef
                width: 30 * sizeCoef
                height: 30 * sizeCoef
                source: "image/user.png"
            }
            Text {
                x: 48 * sizeCoef
                width: parent.width - textArea.x
                height: 54 * sizeCoef
                text: "Albert Kułakowski"
                font.family: Props.fontName
                font.pixelSize: Props.fontSize * sizeCoef
                y: 17 * sizeCoef
            }
        }

    }
}
