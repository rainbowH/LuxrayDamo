#include "dlgusersearch.h"
#include "ui_dlgusersearch.h"

DlgUserSearch::DlgUserSearch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgUserSearch)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | Qt::WindowCloseButtonHint);

}

DlgUserSearch::~DlgUserSearch()
{
    delete ui;
}
