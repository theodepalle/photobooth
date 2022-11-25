import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: standByPage
    width: 1080
    height: 600
    color: "#2B2B2B"

    Text {
        id: touchText
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: parent.width / 5
        text: "Touchez <br> pour commencer !"
        font.family: dancingScriptFont.font.family
        font.pointSize: 64
        color: "white"
        horizontalAlignment: Text.AlignHCenter
    }

    Image {
        id: touchIcon
        source: "assets/touch-icon.png"
        fillMode: Image.PreserveAspectFit
        anchors.left: touchText.right
        anchors.top: touchText.top
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            photoBooth.startLiveView()
            stackView.push("CapturePage.qml")
        }
    }
}
