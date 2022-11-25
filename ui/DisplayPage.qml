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
        id: wrongButton
        source: "assets/wrong.png"
        fillMode: Image.PreserveAspectFit
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.margins: 8

        MouseArea {
            anchors.fill: parent
            onClicked: {
                photoBooth.startLiveView()
                stackView.pop()
            }
        }
    }

    Image {
        id: rightButton
        source: "assets/right.png"
        fillMode: Image.PreserveAspectFit
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.margins: 8

        MouseArea {
            anchors.fill: parent
            onClicked: stackView.pop(null)
        }
    }
}
