import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle{
    id: capturePage
    width: 1080
    height: 600
    color: "#FFFFFF"

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
    Image {
        id: captureButton
        source: "assets/camera-capture-icon.png"
        height: parent.height / 2
        fillMode: Image.PreserveAspectFit
        anchors.centerIn: parent

        MouseArea {
            anchors.fill: captureButton
            onClicked: {
                captureButton.visible = false
                countDown.start()
            }
        }
    }

    Countdown {
        id: countDown
    }

    Item {
        id: smileDisplay
        visible: false
        width: parent.width
        height: parent.height
        anchors.centerIn: parent


        Image {
            id: smileImg
            source : "assets/smile.png"
            fillMode: Image.PreserveAspectFit
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height / 10
        }

        Text {
            id: smileText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: smileImg.top
            text: "Souriez !"
            font.family: dancingScriptFont.font.family
            font.pointSize: 128
            color: "#2B2B2B"
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Connections{
          target: countDown
          function onCountDownFinished(){
                photoBooth.stopLiveView()
                liveView.visible = false
                photoBooth.capture()
                smileDisplay.visible = true
          }
      }

    Connections{
          target: photoBooth
          function onNewCapture(capturePath){
              smileDisplay.visible = false
              liveView.visible = true
              captureButton.visible = true
              stackView.push("DisplayPage.qml", {"imagePath": capturePath})
          }
      }
}


