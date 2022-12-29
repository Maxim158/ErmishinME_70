#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include "dictionary.h"
#include "addinlist.h"
#include <QTreeView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString file_Name = "";
    QString address = "";
    QMap<QString, Dictionary> dict;
    void AddRoot(QString name, QString data, QString phone);
    void Open();
    void Save();
    void SaveAs();

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    static MainWindow* instance();
    QSortFilterProxyModel* filter;

    void setSourceModel(QAbstractItemModel *model);

private slots:

    void on_Add_clicked();

    void on_Remove_clicked();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_filterPattern_textChanged(const QString &arg1);

    void on_senseSorting_stateChanged();

    void on_columnBox_currentIndexChanged(int index);

    void on_senseFilter_stateChanged();

    void on_New_file_triggered();

    void on_itemChanged(QStandardItem *item);

private:
    Ui::MainWindow *ui;
    AddInList* mpclsFloating;
    QTreeView *sourceView;
};
#endif // MAINWINDOW_H
