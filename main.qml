import QtQuick 2.11
import QtQuick.Window 2.11

Window {
    id : rootComponentId    // Идентификатор корневого элемента
    visible: true
    width: 640
    height: 480
    title: qsTr("Signals and Slots Demo")

    // Определяем сигнал, посредством которого передаём сообщение в C++ код
    signal qmlSignal(string msg)

    // Определяем слот, который будет получать информацию от MButtons
    // и пересылать этот сигнал дальше по цеполчке
    function receiveInfo(_count){
        qmlSignal("The count is " + _count)
    }

    // Определяем две кнопки, сообщения о нажатии которых
    // переадрессуем друг другу
    MButton {
        id : leftButtonId
        rectColor: "yellowgreen"

        // Связывание сигнала и слота может быть выполнено через свойство
        // (в данном примере это могло бы быть свойство target), либо
        // явным образом, через уведомление onCompleted
        //target: rightButtonId
    }

    MButton {
        id : rightButtonId
        rectColor: "dodgerblue"
        anchors.right: parent.right
        //target: leftButtonId
    }

    Component.onCompleted: {
        // Связываем сигналы и слоты каждой из кнопок друг с другом
        leftButtonId.notify.connect(rightButtonId.receiveInfo)
        rightButtonId.notify.connect(leftButtonId.receiveInfo)

        // Дополнительно связываем сигналы
        leftButtonId.notify.connect(rootComponentId.receiveInfo)
        rightButtonId.notify.connect(rootComponentId.receiveInfo)
    }
}


