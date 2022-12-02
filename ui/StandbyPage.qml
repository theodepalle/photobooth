import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: standByPage
    width: 1080
    height: 600
    color: "#FFFFFF"

    Text {
        id: touchText
        anchors.centerIn: parent
        text: "Touchez <br> pour commencer !"
        font.family: dancingScriptFont.font.family
        font.pointSize: 64
        color: "#2B2B2B"
        horizontalAlignment: Text.AlignHCenter
    }

    Image {
        id: touchIcon
        source: "assets/touch-icon.png"
        fillMode: Image.PreserveAspectFit
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: parent.height / 8
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            photoBooth.startLiveView()
            stackView.push("CapturePage.qml")
        }
    }
}
