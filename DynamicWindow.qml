import QtQuick 2.11

Item {

    // Свойства, через которые пользовательский код настраивает элемент
    width: componentRectId.width
    height: componentRectId.height

    // Визуализационная часть компонента
    Rectangle {
        id : componentRectId

        // Значения "по умолчанию". Могут быть изменены через свойства
        // родительского элемента
        width: 100
        height: 100
        color: "green"

        Text {
            id : componentTextId
            anchors.centerIn: parent
            font.pointSize: 20
            text : "Hello"
        }
    }
}
