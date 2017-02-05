#include "textediter.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TextEditer w;
	w.show();
	return a.exec();
}
