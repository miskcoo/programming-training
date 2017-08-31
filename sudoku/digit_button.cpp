#include "digit_button.h"
#include "utils.h"

DigitButton::DigitButton(QWidget *parent)
	: QPushButton(parent)
{
	setFocusPolicy(Qt::NoFocus);
}

void DigitButton::mouseReleaseEvent(QMouseEvent *e)
{
	// e->button() == Qt::LeftButton is included in `else`
	if(e->button() == Qt::RightButton) {
		emit right_clicked();
	} else QPushButton::mouseReleaseEvent(e);
}

void DigitButton::update_font()
{
	QString text = this->text();
	QRect rect_lbl = geometry().adjusted(5, 5, -5, -5);
	QFont font = this->font();

	font = fit_font_with_text(font, text, rect_lbl);

	setFont(font);
}

void DigitButton::update_style()
{
	if(is_checked())
	{
		setStyleSheet("font-weight: 900; font-style: italic;");
	} else {
		setStyleSheet("font-weight: normal; font-style: normal;");
	}
}

void DigitButton::toggle()
{
	set_checked(!checked);
}

bool DigitButton::is_checked() const
{
	return checked;
}

void DigitButton::set_checked(bool checked)
{
	this->checked = checked;
	update_style();
}
