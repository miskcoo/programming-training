#include "digit_button.h"
#include "utils.h"

DigitButton::DigitButton(QWidget *parent)
	: QPushButton(parent)
{
	setFocusPolicy(Qt::NoFocus);
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

void DigitButton::update_font()
{
	QString text = this->text();
	QRect rect_lbl = geometry().adjusted(5, 5, -5, -5);
	QFont font = this->font();

	font = fit_font_with_text(font, text, rect_lbl);

	setFont(font);
}
