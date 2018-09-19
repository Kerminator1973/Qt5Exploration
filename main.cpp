#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQuickWindow>
#include <QDebug>   // Заголовочный файл нужен для обеспечения возможности использовать qDebug()
#include "myqmlproxyclass.h"

// Ключевая ссылка:
// http://doc.qt.io/qt-5/qtqml-cppintegration-interactqmlfromcpp.html

static void createQMLDynamically(QQmlApplicationEngine& engine);

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

    // Настраивать свойства QML-компонентов можно двумя альтернативными способами
    wholeWindow->setProperty("title", "Signals and Slots (C++/QML)");

    // TODO: Попробовать вот такой вариант установки свойств компонента.
    // Есть разница между QQmlProperty or QMetaProperty
    //QQmlProperty(wholeWindow, "title").write("Signals and Slots (QML/C++)");

    // Добавить C++ класс пользовательского интерфейса в QML-слой:
    // https://evileg.com/ru/post/197/

    // Создаём QML-объект динамически
    createQMLDynamically(engine);

    // Создать QML-объект динамически, из JavaScript-кода:
    // http://doc.qt.io/qt-5/qtqml-javascript-dynamicobjectcreation.html

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

// Метод динамически создаёт QML-объект из QML-шаблона
static void createQMLDynamically(QQmlApplicationEngine& engine)
{
    QQuickWindow *window = qobject_cast<QQuickWindow*>(engine.rootObjects().at(0));
    if (!window) {
        qFatal("Error: Your root item has to be a window.");
        return;
    }

    QQmlComponent component(&engine, QUrl("qrc:/DynamicWindow.qml"));
    QQuickItem *object = qobject_cast<QQuickItem*>(component.create());

    // Важно заблокировать возможность удаления класса сборщиком мусора
    // JavaScript. Для этого нужно сделать:
    QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);

    // Настраиваем положение (подчинение) созданного компонента
    object->setParentItem(window->contentItem());   // Входной параметр - root
    object->setParent(&engine);

    // Настраиваем геометрические свойства объекта
    object->setProperty("x", QVariant(QString("300")));
    object->setProperty("y", QVariant(QString("300")));

    // TODO: Добавить настройку свойств динамически созданного объекта
    //object->setProperty("color", QVariant(QColor(255, 255, 255)));
    //object->setProperty("text", QVariant(QString("foo")));
}
