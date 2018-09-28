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
    function receiveInfo(_count) {
        qmlSignal("The count is " + _count)
    }

    function logReceivedList(someArray) {

        console.log("Some list is received");
        for (var i=0; i < someArray.length; i++) {
            console.log("Array item:", someArray[i]);
            listModel.addElementToList(someArray[i]);
        }
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
        objectName: "TheSecondButton"
        rectColor: "dodgerblue"
        anchors.right: parent.right
        //target: leftButtonId

        function logUsefulInfo(msg) {
            console.log("It is useful information: " + msg);
            return 55;
        }
    }

    // ListView и ListModel используются для демонстрации возможности
    // динамически добавлять элементы в список
    ListView {
        id: listView
        anchors.left: leftButtonId.right
        width: 150
        height: 250

        model: listModel
        delegate: Rectangle {
            width: listView.width
            height: listView.height / 4
            color: "yellow"

            Text {
                text: hour  // Текст берём из свойства "hour"
                anchors.centerIn: parent
            }
        }
    }

    ListModel {
        id: listModel
        Component.onCompleted: {
            // Пока ничего не добавляем
        }

        function addElementToList(element) {
            // Добавляем в список пару key-value, в котором ключем
            // является поле "hour", а значение передаётся из вне
            append( {hour: element.toString()} );
        }
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


