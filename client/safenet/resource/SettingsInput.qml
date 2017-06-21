import QtQuick 2.5
import "."

Item {
    id: settingsInput
    property double sizeCoef: width / (width < height ? Props.lowerSize : Props.higherSize)

    // TODO header with authors

    signal loaderCall(string name)

    function updateServer() {
        manager.setIP(ipInput.text)
        manager.setPort(portInput.text)
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
            source: "image/info.png"
        }
        Rectangle {
            id: textArea
            x: 48 * sizeCoef
            width: parent.width - textArea.x
            height: 54 * sizeCoef
            color: "transparent"
            clip: true
            Text {
                text: "Safenet!"
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
                settingsInput.loaderCall("Authors.qml")
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
        y: header.height
        width: parent.width
        Item {
            anchors.horizontalCenter: parent.horizontalCenter
            height: 55 * sizeCoef
            width: 200 * sizeCoef
            Text {
                y: 20 * sizeCoef
                text: "Server IP Address:"
                font.family: Props.fontName
                font.pixelSize: Props.fontSize * sizeCoef
            }
        }
        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            height: 45 * sizeCoef
            width: 200 * sizeCoef
            border.width: 2
            border.color: Props.secondBgColor
            TextInput {
                id: ipInput
                text: manager.ip
                validator: RegExpValidator {
                    regExp: /(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}|localhost)/
                }

                anchors.verticalCenter: parent.verticalCenter
                x: 10 * sizeCoef
                width: parent.width - this.x
                font.family: Props.fontName
                font.pixelSize: Props.fontSize * sizeCoef
            }
        }
        Item {
            anchors.horizontalCenter: parent.horizontalCenter
            height: 55 * sizeCoef
            width: 200 * sizeCoef
            Text {
                y: 20 * sizeCoef
                text: "Server Port Number:"
                font.family: Props.fontName
                font.pixelSize: Props.fontSize * sizeCoef
            }
        }
        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            height: 45 * sizeCoef
            width: 200 * sizeCoef
            border.width: 2
            border.color: Props.secondBgColor
            TextInput {
                id: portInput
                text: manager.port
                validator: IntValidator { bottom: 0; top: 65535 }
                anchors.verticalCenter: parent.verticalCenter
                x: 10 * sizeCoef
                width: parent.width - this.x
                font.family: Props.fontName
                font.pixelSize: Props.fontSize * sizeCoef
            }
        }

    }
}
