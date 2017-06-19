import QtQuick 2.0
import "."

Item {
    property double sizeCoef: width / (width < height ? Props.lowerSize : Props.higherSize)

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
            border.width: 1
            border.color: Props.secondBgColor
            TextInput {
                text: "localhost"
                anchors.verticalCenter: parent.verticalCenter
                x: 3 * sizeCoef
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
            border.width: 1
            border.color: Props.secondBgColor
            TextInput {
                text: "8580"
                anchors.verticalCenter: parent.verticalCenter
                x: 3 * sizeCoef
                width: parent.width - this.x
                font.family: Props.fontName
                font.pixelSize: Props.fontSize * sizeCoef
            }
        }

    }
}
