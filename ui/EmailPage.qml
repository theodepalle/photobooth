import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.VirtualKeyboard
import QtQuick.VirtualKeyboard.Settings

Rectangle{
    id: displayPage
    width: 1080
    height: 600
    color: "#FFFFFF"

    // to send email with image
    property string imagePath: ""

    // display button and text
    property bool sending: false
    property bool sent: false

    // to remove display text on first focus
    property bool firstFocus: true

    Flickable {
        id: flickable
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        width: parent.width
        height: Qt.inputMethod.visible ? parent.height - inputPanel.height : parent.height

        contentWidth: parent.width
        contentHeight: parent.height

        // workaround to unfocus TextInput and close VirtualKeyboard
        Pane {
            anchors.fill: parent
            background: Rectangle {
                anchors.fill: parent
                color: "#FFFFFF"
            }

            focusPolicy: Qt.ClickFocus
        }

        Image {
            id: envelope
            source: "assets/envelope.png"
            fillMode: Image.PreserveAspectFit
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: parent.width / 2 - (width / 2 + 30)
            anchors.topMargin: parent.height / 10
        }

        // fill mail and send display
        Column {
            anchors.top: envelope.bottom
            anchors.topMargin: 35
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            width: parent.width / 3
            spacing: 30
            visible: !sending

            Text {
                id: mailText
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Entre ton addresse mail <br> et clic sur \"Envoyer\""
                font.family: dancingScriptFont.font.family
                font.pointSize: 32
                color: "#2B2B2B"
                horizontalAlignment: Text.AlignHCenter
            }

            Rectangle {
                id: mailInput
                height: parent.height / 6
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 5
                color: "#FFFFFF"
                border.color: "#2B2B2B"

                TextInput {
                    id: mailTxtInput
                    anchors.fill: parent
                    text: "martin.dupont@gmail.com"
                    font.family: dancingScriptFont.font.family
                    font.pointSize: 18
                    color: "#2B2B2B"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    inputMethodHints: Qt.ImhLowercaseOnly
                    validator: RegularExpressionValidator { regularExpression:/(?:[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*|"(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21\x23-\x5b\x5d-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])*")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\[(?:(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9]))\.){3}(?:(2(5[0-5]|[0-4][0-9])|1[0-9][0-9]|[1-9]?[0-9])|[a-z0-9-]*[a-z0-9]:(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21-\x5a\x53-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])+)\])/ }
                    maximumLength: 40
                    onActiveFocusChanged:
                    {
                        // when focus is active Keyboard is displayed and we want to
                        //change our position to show the texInput in the flickable
                        if(activeFocus)
                        {
                            flickable.contentY = 200
                            if(firstFocus)
                            {
                                clear()
                                firstFocus = false
                            }
                        }
                    }

                }

                Text {
                    id: validatorTxt
                    anchors.top: mailInput.bottom
                    anchors.topMargin: 1
                    anchors.left: mailInput.left
                    anchors.leftMargin: 10
                    visible: !mailTxtInput.acceptableInput
                    text: "Adresse email invalide"
                    font.family: dancingScriptFont.font.family
                    font.pointSize: 12
                    color: "#CA4F4F"
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            Rectangle {
                id: sendButton
                height: parent.height / 6
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 5
                color: "#91A790"
                border.color: "#2B2B2B"

                Text {
                    id: sendText
                    anchors.centerIn: parent
                    text: "Envoyer"
                    font.family: dancingScriptFont.font.family
                    font.pointSize: 18
                    color: "#FFFFFF"
                    horizontalAlignment: Text.AlignHCenter
                }

                MouseArea {
                    anchors.fill: parent
                    enabled: mailTxtInput.acceptableInput
                    onClicked: {
                        photoBooth.sendCaptureByMail(imagePath, mailTxtInput.text)
                        sending = true
                    }
                }
            }
        }

        // sending display
        Column {
            anchors.top: envelope.bottom
            anchors.topMargin: 35
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            width: parent.width / 3
            spacing: 30
            visible: sending

            Text {
                id: sendingText
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Envoie en cours..."
                font.family: dancingScriptFont.font.family
                font.pointSize: 18
                color: "#2B2B2B"
                horizontalAlignment: Text.AlignHCenter
            }

            BusyIndicator {
                id: busyIndicator
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width / 4
                height: width
                Material.accent: "#2B2B2B"
                running: !sent
            }

            Rectangle {
                id: continueButton
                height: parent.height / 6
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                radius: 5
                color: "#91A790"
                border.color: "#2B2B2B"
                visible: sent

                Text {
                    anchors.centerIn: parent
                    text: "Continuer"
                    font.family: dancingScriptFont.font.family
                    font.pointSize: 18
                    color: "#FFFFFF"
                    horizontalAlignment: Text.AlignHCenter
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: stackView.pop()
                }
            }
        }

        Connections {
            target: photoBooth
            function onMailSended(){
                sendingText.text = "Envoyé !"
                sent = true
            }
        }
    }

    Image {
        id: backButton
        source: "assets/back-button.png"
        fillMode: Image.PreserveAspectFit
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: parent.height / 10
        visible: !sending

        MouseArea {
            anchors.fill: parent
            onClicked: stackView.pop()
        }
    }

    InputPanel {
        id: inputPanel
        y: Qt.inputMethod.visible ? parent.height - inputPanel.height : parent.height
        anchors.left: parent.left
        anchors.right: parent.right
    }

    Component.onCompleted: {
            VirtualKeyboardSettings.locale = "fr_FR"
    }
}
