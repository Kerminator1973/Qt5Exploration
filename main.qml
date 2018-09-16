import QtQuick 2.11
import QtQuick.Window 2.11

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Signals and Slots Demo")

    // Определяем две кнопки, сообщения о нажатии которых
    // переадрессуем друг другу
    MButton {
        id : leftButtonId
        rectColor: "yellowgreen"
    }

    MButton {
        id : rightButtonId
        rectColor: "dodgerblue"
        anchors.right: parent.right
    }

    Component.onCompleted: {
        // Связываем сигналы и слоты каждой из кнопок друг с другом
        leftButtonId.notify.connect(rightButtonId.receiveInfo)
        rightButtonId.notify.connect(leftButtonId.receiveInfo)
    }
}


