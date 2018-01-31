#ifndef DLGPATHSETTING_H
#define DLGPATHSETTING_H

#include <QDialog>

namespace Ui {
class DlgPathSetting;
}

class DlgPathSetting : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPathSetting(QWidget *parent = 0);
    ~DlgPathSetting();
    void setPath(QString path);
    QString getPath();

private slots:
    void on_btnSelect_clicked();

private:
    Ui::DlgPathSetting *ui;
    QString path;
};

#endif // DLGPATHSETTING_H
