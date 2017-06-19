pragma Singleton
import QtQuick 2.5

QtObject {

    property color textColor: "black"
    property color firstBgColor: "white"
    property color secondBgColor: "lightgrey"
    property color pressedBgColor: "lightblue"

    property int fontSize: 17
    property string fontName: "Source Code Pro"

    property int lowerSize: 320
    property int higherSize: 800

    property bool showFiles: true
    property string startPath: "file:///"
//    property string startPath: "file:///home/jakub/Pictures"
    property var formats: ["*.bmp", "*.png", "*.jpg", "*.jpeg"]
}
