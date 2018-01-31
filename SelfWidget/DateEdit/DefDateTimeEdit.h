#ifndef DEFDATETIMEEDIT_H
#define DEFDATETIMEEDIT_H

#include <QDateTimeEdit>
#include <QDateEdit>
#include "DefineCalendar.h"

class DefDateTimeEdit : public QDateEdit
{
	Q_OBJECT

public:
	DefDateTimeEdit(QWidget *parent);
	~DefDateTimeEdit();
	void setMyStytle();

protected slots:
    void getDate(const QDate &date);

private:
	DefineCalendar *m_DefCalendar;
	
};

#endif // DEFDATETIMEEDIT_H
