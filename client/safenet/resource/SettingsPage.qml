import QtQuick 2.0
import "."

Item {
    property double sizeCoef: width / (width < height ? Props.lowerSize : Props.higherSize)

    // TODO header with authors

    function updateServer() {
        manager.setIP(ipInput.text)
        manager.setPort(portInput.text)
    }

    Column {
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
