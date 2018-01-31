#ifndef DLGMIDEDGEPARADATAS_H
#define DLGMIDEDGEPARADATAS_H

#include <QDialog>

namespace Ui {
class DlgMidEdgeParaDatas;
}

class DlgMidEdgeParaDatas : public QDialog
{
    Q_OBJECT

public:
    explicit DlgMidEdgeParaDatas(QWidget *parent = 0);
    ~DlgMidEdgeParaDatas();

private:
    Ui::DlgMidEdgeParaDatas *ui;
};

#endif // DLGMIDEDGEPARADATAS_H
