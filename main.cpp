#include "mainwindow.h"

#include <QApplication>
#include <QAction>
#include <QDate>
#include <QDateTime>

QStandardItemModel *CreateModel(QObject *parent)
{
    QStandardItemModel *model = new QStandardItemModel(0, 3, parent);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Full Name"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Passport data"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Phone"));

    return model;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = MainWindow::instance();
    auto model = CreateModel(w);
    w->setSourceModel(model);
    QObject::connect(model, SIGNAL(itemChanged(QStandardItem*)), w, SLOT(on_itemChanged(QStandardItem*)));
    w->show();
    return a.exec();
}
