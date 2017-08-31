#include "tool_button.h"
#include "config.h"

ToolButton::ToolButton(QWidget *parent)
	: QPushButton(parent)
{
	is_mouse_over = false;
	is_mouse_pressed = false;
	setFocusPolicy(Qt::NoFocus);
	setFixedSize(TOOL_FIXED_SIZE, TOOL_FIXED_SIZE);
	setContentsMargins(3, 0, 3, 0);
	setAttribute(Qt::WA_Hover, true);
}

void ToolButton::set_image(QString path)
{
	image_path = path;
	update_style();
}

void ToolButton::update_style()
{
	update();
}

void ToolButton::paintEvent(QPaintEvent *)
{
	QRect rect = this->rect();

	QPainter p(this);

	QPixmap bg(image_path);
	bg = bg.scaled(rect.width(), rect.height(),
				   Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	if(!isEnabled())
	{
		QImage img = bg.toImage();
		p.drawImage(0, 0, img);
	} else {
		p.drawPixmap(QPoint(0, 0), bg);
		if(is_mouse_pressed)
		{
			p.setCompositionMode(QPainter::CompositionMode_Multiply);
			p.drawPixmap(QPoint(0, 0), bg);
		} else if(is_mouse_over) {
			p.setCompositionMode(QPainter::CompositionMode_HardLight);
			p.drawPixmap(QPoint(0, 0), bg);
		}
	}
}

void ToolButton::mousePressEvent(QMouseEvent *ev)
{
	if(ev->button() == Qt::LeftButton)
		is_mouse_pressed = true;
	QPushButton::mousePressEvent(ev);
}

void ToolButton::mouseReleaseEvent(QMouseEvent *ev)
{
	if(ev->button() == Qt::LeftButton)
		is_mouse_pressed = false;
	QPushButton::mouseReleaseEvent(ev);
}

void ToolButton::enterEvent(QEvent *ev)
{
	is_mouse_over = true;
	QPushButton::enterEvent(ev);
}

void ToolButton::leaveEvent(QEvent *ev)
{
	is_mouse_over = false;
	QPushButton::leaveEvent(ev);
}
