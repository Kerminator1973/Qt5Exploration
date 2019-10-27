# Достоинства/недостатки Qt 5 для мобильных приложений

Сильными сторонами Qt 5, в части разработки приложений для мобильных устройств являются:
1. Мощный язык описания композиции элементов пользовательского интерфейса **QML**. Большое число анимационных эффектов позволяют разрабатывать более насыщенный и привлекательный пользовательский интерфейс, чем у типовых Android/iOS приложений, разработанных с использованием Platform SDK
2. Использование C++ позволяет реализовывать алгоритмы с высокой вычислительной сложностью, потребляющие меньшее количество энергии, чем приложения, разработанные на managed языках (Java, Swift). Следует учитывать, что в приложениях на Platform SDK может быть использован Native-код (С++)
3. Один и тот же код может быть использован как для сборки приложения под Android, так и под iOS
4. Можно использовать уже существующие библиотеки на C++, что может быть критичным, например, в случае применения криптографии

Недостатки Qt 5 при разработке мобильных приложений:
1. Технологический stack гораздо более сложный. Часть кода разрабатывается на Platform SDK (Java/Kotlin/Swift), а часть на C++, QML и JavaScript. Настройка инструментальных средств сложнее, чем в случае использования Xcode, или Android Studio
2. Разные целевые платформы поддерживают разные подмножества Qt API. Например, Qt Multimedia Widgets не поддерживаются на Android, а под iOS нельзя получить информацию о видимых спутниках GPS
3. Необходимо совмещать в apk-файле код для разных аппаратных платформ, либо создавать разные packages для разных аппаратных платформ (ARM и x86). Проблема актуальна только для Android
4. Добавление Qt-библиотек ощутимо увеличивает размер apk-файлов
5. При необходимости использования Platform Specific-функций, например, для доступа к хранилищу изображений, необходимо разрабатывать интеграционный слой с использованием JNI и наследованием QML Activity в Java-коде, Java-классом. Это существенно повышает сложность приложения.

# Qt5Exploration 

В данном репозитарии хранится приложение, в котором решаются *пограничные* задачи, такие как взаимодействие C++ и JavaScript-кода (QML) в приложении на Qt5.

**Задача**: реализовать обмен сообщениями через **Signals and Slots** кода на **C++** и **QML**.

**Подзадачи**:
1. Обеспечить выполнение connect() внутри компонента QML через свойство
2. Обеспечить обмен сигналами через код на C++
3. Обеспечить передачу данных из кода на C++ в список в QML, посредством QAbstractListModel
4. Выполнить https-запрос и разобрать полученный JSON-ответ

# Выполнение http/https-запросов

Ключевым классом, обеспечивающим возможность выполнения http(s)-запросов является **QNetworkAccessManager**. Через экземпляр этого класса можно запускать запрос на выполнение, например, так:

```cpp
QNetworkReply *networkReply = manager->get(request);
```

Для того, чтобы выполнять подключение по **SSL/TLS** необходимо указать SSL-конфигурацию, в которой можно настроить сертификаты:

```cpp
QNetworkRequest request;

request.setUrl(QUrl("https://restcountries.eu/rest/v2/all"));
request.setSslConfiguration(QSslConfiguration::defaultConfiguration());
```

Необходимо добавить динамические библиотеки из состава [OpenSSL](https://www.openssl.org/). Их можно собрать самостоятельно из исходных текстов, либо загрузить из доверенного источника (я использовал - https://indy.fulgan.com/SSL/). Необходимо учитывать разрядность библиотек (**x64, x32**). Названия требуемых библиотек: **libeay32.dll** и **ssleay32.dll**
