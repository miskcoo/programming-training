#include <QFontMetrics>
#include "utils.h"

QFont fit_font_with_text(QFont font, QString text, QRect rect_lbl)
{
	/* This function is modified from
	 * https://stackoverflow.com/questions/42652738/how-to-automatically-increase-decrease-text-size-in-label-in-qt
	 */

	// get initial settings
	if(text.length() == 0)
		return font;

	int size = font.pointSize();
	QFontMetrics fm(font);
	QRect rect = fm.boundingRect(rect_lbl, Qt::TextWordWrap, text);

	// decide whether to increase or decrease
	int step = rect.height() > rect_lbl.height() ? -1 : 1;

	// iterate until text fits best into rectangle of label
	for (;;)
	{
		font.setPointSize(size + step);
		QFontMetrics fm(font);
		rect = fm.boundingRect(rect_lbl, Qt::TextWordWrap, text);
		if (size <= 1)
		{
			// Font cannot be made smaller
			break;
		}
		if (step < 0)
		{
			size += step;
			if (rect.height() < rect_lbl.height()) break;
		} else {
			if (rect.height() > rect_lbl.height()) break;
			size += step;
		}
	}

	// apply result of iteration
	font.setPointSize(size);
	return font;
}
