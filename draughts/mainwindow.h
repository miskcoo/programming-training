#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QtWidgets>
#include "chessboard.h"
#include "config.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void initWidgets();

public slots:
	void connectToServer();
	void startListening();

private slots:
	void connectedToHost();
	void acceptConnection();
	void recvMessage();
	void playerMove(vector<pair<int, int>>);
	void gameEnd();
	void peaceEnd();

	void on_actionGive_Up_triggered();

	void on_actionMake_Peace_triggered();

private:
	Ui::MainWindow *ui;
	QWidget *window;
	QLabel *info_label;
	ChessBoard *chess_board;

	// the following three variables are used for user-defined game
	bool is_self_defined_game;
	QString self_defined_game;
	bool is_black_first;

	QTcpServer *listen_socket;
	QTcpSocket *rw_socket;
};

#endif // MAINWINDOW_H
