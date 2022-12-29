#include "dictionary.h"

Dictionary::Dictionary()
{
}

Dictionary::Dictionary(QString name, QString data, QString phone) {
    this->name = name;
    this->data = data;
    this->phone = phone;
}

QString Dictionary::getData() const {
    return data;
}

QString Dictionary::getName() const {
    return name;
}

QString Dictionary::getPhone()  const {
    return phone;
}

void Dictionary::setData(QString data) {
    this->data = data;
}

void Dictionary::setName(QString name) {
    this->name = name;
}

void Dictionary::setPhone(QString phone) {
    this->phone = phone;
}
