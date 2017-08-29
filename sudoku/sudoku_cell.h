#ifndef SUDOKUCELL_H
#define SUDOKUCELL_H

#include <QtWidgets>

class SudokuCell : public QLabel
{
    Q_OBJECT
public:
    explicit SudokuCell(QWidget *parent = 0);

signals:

public slots:
};

#endif // SUDOKUCELL_H
