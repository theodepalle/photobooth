import QtQuick

Rectangle{
    anchors.fill: parent

    Image {
        id: captureButton
        source: "assets/camera-capture-icon.jpg"
        width: parent.width / 4
        fillMode: Image.PreserveAspectFit
        anchors.centerIn: parent

        MouseArea {
            anchors.fill: parent
            onClicked: {
                PhotoBooth.takePicture()
                stackView.push("DisplayPage.qml")
            }
        }
    }
}


