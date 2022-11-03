import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: mainWindow
    visible: true
    visibility: Window.Maximized
    title: qsTr("Photobooth")


    Button {
        id: captureButton
        text: qsTr("Capture")
        anchors.centerIn: parent
        onClicked: PhotoBooth.takePicture()
    }
}
