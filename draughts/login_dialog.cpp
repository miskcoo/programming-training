#include "login_dialog.h"
#include "ui_login_dialog.h"
#include <QtWidgets>
#include <QHostAddress>

LoginDialog::LoginDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LoginDialog)
{
	ui->setupUi(this);
	ui->port_input->setValidator(new QIntValidator(1, 65536, this));
	ui->ip_input->setFocus();
}

QString LoginDialog::getAddress()
{
	return ui->ip_input->text();
}

int LoginDialog::getPort()
{
	return ui->port_input->text().toInt();
}

LoginDialog::~LoginDialog()
{
	delete ui;
}
