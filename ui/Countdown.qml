import QtQuick 2.15

Item {
    id: countDown
    property int seconds: 5
    anchors.centerIn: parent

    signal countDownFinished

    Image {
        id: countImg
        source: (innerTimer.running && countDown.seconds > 0) ? "assets/countdown/"+countDown.seconds+".png" : ""
        anchors.centerIn: parent
    }

    Timer {
        id: innerTimer
        repeat: true
        interval: 1000
        onTriggered: {
            countDown.seconds--;
            if (countDown.seconds == 0) {
                running = false;
                countDown.seconds = 5
                countDown.visible = false
                countDown.countDownFinished()
            }
        }
    }

    function start() {
        countDown.seconds = 5
        countDown.visible = true
        innerTimer.start();
    }
}
