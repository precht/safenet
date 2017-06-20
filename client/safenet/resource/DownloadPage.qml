import QtQuick 2.0
import "."



Item {
    property double sizeCoef: width / (width < height ? Props.lowerSize : Props.higherSize)

    Item {
        anchors.centerIn: parent

        Rectangle {
            id: ubutton
            height: 54 * sizeCoef
            width: 100 * sizeCoef
            color: Props.secondBgColor
            Text {
                text: "Download"
                font.family: Props.fontName
                font.pixelSize: Props.fontSize * sizeCoef
                y: 16 * sizeCoef
                anchors.horizontalCenter: parent.horizontalCenter
            }
            MouseArea {
                anchors.fill: parent
                property bool isEncrypt: true
                onPressed: {
                    parent.color = Props.pressedBgColor
                }
                onReleased:  {
                    manager.downloadImage("senttoserver.png")
                    restoreOriginal()
                }
                onCanceled: {
                    restoreOriginal()
                }
                function restoreOriginal() {
                    parent.color = Props.secondBgColor
                }
            }
        }

    }
}
