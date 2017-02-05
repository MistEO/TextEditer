#ifndef TEXTEDITER_H
#define TEXTEDITER_H

#include <QtWidgets/QMainWindow>
#include "ui_textediter.h"

class TextEditer : public QMainWindow
{
	Q_OBJECT

public:
	TextEditer(QWidget *parent = 0);
	~TextEditer();

private:
	Ui::TextEditerClass ui;
};

#endif // TEXTEDITER_H
