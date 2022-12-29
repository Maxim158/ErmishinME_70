#ifndef ADDINLIST_H
#define ADDINLIST_H

#include <QDialog>

namespace Ui {
class AddInList;
}

class AddInList : public QDialog
{
    Q_OBJECT

public:
    explicit AddInList(QWidget *parent = nullptr);
    ~AddInList();

private slots:
    void on_acceptBox_accepted();

    void on_acceptBox_rejected();

private:
    Ui::AddInList *ui;
};

#endif // ADDINLIST_H
