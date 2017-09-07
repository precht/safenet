import QtQuick 2.5
import QtQuick.Controls 2.1
import Qt.labs.folderlistmodel 2.1
import "."

Item {
    id: serverBrowser
    property double sizeCoef: width / (width < height ? Props.lowerSize : Props.higherSize)
    signal loaderCall(string name, string path)

    function show(path) {
        headerLoader.sourceComponent = headerComponent
        browserLoader.sourceComponent = browserComponent
    }

    // layout

    Loader {
        id: headerLoader
        x: 0
        y: 0
        height: 55 * sizeCoef;
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }
    Loader {
        id: browserLoader
        x: 0
        y: headerLoader.height
        width: parent.width
        height: parent.height - headerLoader.height
        clip: true
    }

    // components to be loaded

    Component {
        id: headerComponent

        Rectangle {
            width: parent.width
            height: 55 * sizeCoef
            color: Props.secondBgColor

            Image {
                anchors.verticalCenter: parent.verticalCenter
                x: 13 * sizeCoef
                width: 28 * sizeCoef
                height: 28 * sizeCoef
                source: "image/connected.png"
            }
            Rectangle {
                id: textRect
                color: "transparent"
                x: 48 * sizeCoef
                width: parent.width - textRect.x
                height: 54 * sizeCoef
                clip: true
                Text {
                    text: manager.address
                    font.family: Props.fontName
                    font.pixelSize: Props.fontSize * sizeCoef
                    y: 17 * sizeCoef
                }
            }
            MouseArea {
                anchors.fill: parent
                onPressed: {
                    parent.x = 1
                    parent.color = Props.pressedBgColor
                }
                onReleased:  {
                    restoreOriginal()
                    manager.updateServerModel()
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
        id: browserComponent

        Rectangle {
            id: root

            property alias listView: listView

            Component {
                id: browserComponent

                Row {
                    width: listView.width
                    height: 54 * sizeCoef + 1
                    clip: true

                    Rectangle {
                        width: listView.width
                        height: 54 * sizeCoef
                        opacity: 1.0
                        color: Props.firstBgColor
                        // display model
                        Image {
                            id: leftIcon
                            anchors.verticalCenter: parent.verticalCenter
                            x: 15 * sizeCoef
                            width: 30 * sizeCoef
                            height: 30 * sizeCoef
                            source: "image/picture.png"
                        }
                        Rectangle {
                            color: "transparent"
                            x: 55 * sizeCoef
                            width: listView.width - (66 * sizeCoef)
                            height: 55 * sizeCoef
                            clip: true
                            Text {
                                text: display
                                font.family: Props.fontName
                                font.pixelSize: Props.fontSize * sizeCoef
                                y: 17 * sizeCoef
                            }
                        }
                        // separators
                        Rectangle {
                            y: parent.height - 1
                            width: listView.width
                            height: 1
                            color: Props.secondBgColor
                        }
                        // input handler
                        MouseArea {
                            anchors.fill: parent
                            onPressed: {
                                parent.x = 1
                                parent.color = Props.pressedBgColor
                            }
                            onReleased:  {
                                restoreOriginal()
                                serverBrowser.loaderCall("RemotePicture.qml", display)
                            }
                            onCanceled: {
                                restoreOriginal()
                            }
                            function restoreOriginal() {
                                parent.x = 0;
                                parent.color = Props.firstBgColor
                            }
                        }
                    }
                }
            }

            ListView {
                id: listView
                y: 0
                height: 160
                orientation: ListView.Vertical
                boundsBehavior: Flickable.StopAtBounds
                anchors.fill: parent
                delegate: browserComponent
                model: serverModel

                ScrollBar.vertical: ScrollBar {
                    width: 7 * sizeCoef
                    x: parent.width - width
                }
            }
        }
    }

    // handle android backbutton and esc key
    // NOTE: this require focus to be set to true by loader
    Keys.onReleased: {
        if(event.key === Qt.Key_Escape) {
            event.accepted = true
            if (parentPath !== "") {
                show(parentPath)
            }
        }
        if(event.key === Qt.Key_Back) {
            event.accepted = true
            if (parentPath !== "") {
                show(parentPath)
            } else {
                Qt.quit()
            }
        }
    }
}
