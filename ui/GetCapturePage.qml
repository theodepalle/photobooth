import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: getCapturePage
    width: 1080
    height: 600
    color: "#FFFFFF"

    property string imagePath: ""

    Text {
        id: getPictureText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: parent.height / 20
        text: "Obtenir ma photo !"
        font.family: dancingScriptFont.font.family
        font.pointSize: 48
        color: "#2B2B2B"
        horizontalAlignment: Text.AlignHCenter
    }

    Text {
        id: infoText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: getPictureText.bottom
        text: "(Toutes les photos sont enregistrées et passées à l’organisateur de l’évènement)"
        font.family: dancingScriptFont.font.family
        font.pointSize: 24
        color: "#2B2B2B"
        horizontalAlignment: Text.AlignHCenter
    }

    Rectangle {
        id: emailButton
        width: 600
        height: 125
        radius : 15
        anchors.centerIn: parent
        anchors.topMargin: 40
        color: "#AAAAAA"
        border.color: "#2B2B2B"

        Text {
            anchors.centerIn: parent
            text: "Envoie par email"
            font.family: dancingScriptFont.font.family
            font.pointSize: 32
            color: "#2B2B2B"
            horizontalAlignment: Text.AlignHCenter
        }

        MouseArea {
            anchors.fill: parent
            onClicked: stackView.push("EmailPage.qml", {"imagePath": imagePath})
        }
    }

    Image {
        id: finishButton
        source: "assets/green-button.png"
        fillMode: Image.PreserveAspectFit
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: parent.height / 40

        Text {
            anchors.centerIn: parent
            text: "J'ai finis !"
            font.family: dancingScriptFont.font.family
            font.pointSize: 14
            color: "#FFFFFF"
            horizontalAlignment: Text.AlignHCenter
        }

        MouseArea {
            anchors.fill: parent
            onClicked: stackView.pop(null)
        }
    }

    Image {
        id: newPicButton
        source: "assets/green-button.png"
        fillMode: Image.PreserveAspectFit
        anchors.bottom: finishButton.top
        anchors.right: parent.right
        anchors.margins: parent.height / 40

        Text {
            anchors.centerIn: parent
            text: "Nouvelle Photo !"
            font.family: dancingScriptFont.font.family
            font.pointSize: 14
            color: "#FFFFFF"
            horizontalAlignment: Text.AlignHCenter
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                stackView.pop(null)
                photoBooth.startLiveView()
                stackView.push("CapturePage.qml")
            }
        }
    }
}
