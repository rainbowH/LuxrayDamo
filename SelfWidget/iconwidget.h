#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include <QObject>
#include <QWidget>

class IconWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IconWidget(QWidget *parent = nullptr);

    QString picName;

    void paint(QString pictureName);
protected:
    void paintEvent(QPaintEvent *event);
signals:

public slots:

};

#endif // ICONWIDGET_H
