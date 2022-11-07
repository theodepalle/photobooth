import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: mainWindow
    visible: true
    visibility: Window.Maximized
    title: qsTr("Photobooth")

    StackView {
        id : stackView
        anchors.fill: parent
        initialItem: "CapturePage.qml"
    }
}
