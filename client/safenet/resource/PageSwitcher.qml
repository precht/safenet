import QtQuick 2.0
import "."

// TODO: backkey and esc bindings

Item {
    property double sizeCoef: width / (width < height ? Props.lowerSize : Props.higherSize)
    property double screenRatio: width / height
    property int currentIndex: -1
    property int iconSize: 27

    function setIndex(index) {
        if (index !== currentIndex) {
            if (currentIndex === 0) {
                pageLoader.item.updateServer()
            }
            currentIndex = index
            switch(index) {
            case 0:  pageLoader.source = "SettingsPage.qml"; break
            case 1: pageLoader.source = "UploadPage.qml"; break
            case 2: pageLoader.source = "DownloadPage.qml"; break
            }
            pageLoader.item.focus = true
            updateFooter(index)
        }
    }

    function updateFooter(index) {
        settingsPageButton.color = index === 0 ? Props.firstBgColor : Props.secondBgColor
        uploadPageButton.color = index === 1 ? Props.firstBgColor : Props.secondBgColor
        downloadPageButton.color = index === 2 ? Props.firstBgColor : Props.secondBgColor
    }

    Loader {
        id: pageLoader
        width: parent.width
        height: parent.height - footer.height

        Component.onCompleted: setIndex(0)
    }

    Row {
        id: footer
        y: parent.height - footer.height
        height: 55 * sizeCoef
        width: parent.width

        Rectangle {
            id: settingsPageButton
            height: parent.height
            width: parent.width / 3 - 1
            Image {
                anchors.centerIn: parent
                width: (iconSize - 3) * sizeCoef
                height: (iconSize - 3) * sizeCoef
                source: "image/settings.png"
            }
            MouseArea {
                id: settigsArea
                anchors.fill: parent
                onPressed: {
                    if (currentIndex !== 0) {
                        parent.color = Props.pressedBgColor
                    }
                }
                onReleased:  {
                    setIndex(0)
                }
                onCanceled: {
                    if (currentIndex !== 0) {
                        parent.color = Props.secondBgColor
                    }
                }
            }
        }
        Rectangle { // separator
            width: 2
            height: parent.height
            color: Props.firstBgColor;
        }
        Rectangle {
            id: uploadPageButton
            height: parent.height
            width: parent.width / 3 - 2

            Image {
                anchors.centerIn: parent
                width: iconSize * sizeCoef
                height: iconSize * sizeCoef
                source: "image/upload.png"
            }
            MouseArea {
                id: uploadArea
                anchors.fill: parent
                onPressed: {
                    if (currentIndex !== 1) {
                        parent.color = Props.pressedBgColor
                    }
                }
                onReleased:  {
                    setIndex(1)
                }
                onCanceled: {
                    if (currentIndex !== 1) {
                        parent.color = Props.secondBgColor
                    }
                }
            }
        }
        Rectangle { // separator
            width: 2
            height: parent.height
            color: Props.firstBgColor;
        }
        Rectangle {
            id: downloadPageButton
            height: parent.height
            width: parent.width / 3 - 1

            Image {
                anchors.centerIn: parent
                width: iconSize * sizeCoef
                height: iconSize * sizeCoef
                source: "image/download.png"
            }
            MouseArea {
                id: downloadArea
                anchors.fill: parent
                onPressed: {
                    if (currentIndex !== 2) {
                        parent.color = Props.pressedBgColor
                    }
                }
                onReleased:  {
                    setIndex(2)
                }
                onCanceled: {
                    if (currentIndex !== 2) {
                        parent.color = Props.secondBgColor
                    }
                }
            }
        }
    }

    Rectangle { // footer top line
        y: footer.y
        width: parent.width;
        height: 2
        color: Props.secondBgColor;
    }


}
