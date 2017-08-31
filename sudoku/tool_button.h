#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QtWidgets>

class ToolButton : public QPushButton
{
	Q_OBJECT
public:
	ToolButton(QWidget* parent = 0);

	void set_image(QString);
signals:

public slots:

protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);

private:
	void update_style();

private:
	bool is_mouse_pressed, is_mouse_over;
	QString image_path;
};

#endif // TOOLBUTTON_H
