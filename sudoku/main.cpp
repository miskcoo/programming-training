#include "mainwindow.h"
#include <ctime>
#include <cstdlib>
#include <QApplication>

int main(int argc, char *argv[])
{
	std::srand(std::time(0));
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
