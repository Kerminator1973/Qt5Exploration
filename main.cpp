#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>   // Загловочный файл нужен для обеспечения возможности использовать qDebug()
#include "myqmlproxyclass.h"

// Ключевая ссылка:
// http://doc.qt.io/qt-5/qtqml-cppintegration-interactqmlfromcpp.html

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    // Дальше выполняем настройки компонентов QML на странице
    QObject *wholeWindow = engine.rootObjects().first();

    // TODO: Попробовать вот такой вариант поиска компонентов
    //QObject *rect = object->findChild<QObject*>("rect");

    // Настраивать свойства свойства QML-компонентов можно двумя альтернативными способами
    wholeWindow->setProperty("title", "Signals and Slots (C++/QML)");

    // TODO: Попробовать вот такой вариант установки свойств компонента
    //QQmlProperty(wholeWindow, "title").write("Signals and Slots (QML/C++)");

    // Создаём Proxy-класс для обработки сигналов QML-кода.
    // Важно, чтобы класс MyQmlProxyClass был определён в отдельном заголовочной файле.
    MyQmlProxyClass myClass;

    // Указываем, что сигнал qmlSignal() главного окна QML, должен быть связан с методом
    // cppSlot() объекта myClass, который является Proxy-компонентом
    qDebug() << QObject::connect(wholeWindow, SIGNAL(qmlSignal(QString)), &myClass, SLOT(cppSlot(QString)));

    // Запускаем основной цикл обработки сообщений
    return app.exec();
}
