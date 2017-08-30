#ifndef DIGITBUTTON_H
#define DIGITBUTTON_H

#include <QtWidgets>

class DigitButton : public QPushButton
{
	Q_OBJECT
public:
	explicit DigitButton(QWidget *parent = 0);

signals:
	void right_clicked();

public slots:

protected:
	void mouseReleaseEvent(QMouseEvent *);
};

#endif // DIGITBUTTON_H
