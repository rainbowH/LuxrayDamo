#include "toolbutton.h"

ToolButton::ToolButton(QWidget *parent)
    :QToolButton(parent)
{
    this->setStyleSheet("background: transparent;");
    this->setIconSize(this->size());
}

void ToolButton::mousePressEvent(QMouseEvent *event)
{
    this->setToolButtonStyle(Qt::ToolButtonIconOnly);
}

void ToolButton::mouseReleaseEvent(QMouseEvent *event)
{
    this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}
