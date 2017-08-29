#include "timer.h"

Timer::Timer(QWidget *parent)
	: QLabel(parent),
	  clock_now(0)
{
	timer = new QTimer(this);
	timer->setInterval(1000);

	connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));

	setText("00:00:00");
}

void Timer::restart_timer()
{
	setText("00:00:00");
	clock_now = 0;
	timer->stop();
	timer->start();
}

void Timer::toggle_timer()
{
	if(timer->isActive())
	{
		timer->stop();
	} else {
		timer->start();
	}
}

void Timer::timeout()
{
	QString time = QDateTime::fromTime_t(++clock_now).toUTC().toString("hh:mm:ss");
	setText(time);
}
