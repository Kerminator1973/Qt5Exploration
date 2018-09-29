#ifndef SHAREDCLASS_H
#define SHAREDCLASS_H

#include <QObject>
#include <QDebug>
using namespace std;

struct SharedStruct {
    Q_GADGET    // Поскольку SharedStruct не наследуется от QObject, необходимо использовать макрос Q_GADGET

public:
    int m_val;
    QString m_name1;
    QString m_name2;

private:
    Q_PROPERTY(int val MEMBER m_val)
    Q_PROPERTY(QString name1 MEMBER m_name1)
    Q_PROPERTY(QString name2 MEMBER m_name2)
};

class SharedClass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SharedStruct sharedStruct READ getSharedStruct
        WRITE setSharedStruct NOTIFY sharedStructChanged)

public:
    SharedStruct strObj;

public:
    explicit SharedClass(QObject *parent = nullptr) {}

    // Edit: changed get function
    SharedStruct getSharedStruct() const
    {
        return strObj;
    }

    // Edit: Added set function
    void setSharedStruct(SharedStruct val)
    {
        strObj = val;
        emit sharedStructChanged();
    }

signals:
    void sharedStructChanged();
};

Q_DECLARE_METATYPE(SharedStruct)

#endif // SHAREDCLASS_H
