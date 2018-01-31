#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QObject>
#include <QWidget>
#include <QToolButton>
#include <QMouseEvent>

class ToolButton : public QToolButton
{
public:
    ToolButton(QWidget *parent = 0);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:

};

#endif // TOOLBUTTON_H
