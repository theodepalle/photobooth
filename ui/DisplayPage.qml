import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle{
    id: displayPage
    width: 1080
    height: 600
    color: "#2B2B2B"


    property string imagePath: ""

    Image {
        id: capturedImage
        height: parent.height
        fillMode: Image.PreserveAspectFit
        anchors.centerIn: parent
        source: "file://" + imagePath
    }

    Image {
        id: backButton
        source: "assets/back-arrow.png"
        width: parent.width * 0.1
        fillMode: Image.PreserveAspectFit
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 20

        MouseArea {
            anchors.fill: parent
            onClicked: stackView.pop(null)
        }
    }
}
