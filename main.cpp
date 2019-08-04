#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQuickWindow>
#include <QDebug>   // Заголовочный файл нужен для обеспечения возможности использовать qDebug()

// Включаемые файлы нужны для использования разделяемых между C++ и QML структур
#include <QQmlContext>
#include <QObject>
#include "sharedclass.h"

#include "myqmlproxyclass.h"
#include "mypersonallistmodel.h"


// Ключевая ссылка:
// http://doc.qt.io/qt-5/qtqml-cppintegration-interactqmlfromcpp.html

static void createQMLDynamically(QQmlApplicationEngine& engine);
static void sendStringListToQML(QObject *pWindow);

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Предоставляем возможность использования структуры, определённой
    // в C++ в QML-коде. Структуру нужно определить до загрузки QML
    SharedClass classObj;
    classObj.strObj.m_val = 78;
    classObj.strObj.m_name1 = "Maxim";
    classObj.strObj.m_name2 = "Rozhkov";
    engine.rootContext()->setContextProperty( "classObj", &classObj);

    // Создаём модель (для ListView) и заполняем её некоторыми данными
    MessageEntry m1(QString("1223"), QString("Apple"));
    MessageEntry m2(QString("1256"), QString("Amazon"));
    MessageEntry m3(QString("1278"), QString("Microsoft"));
    MessageEntry m4(QString("1309"), QString("Google"));

    MyPersonalListModel model;
    model.addEntry(m1);
    model.addEntry(m2);
    model.addEntry(m3);
    model.addEntry(m4);

    // Передаём в QML нашу модель с именем "quickListModel"
    engine.rootContext()->setContextProperty("quickListModel", &model);

    // Загружаем QML-документ
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    // Дальше выполняем настройки компонентов QML на странице
    QObject *wholeWindow = engine.rootObjects().first();

    // Пример вызова QML-функции из C++ кода. Ключевой момент: поиск
    // QML-компонента осуществляется не по id, а по "objectName"
    QObject *pQmlComponent = wholeWindow->findChild<QObject*>("TheSecondButton");
    if(nullptr != pQmlComponent) {

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
    MyQmlProxyClass myQMLProxyClass;

    // Указываем, что сигнал qmlSignal() главного окна QML, должен быть связан с методом
    // cppSlot() объекта myClass, который является Proxy-компонентом.
    //
    // Общий синтаксис вызова connect():
    // - Кто посылает
    // - Что посылает
    // - Кто обрабатывает
    // - Какой метод обрабатывает
    qDebug() << "The connection complete status is: " <<
        QObject::connect(wholeWindow, SIGNAL(qmlSignal(QString)), &myQMLProxyClass, SLOT(cppSlot(QString)));

    // TODO: Нужно вызывать и disconnect, когда сообщения больше не нужны

    // Передаём в QML-код массив строк
    sendStringListToQML(wholeWindow);

    // Запускаем основной цикл обработки сообщений
    return app.exec();
}

// Метод динамически создаёт QML-объект из QML-шаблона
static void createQMLDynamically(QQmlApplicationEngine& engine)
{
    QQuickWindow *window = qobject_cast<QQuickWindow*>(engine.rootObjects().at(0));

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

static void sendStringListToQML(QObject *pWindow)
{
    // В список можно поместить объекты типов, известных классу QVariantList
    QVariantList list;
    list << 10 << QColor(Qt::green) << "bottles";

    QMetaObject::invokeMethod(pWindow, "logReceivedList",
        Q_ARG(QVariant, QVariant::fromValue(list)));

    // Создаём список из структур
    QVariantList complexList;

    SharedStruct firstClass;
    firstClass.m_val = 15;
    firstClass.m_name1 = "Token";
    firstClass.m_name2 = "Bottox";

    SharedStruct secondClass;
    secondClass.m_val = 18;
    secondClass.m_name1 = "Biber";
    secondClass.m_name2 = "Asterix";

    complexList << QVariant::fromValue(firstClass);
    complexList << QVariant::fromValue(secondClass);

    QMetaObject::invokeMethod(pWindow, "logReceivedComplexList",
        Q_ARG(QVariant, QVariant::fromValue(complexList)));
}
