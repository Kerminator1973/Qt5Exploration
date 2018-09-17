#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>   // Заголовочный файл нужен для обеспечения возможности использовать qDebug()
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

    // Пример вызова QML-функции из C++ кода. Ключевой момент: поиск
    // QML-компонента осуществляется не по id, а по "objectName"
    QObject *pQmlComponent = wholeWindow->findChild<QObject*>("TheSecondButton");
    if(NULL != pQmlComponent) {

        QVariant returnedValue;
        QVariant msg = "Hello from C++";
        QMetaObject::invokeMethod(pQmlComponent, "logUsefulInfo",
            Q_RETURN_ARG(QVariant, returnedValue),
            Q_ARG(QVariant, msg));

        qDebug() << "The Result is:" << returnedValue;
    }

    // Настраивать свойства свойства QML-компонентов можно двумя альтернативными способами
    wholeWindow->setProperty("title", "Signals and Slots (C++/QML)");

    // TODO: Попробовать вот такой вариант установки свойств компонента
    //QQmlProperty(wholeWindow, "title").write("Signals and Slots (QML/C++)");

    // Создаём Proxy-класс для обработки сигналов QML-кода.
    // Важно, чтобы класс MyQmlProxyClass был определён в отдельном заголовочной файле.
    MyQmlProxyClass myClass;

    // Указываем, что сигнал qmlSignal() главного окна QML, должен быть связан с методом
    // cppSlot() объекта myClass, который является Proxy-компонентом
    qDebug() << "The connection complete status is: " <<
        QObject::connect(wholeWindow, SIGNAL(qmlSignal(QString)), &myClass, SLOT(cppSlot(QString)));

    // Запускаем основной цикл обработки сообщений
    return app.exec();
}
