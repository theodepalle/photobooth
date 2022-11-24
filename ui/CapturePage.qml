import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

Rectangle{
    id: capturePage
    width: 1080
    height: 600
    color: "#2B2B2B"

    // Live View in backGround
    Image {
        id: liveView
        height: parent.height
        fillMode: Image.PreserveAspectFit
        anchors.centerIn: parent
        cache: false
    }

    Connections{
        target: photoBooth
        function onNewLiveView(viewPath){
            // trick to reload image
            liveView.source = ""
            liveView.source = "file://" + viewPath
        }
    }

    // Capture Logic
    property bool capturing : false

    Connections{
          target: photoBooth
          function onNewCapture(capturePath){
              capturing = false
              stackView.push("DisplayPage.qml", {"imagePath": capturePath})
          }
      }

    Image {
        id: captureButton
        source: "assets/camera-capture-icon.png"
        width: parent.width / 4
        fillMode: Image.PreserveAspectFit
        anchors.centerIn: parent
        visible: !capturing

        MouseArea {
            anchors.fill: captureButton
            onClicked: {
                capturing = true
                photoBooth.stopLiveView()
                photoBooth.capture()
            }
        }
    }

    BusyIndicator {
        id: busyIndicator
        width: parent.width / 5
        height: parent.width / 5
        anchors.centerIn: parent
        Material.accent: Material.Blue
        running : capturing
    }
}


