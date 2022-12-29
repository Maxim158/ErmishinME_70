#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "dictionary.h"
#include <QTime>
#include <QTimeEdit>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Dictionary"));
    mpclsFloating = new AddInList(this);
    filter = new QSortFilterProxyModel;
    sourceView = new QTreeView;
    ui->Model->setModel(filter);
    ui->Model->setSortingEnabled(true);
    ui->Model->setAlternatingRowColors(true);
    ui->Model->setRootIsDecorated(false);
    ui->Model->setExpandsOnDoubleClick(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow* MainWindow::instance()
{
    static MainWindow *instance = new MainWindow();
    return instance;
}

void MainWindow::setSourceModel(QAbstractItemModel *model)
{
    filter->setSourceModel(model);
    sourceView->setModel(model);
}

void MainWindow::AddRoot(QString name, QString data, QString phone){
    QStandardItemModel *model = static_cast<QStandardItemModel*>(sourceView->model());
    model->appendRow({new QStandardItem(name), new QStandardItem(data), new QStandardItem(phone)});
}

void MainWindow::Open() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Address Book"), "", tr("Address Book (*.txt);;All Files (*)"));
    if (fileName.isEmpty())
            return;
        else {
            QString name;
            QString data;
            QString phone;
            QFile file(fileName);

            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;
            }
            dict.clear();
            QAbstractItemModel *model = sourceView->model();
            if (model->hasChildren()) {
                model->removeRows(0, model->rowCount());
            }

            QTextStream stream(&file);
            while (!stream.atEnd()) {
                QString line = stream.readLine();
                name = line.left(line.indexOf(" "));
                line.remove(0, line.indexOf(" ")+1);
                data = line.left(line.indexOf(" "));
                line.remove(0, line.indexOf(" ")+1);
                phone = line.left(line.indexOf(" "));
                dict.insert(phone, Dictionary(name, data, phone));
                AddRoot(name, data, phone);
            }
            file_Name = fileName;
            file.close();
    }
}

void MainWindow::Save() {
   QFile file(file_Name);
   if (!file.open(QIODevice::WriteOnly)) {
       QMessageBox::information(this, tr("Unable to open file"),
           file.errorString());
       return;
   }
   QTextStream out(&file);
   foreach (Dictionary value, dict) {
       out << value.toString() << "\r\n";
   }
   file.close();
}

void MainWindow::SaveAs() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Address Book"), "", tr("Address Book (*.txt);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }
        QTextStream out(&file);
        foreach (Dictionary value, dict) {
            out << value.toString() << "\r\n";
        }
        file_Name = fileName;
        file.close();
    }
}

void MainWindow::on_Add_clicked()
{
    mpclsFloating->setParent(nullptr);
    mpclsFloating->show();
}

void MainWindow::on_Remove_clicked()
{
    if (dict.size() > 0 && ui->Model->selectionModel()->hasSelection()) {
        QModelIndexList list = ui->Model->selectionModel()->selectedIndexes();
        QAbstractItemModel *model = ui->Model->model();
            dict.remove(model->data(list[2]).toString());
        model->removeRow(list[0].row(), list[0].parent());
    }
}

void MainWindow::on_actionOpen_triggered()
{
    this->Open();
}


void MainWindow::on_actionSave_triggered()
{
    if (file_Name != "") {
        this->Save();
    }
    else {
        this->SaveAs();
    }
}


void MainWindow::on_actionSave_As_triggered()
{
    this->SaveAs();
}

void MainWindow::on_filterPattern_textChanged(const QString &arg1)
{
    QRegExp::PatternSyntax syntax = QRegExp::PatternSyntax(ui->syntaxBox->itemData(ui->syntaxBox->currentIndex()).toInt());
    Qt::CaseSensitivity caseSensitivity =ui->senseFilter->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;

    QRegExp regExp(arg1, caseSensitivity, syntax);
    filter->setFilterRegExp(regExp);
    ui->Model->setModel(filter);
}



void MainWindow::on_senseSorting_stateChanged()
{
    filter->setSortCaseSensitivity(ui->senseSorting->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
}


void MainWindow::on_columnBox_currentIndexChanged(int index)
{
    filter->setFilterKeyColumn(index);
}


void MainWindow::on_senseFilter_stateChanged()
{
    QRegExp::PatternSyntax syntax = QRegExp::PatternSyntax(ui->syntaxBox->itemData(ui->syntaxBox->currentIndex()).toInt());
    Qt::CaseSensitivity caseSensitivity =ui->senseFilter->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;

    QRegExp regExp(ui->filterPattern->text(), caseSensitivity, syntax);
    filter->setFilterRegExp(regExp);
    ui->Model->setModel(filter);
}

void MainWindow::on_New_file_triggered()
{
    dict.clear();
    QModelIndex index = ui->Model->selectionModel()->currentIndex();
    QAbstractItemModel *model = ui->Model->model();
    if (model->rowCount(index.parent()) != 0) model->removeRows(0, model->rowCount(index.parent()) , index.parent());
    file_Name = "";
}


void MainWindow::on_itemChanged(QStandardItem *item)
{
    QAbstractItemModel *model = ui->Model->model();
    QString element =  model->data(model->index(item->row(), 2)).toString();

    switch (item->column()) {
        case 0:
        {
            auto new_value = item->text();
            auto old_value = dict.value(element).getName();
            if (old_value.compare(new_value) == 0) return;
            if (new_value.size() == 0)
            {
                QMessageBox msgBox;
                msgBox.setText("Incorrect Name");
                msgBox.setWindowTitle("Error");
                msgBox.exec();
                item->setText(old_value);
                return;
            }
            dict[element].setName(new_value);
        } break;
        case 1:
        {
            auto new_value = item->text();
            auto old_value = dict.value(element).getData();
            if (old_value.compare(new_value) == 0) return;
            bool ok;
            new_value.toInt(&ok);
            if (!ok)
            {

                QMessageBox msgBox;
                msgBox.setText("Incorrect Passport Data");
                msgBox.setWindowTitle("Error");
                msgBox.exec();
                item->setText(old_value);
                return;
            }
            dict[element].setData(new_value);
        } break;
        case 2:
        {
            auto new_value = item->text();
            auto old_value = dict.value(element).getPhone();
            if (old_value.compare(new_value) == 0) return;
            bool ok;
            new_value.toInt(&ok);
            if (!ok || dict.contains(new_value))
            {
                QMessageBox msgBox;
                msgBox.setText("Incorrect Phone");
                msgBox.setWindowTitle("Error");
                msgBox.exec();
                item->setText(old_value);
                return;
            }
            dict[element].setPhone(new_value);
        } break;
    }
}
