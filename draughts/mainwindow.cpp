#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login_dialog.h"
#include "listen_dialog.h"

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
	} else if(oper == OPER_START) {
		chess_board->startGame(DraughtsInfo::white);
	} else if(oper == OPER_SETGAME) {
		QString first_player, game;
		os >> first_player >> game;
		Draughts draughts(game.toStdString().c_str());
		chess_board->startGame(DraughtsInfo::white,
							   first_player == "W" ? DraughtsInfo::white : DraughtsInfo::black,
							   &draughts);
	} else if(oper == OPER_MAKEPEACE) {
		if(QMessageBox::question(this, "Make Peace?",
								 "Your anamy wants to make peace, do you agree?",
								 "No", "Yes") != QMessageBox::NoButton)
		{
			rw_socket->write(OPER_CONFIRMPEACE);
			peaceEnd();
		} else rw_socket->write(OPER_DENIEDPEACE);
	} else if(oper == OPER_CONFIRMPEACE) {
		peaceEnd();
	} else if(oper == OPER_DENIEDPEACE) {
		QMessageBox::information(this, "Make Peace", "Your anamy disagrees to make peace with you!");
	}
}

void MainWindow::connectedToHost()
{
	info_label->setText("Connected to " + rw_socket->peerAddress().toString()
						+ ", port " + QString::number(rw_socket->peerPort()));
	// game starts when received OPER_START message
}

void MainWindow::acceptConnection()
{
	rw_socket = listen_socket->nextPendingConnection();
	connect(rw_socket, SIGNAL(readyRead()), this, SLOT(recvMessage()));

	info_label->setText("Connected with " + rw_socket->peerAddress().toString()
						+ ", port " + QString::number(rw_socket->peerPort()));

	if(is_self_defined_game)
	{
		Draughts draughts(self_defined_game.toStdString().c_str());
		chess_board->startGame(DraughtsInfo::black,
							   is_black_first ? DraughtsInfo::black : DraughtsInfo::white,
							   &draughts);
		QString text = OPER_SETGAME;
		text += is_black_first ? " B " : " W ";
		text += self_defined_game;
		rw_socket->write(text.toUtf8());
	} else {
		chess_board->startGame(DraughtsInfo::black);
		rw_socket->write(OPER_START);
	}
}

void MainWindow::startListening()
{
	ListenDialog dlg;
	if(dlg.exec() != QDialog::Accepted)
		return;

	QHostAddress listen_addr;
	if(!listen_addr.setAddress(dlg.getAddress()))
	{
		QMessageBox::warning(this, "Error", "Error: Invalid IP address.");
		return;
	}

	int listen_port = dlg.getPort();
	is_self_defined_game = dlg.isUserDefined();
	self_defined_game = dlg.getInitGameStatus();
	is_black_first = dlg.isBlackFirst();

	listen_socket = new QTcpServer;
	if(listen_socket->listen(listen_addr, listen_port))
	{
		connect(listen_socket, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

		info_label->setText("Listening on " + listen_addr.toString()
							+ ", port " + QString::number(listen_socket->serverPort()));

		for(const QHostAddress& address : QNetworkInterface::allAddresses())
		{
			if(address.protocol() == QAbstractSocket::IPv4Protocol
					&& address != QHostAddress(QHostAddress::LocalHost))
				info_label->setText("Listening on " + listen_addr.toString()
									+ ", port " + QString::number(listen_socket->serverPort())
									+ ". Server: " + address.toString());
		}
	} else {
		QMessageBox::warning(this, "Error", "Error: " + listen_socket->errorString());
		delete listen_socket;
	}
}

void MainWindow::gameEnd()
{
	QMessageBox *msg_box = new QMessageBox(this);
	msg_box->setModal(false);
	msg_box->setStandardButtons(QMessageBox::Ok);
	msg_box->setWindowTitle("Game Over");
	if(chess_board->getCurrentPlayer() == chess_board->getPlayer())
		msg_box->setText("You lost!");
	else msg_box->setText("Congratulations! You win!");
	msg_box->show();
}

void MainWindow::peaceEnd()
{
	chess_board->clearMarks();
	QMessageBox::information(this, "Game Over", "Peace!");
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

void MainWindow::on_actionMake_Peace_triggered()
{
	if(rw_socket)
	{
		rw_socket->write(OPER_MAKEPEACE);
	}
}
