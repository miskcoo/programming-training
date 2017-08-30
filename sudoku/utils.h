#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <QFont>
#include <QString>
#include <QRect>

typedef std::vector<int> IntList;
typedef std::vector<IntList> IntMatrix;

QFont fit_font_with_text(QFont font, QString text, QRect rect);

#endif // UTILS_H
