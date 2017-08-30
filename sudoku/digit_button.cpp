#include "digit_button.h"

DigitButton::DigitButton(QWidget *parent)
	: QPushButton(parent)
{

}

void DigitButton::mouseReleaseEvent(QMouseEvent *e)
{
	if(e->button() == Qt::LeftButton)
	{
		emit clicked();
	} else if(e->button() == Qt::RightButton) {
		emit right_clicked();
	}

	QPushButton::mouseReleaseEvent(e);
}
