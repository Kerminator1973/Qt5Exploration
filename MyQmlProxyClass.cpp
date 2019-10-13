#include <QNetworkReply>
#include "myhttpservice.h"
#include "myqmlproxyclass.h"


void MyQmlProxyClass::cppOnButtonClicked() {
    qDebug() << "Called the C++ slot without message";

    auto manager = CMyHttpService::getManager();

    connect(manager, &QNetworkAccessManager::finished, this, &MyQmlProxyClass::replyFinished);

    // Выполняем GET-запрос
    QNetworkRequest request;
    request.setUrl(QUrl("https://restcountries.eu/rest/v2/all"));

    // В простейшем случае можно не использовать SSL, но такое решение не подходит
    // для промышленных приложений
    //request.setUrl(QUrl("http://flagpedia.net/"));

    // Для успешного подключения с использованием SSL/TLS необходимо
    // сконфигурировать подключение, в том числе, настроить сертификаты.
    // Также необходимо добавить динамические библиотеки из состава OpenSSL.
    // Их можно собрать самостоятельно из исходных текстов, либо загрузить
    // из доверенного источника (я использовал - https://indy.fulgan.com/SSL/).
    // Необходимо учитывать разрядность библиотек (x64, x32). Названия требуемых
    // библиотек для Windows: libeay32.dll, ssleay32.dll
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());

    QNetworkReply *networkReply = manager->get(request);

    // Добавляем ещё несколько обработчиков событий, но уже связанных
    // с QNetworkReply, а не с QNetworkAccessManager
    /*
    connect(networkReply, &QNetworkReply::error, this, &MyClass::networkReplyError);
    connect(networkReply, &QNetworkReply::QOverload<QNetworkReply::NetworkError>::of(&QNetworkReplu::error, this, &MyClass::networkReplyError);
    connect(networkReply, &QNetworkReply::finished, this, &MyClass::requestFinised);
    connect(networkReply, &QNetworkReply::readyRead, this, &MyClass::readyRead);
    */
}

void MyQmlProxyClass::replyFinished(QNetworkReply *reply) {

    if(reply->error()) {

        qDebug() << "The error happened during the request";

    } else {

        qDebug() << "The data was received";
        //...reply->rawHeaderList();
    }

    // Указывем на необходимость последующего удаления памяти,
    // зарезервированной для хранения http-запросов
    reply->deleteLater();
}
