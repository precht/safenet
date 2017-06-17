import QtQuick 2.5
import "."

Item {
    id: pictureViewer

    property double sizeCoef: width / (width + 160 < height ? Props.lowerSize : Props.higherSize)
    property string path
    property string folderPath
    signal loaderCall(string name, string path)

    function show(path, folderPath) {
        pictureViewer.path = path
        var result = path.match(/(^file:\/\/\/|)(.*)\/[^\/]*$/)
        pictureViewer.folderPath = result[1] !== "" ? result[1] + result[2] : ""
        headerLoader.sourceComponent = headerComponent
        viewerLoader.sourceComponent = viewerComponent
    }

    Loader {
        id: headerLoader
        height: 55 * sizeCoef
        width: parent.width
    }

    Rectangle {
        property alias viewerLoader: viewerLoader
        y: headerLoader.height
        height: parent.height - headerLoader.height
        width: parent.width
        color: "transparent"
        clip: true

        Loader {
            id: viewerLoader
            height: 420 * sizeCoef
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width
        }
    }

    Component {
        id: headerComponent

        Rectangle {
            property string currentFolder: folderPath.match(/([^\/]*$)/)[1]
            width: parent.width
            height: 55 * sizeCoef
            color: Props.secondBgColor

            Image {
                anchors.verticalCenter: parent.verticalCenter
                x: 15 * sizeCoef
                width: 30 * sizeCoef
                height: 30 * sizeCoef
                visible: currentFolder !== ""
                source: "images/folder.png"
            }
            Rectangle {
                id: textArea
                x: 55 * sizeCoef
                width: parent.width - textArea.x
                height: 54 * sizeCoef
                color: "transparent"
                clip: true
                Text {
                    text: currentFolder
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
                    if (folderPath !== "") {
                        parent.x = 1
                        parent.color = Props.pressedBgColor
                    }
                }
                onReleased:  {
                    restoreOriginal()
                    pictureViewer.loaderCall("FileBrowser.qml", folderPath)
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
    }

    Component {
        id: viewerComponent

        Item {
            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                height: 300 * sizeCoef
                width: 300 * sizeCoef
                color: "transparent"

                Image {
                    id: image
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    source: path
                }
            }
            Rectangle {
                y: 320 * sizeCoef
                height: 54 * sizeCoef
                width: 100 * sizeCoef
                anchors.horizontalCenter: parent.horizontalCenter
                color: Props.secondBgColor
                Text {
                    text: "Upload"
                    font.family: Props.fontName
                    font.pixelSize: Props.fontSize * sizeCoef
                    x: 22 * sizeCoef
                    y: 16 * sizeCoef

                }
                Rectangle {
                    x: 0; y: 0
                    height: 1; width: parent.width
                    color: Props.secondBgColor
                }
                Rectangle {
                    x: 0; y: parent.height - 1
                    height: 1; width: parent.width
                    color: Props.secondBgColor
                }
                Rectangle {
                    x: 0;  y: 0
                    height: parent.height;  width: 1
                    color: Props.secondBgColor
                }
                Rectangle {
                    x: parent.width;  y: 0
                    height: parent.height;  width: 1
                    color: Props.secondBgColor
                }
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        parent.color = Props.pressedBgColor
                    }
                    onReleased:  {
                        restoreOriginal()
                        // TODO: pressed action
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

    // handle android backbutton and esc key
    // NOTE: this require focus to be set to true by loader
    Keys.onReleased: {
        if(event.key === Qt.Key_Escape || event.key === Qt.Key_Back) {
            event.accepted = true
            pictureViewer.loaderCall("FileBrowser.qml", folderPath)
        }
    }
}
