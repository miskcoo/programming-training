#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login_dialog.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	rw_socket = nullptr;
	listen_socket = nullptr;

	initWidgets();

	connect(ui->actionStart, SIGNAL(triggered(bool)), this, SLOT(startListening()));
	connect(ui->actionConnect, SIGNAL(triggered(bool)), this, SLOT(connectToServer()));
}

void MainWindow::initWidgets()
{
	window = new QWidget;
	setCentralWidget(window);

	QVBoxLayout *layout = new QVBoxLayout(window);

	chess_board = new ChessBoard;
	layout->addWidget(chess_board);

	info_label = new QLabel;
	info_label->setMargin(0);
	ui->statusBar->addWidget(info_label);
	ui->statusBar->setSizeGripEnabled(false);

	connect(chess_board, SIGNAL(noAvailChess()), this, SLOT(gameEnd()));
	connect(chess_board, SIGNAL(playerMove(vector<pair<int,int> >)),
			this, SLOT(playerMove(vector<pair<int,int> >)));
}

void MainWindow::connectToServer()
{
	LoginDialog dlg;
	if(dlg.exec() == QDialog::Accepted)
	{
		QHostAddress host_addr;
		if(host_addr.setAddress(dlg.getAddress()))
		{
			rw_socket = new QTcpSocket;
			rw_socket->connectToHost(host_addr, dlg.getPort());
			info_label->setText("Connecting...");
			connect(rw_socket, SIGNAL(connected()), this, SLOT(connectedToHost()));
			connect(rw_socket, SIGNAL(readyRead()), this, SLOT(recvMessage()));
		} else {
			QMessageBox::warning(this, "Error", "Error: Invalid address!");
		}
	}
}

void MainWindow::playerMove(vector<pair<int, int>> move_trace)
{
	QString text = QString("%1 %2")
			.arg(OPER_MOVE,
				 QString::number(move_trace.size()));
	for(auto pos : move_trace)
		text += QString(" %1 %2")
				.arg(QString::number(pos.first),
					 QString::number(pos.second));
	rw_socket->write(text.toUtf8());
//	qDebug() << text;
}

void MainWindow::recvMessage()
{
	QTextStream os(rw_socket);

	QString oper;
	os >> oper;
	if(oper == OPER_MOVE)
	{
		int trace_len;
		vector<pair<int, int>> move_trace;
		os >> trace_len;
		for(int i = 0; i != trace_len; ++i)
		{
			int x, y;
			os >> x >> y;
			move_trace.push_back({x, y});
		}

		chess_board->moveChess(move_trace);
	} else if(oper == OPER_GIVEUP) {
		QMessageBox::information(this, "Game Over", "Congratulations! You win!");
		chess_board->clearMarks();
	}
}

void MainWindow::connectedToHost()
{
	info_label->setText("Connected to " + rw_socket->peerAddress().toString()
						+ ", port " + QString::number(rw_socket->peerPort()));

	chess_board->startGame(DraughtsInfo::white);
}

void MainWindow::acceptConnection()
{
	rw_socket = listen_socket->nextPendingConnection();
	connect(rw_socket, SIGNAL(readyRead()), this, SLOT(recvMessage()));

	info_label->setText("Connected with " + rw_socket->peerAddress().toString()
						+ ", port " + QString::number(rw_socket->peerPort()));

	chess_board->startGame(DraughtsInfo::black);
}

void MainWindow::startListening()
{
	listen_socket = new QTcpServer;
	if(listen_socket->listen())
	{
		connect(listen_socket, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

		info_label->setText("Listening on " + listen_socket->serverAddress().toString()
							+ ", port " + QString::number(listen_socket->serverPort()));
	} else {
		QMessageBox::warning(this, "Error", "Error: " + listen_socket->errorString());
		delete listen_socket;
	}
}

void MainWindow::gameEnd()
{
	if(chess_board->getCurrentPlayer() == chess_board->getPlayer())
	{
		// lost
		QMessageBox::information(this, "Game Over", "You lost!");
	} else {
		// win
		QMessageBox::information(this, "Game Over", "Congratulations! You win!");
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionGive_Up_triggered()
{
   if(rw_socket)
   {
	   rw_socket->write(OPER_GIVEUP);
	   QMessageBox::information(this, "Game Over", "You lost!");
   }
}
