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
	void socketError(QTcpSocket::SocketError);
	void recvMessage();
	void playerMove(int, int, int, int);
	void gameEnd();

private:
	Ui::MainWindow *ui;
	QWidget *window;
	QLabel *info_label;
	ChessBoard *chess_board;

	QTcpServer *listen_socket;
	QTcpSocket *rw_socket;
};

#endif // MAINWINDOW_H
