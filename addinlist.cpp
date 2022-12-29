#include "addinlist.h"
#include "ui_addinlist.h"
#include "mainwindow.h"
#include "dictionary.h"
#include <QMessageBox>
#include <string>

AddInList::AddInList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddInList)
{
    ui->setupUi(this);
}

AddInList::~AddInList()
{
    delete ui;
}

void AddInList::on_acceptBox_accepted()
{
    if (ui->nameLine->text() != "" && ui->dataLine->text() != "" && ui->phoneLine->text() != "" && ui->dataLine->text().toInt() &&  ui->phoneLine->text().toInt()) {
        QMap<QString, Dictionary> &dict = MainWindow::instance()->dict;
        bool exist = false;
        for (Dictionary element : dict) {
            if (ui->phoneLine->text() == element.getPhone() ) exist = true;
        }
        if (!exist) {
        dict.insert(ui->phoneLine->text(), Dictionary(ui->nameLine->text(), ui->dataLine->text(), ui->phoneLine->text()));
        MainWindow::instance()->AddRoot(ui->nameLine->text(), ui->dataLine->text(), ui->phoneLine->text());
        }
        else {
            QMessageBox msgBox;
            msgBox.setText("Person already in dictionary");
            msgBox.setWindowTitle("Error");
            msgBox.exec();
        }
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("Incorrect Input");
        msgBox.setWindowTitle("Error");
        msgBox.exec();
    }

    ui->nameLine->setText("");
    ui->dataLine->setText("");
    ui->phoneLine->setText("");
}

void AddInList::on_acceptBox_rejected()
{
    ui->nameLine->setText("");
    ui->dataLine->setText("");
    ui->phoneLine->setText("");
}
