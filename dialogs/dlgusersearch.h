#ifndef DLGUSERSEARCH_H
#define DLGUSERSEARCH_H

#include <QDialog>

namespace Ui {
class DlgUserSearch;
}

class DlgUserSearch : public QDialog
{
    Q_OBJECT

public:
    explicit DlgUserSearch(QWidget *parent = 0);
    ~DlgUserSearch();

private:
    Ui::DlgUserSearch *ui;
};

#endif // DLGUSERSEARCH_H
