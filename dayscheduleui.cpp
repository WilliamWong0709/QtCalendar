#include "dayscheduleui.h"
#include <QDebug>

DayScheduleUi::DayScheduleUi(QLabel *tl, QLabel *al, QPushButton *mb, QPushButton *db)
	: mTimeLbl(tl), mActivityLbl(al), mModifyBtn(mb), mDeleteBtn(db)
{

}

DayScheduleUi::~DayScheduleUi()
{
	delete mTimeLbl;
	delete mActivityLbl;
	delete mModifyBtn;
	delete mDeleteBtn;
}
