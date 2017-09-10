#include "listen_dialog.h"
#include "ui_listen_dialog.h"

ListenDialog::ListenDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ListenDialog)
{
	ui->setupUi(this);
	ui->port_input->setValidator(new QIntValidator(1, 65536, this));
	ui->ip_input->setFocus();
	on_self_defined_game_toggled(false);
}

ListenDialog::~ListenDialog()
{
	delete ui;
}

void ListenDialog::on_self_defined_game_toggled(bool checked)
{
	ui->label_init_game->setVisible(checked);
	ui->init_game->setVisible(checked);
	ui->black_first->setVisible(checked);
	ui->white_first->setVisible(checked);
}

QString ListenDialog::getAddress()
{
	return ui->ip_input->text();
}

int ListenDialog::getPort()
{
	return ui->port_input->text().toInt();
}

bool ListenDialog::isUserDefined()
{
	return ui->self_defined_game->isChecked();
}

QString ListenDialog::getInitGameStatus()
{
	return ui->init_game->text();
}

bool ListenDialog::isBlackFirst()
{
	return ui->black_first->isChecked();
}
