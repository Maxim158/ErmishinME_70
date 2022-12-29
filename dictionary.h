#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QString>

class Dictionary
{
private:

    QString name;
    QString data;
    QString phone;

public:
    Dictionary();
    Dictionary(QString name, QString data, QString phone);
    QString getData() const;
    QString getName() const;
    QString getPhone() const;
    void setData(QString data);
    void setName(QString name);
    void setPhone(QString phone);
    QString toString() {
        return (name + " " + data + " " + phone);
    }
};

#endif // DICTIONARY_H
