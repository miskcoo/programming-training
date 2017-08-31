#ifndef DIGITBUTTON_H
#define DIGITBUTTON_H

#include <QtWidgets>

class DigitButton : public QPushButton
{
	Q_OBJECT
public:
	explicit DigitButton(QWidget *parent = 0);

	void update_font();
	void toggle();
	bool is_checked() const;
	void set_checked(bool);

signals:
	void right_clicked();

public slots:

private:
	void update_style();

protected:
	void mouseReleaseEvent(QMouseEvent *);

private:
	bool checked;
};

#endif // DIGITBUTTON_H
