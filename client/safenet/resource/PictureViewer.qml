import QtQuick 2.5
import CryptoImage 1.0
import "."

Item {
    id: pictureViewer

    property double sizeCoef: width / (width < height ? Props.lowerSize : Props.higherSize)
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

    Loader {
        id: viewerLoader
        anchors.fill: parent
        anchors.topMargin: headerLoader.height + 15
        width: parent.width
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
                source: "image/folder.png"
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
            Item {
                id: cimageWrapper
                x: 20 * sizeCoef
                y: 0
                width: parent.width - 40 * sizeCoef
                height: parent.height - 100 * sizeCoef

                CryptoImage {
                    id: cimage
                    anchors.fill: parent
                    source: path
                }
            }
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                y: cimageWrapper.height + 20 * sizeCoef

                Rectangle {
                    id: cbutton
                    property bool crytoMode: true
//                    y: cimageWrapper.height + 20 * sizeCoef
                    height: 54 * sizeCoef
                    width: 100 * sizeCoef
                    color: Props.secondBgColor
                    Text {
                        text: cbutton.crytoMode ? "Encrypt" : "Decrypt"
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
                            if (cbutton.crytoMode) {
                                cimage.encrypt()
                                cbutton.crytoMode = false
                            } else {
                                cimage.decrypt()
                                cbutton.crytoMode = true
                            }
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
                Rectangle {
                    x: 100 * sizeCoef
                    width: 50 * sizeCoef
                    height: 54 * sizeCoef
                }

                Rectangle {
                    id: ubutton
                    x: 250 * sizeCoef
//                    y: cimageWrapper.height + 20 * sizeCoef
                    height: 54 * sizeCoef
                    width: 100 * sizeCoef
                    color: Props.secondBgColor
                    Text {
                        text: "Upload"
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
                            manager.uploadImage(path)
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
