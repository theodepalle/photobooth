import QtQuick 2.15

Rectangle {
    id: standByPage
    width: 1080
    height: 600
    color: "#2B2B2B"

    Text {
        anchors.centerIn: parent
        text: "Click to start !"
        font.pointSize: 24
        color: "blue"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            photoBooth.startLiveView()
            stackView.push("CapturePage.qml")
        }
    }

}
