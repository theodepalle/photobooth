import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Window {
    id: mainWindow
    visible: true
    width: 1080
    height: 600
    title: qsTr("Photobooth")

    Connections{
        target: photoBooth
        function onNewState(state){
            console.log(state)
        }
    }

    StackView {
       id: stackView
       anchors.fill: parent
       initialItem: "StandbyPage.qml"
    }
}
