#ifndef LISTEN_DIALOG_H
#define LISTEN_DIALOG_H

#include <QDialog>

namespace Ui {
class ListenDialog;
}

class ListenDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ListenDialog(QWidget *parent = 0);
	~ListenDialog();

	QString getAddress();
	int getPort();
	bool isUserDefined();
	QString getInitGameStatus();
	bool isBlackFirst();

private slots:
	void on_self_defined_game_toggled(bool checked);

private:
	Ui::ListenDialog *ui;
};

#endif // LISTEN_DIALOG_H
