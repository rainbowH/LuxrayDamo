#ifndef DLGMANAGESEARCHSTDWHEEL_H
#define DLGMANAGESEARCHSTDWHEEL_H

#include <QDialog>
#include <QToolButton>

namespace Ui {
class DlgManageSearchStdWheel;
}

class DlgManageSearchStdWheel : public QDialog
{
    Q_OBJECT

public:
    explicit DlgManageSearchStdWheel(QWidget *parent = 0);
    ~DlgManageSearchStdWheel();

    QList<QToolButton*> btnList;

public slots:
    void onExport();
    void onDelete();



private:
    Ui::DlgManageSearchStdWheel *ui;

    void addBtnExport();
    void addBtnDelete();
    const QString btnQSS;
};

#endif // DLGMANAGESEARCHSTDWHEEL_H
