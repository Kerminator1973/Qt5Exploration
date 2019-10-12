#ifndef MYQMLPROXYCLASS_H
#define MYQMLPROXYCLASS_H

#include <QGuiApplication>
#include <QDebug>   // Заголовочный файл нужен для обеспечения возможности использовать qDebug()

// Proxy-класс, через который обмениваются информацией
// два компонента пользовательского интерфейса (QML)
class MyQmlProxyClass : public QObject
{
    Q_OBJECT
public slots:
    void cppSlot(const QString &msg) {
        qDebug() << "Called the C++ slot with message:" << msg;
    }

    void cppOnButtonClicked() {
        qDebug() << "Called the C++ slot without message";
    }
};

#endif // MYQMLPROXYCLASS_H
