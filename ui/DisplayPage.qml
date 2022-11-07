import QtQuick
import QtQuick.Controls

Rectangle{
    anchors.fill: parent

    Image {
        id: captureButton
        source: "file://" + PhotoBooth.getLastCapturePath()
        width: parent.width
        height: parent.height
        fillMode: Image.PreserveAspectFit
        anchors.centerIn: parent
        cache: false
    }

    Image {
        id: backButton
        source: "assets/back-arrow.png"
        width: parent.width * 0.1
        fillMode: Image.PreserveAspectFit

        MouseArea {
            anchors.fill: parent
            onClicked: stackView.pop()
        }
    }
}
