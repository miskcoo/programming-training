#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QtWidgets>

class ToolButton : public QPushButton
{
	Q_OBJECT
public:
	using QPushButton::QPushButton;

	void set_image(QString);
signals:

public slots:

protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

private:
	void update_style();

private:
	bool is_mouse_pressed;
	QString image_path;
};

#endif // TOOLBUTTON_H
