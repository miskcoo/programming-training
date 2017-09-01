#ifndef TIMER_H
#define TIMER_H

#include <QtWidgets>
#include <QTimer>

class Timer : public QLabel
{
	Q_OBJECT
public:
	explicit Timer(QWidget *parent = 0);

	void stop_timer();
	QString get_time() const;

signals:

public slots:
	void restart_timer();
	void toggle_timer();
	void timeout();

private:
	void set_time(QString);

private:
	QTimer *timer;
	int clock_now;
};

#endif // TIMER_H
