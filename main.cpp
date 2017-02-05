#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <QtWidgets\qsplashscreen.h>
#include <Windows.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//³ÌÐòÆô¶¯»­Ãæ
	//QSplashScreen *splash = new QSplashScreen;
	//splash->setPixmap(QPixmap("./Blank.jpg"));
	//splash->show();
	//splash->showMessage(QObject::tr("Loading..."),
	//	Qt::Alignment(Qt::AlignRight | Qt::AlignTop), Qt::white);

	MainWindow w;

	//splash->finish(&w);

	w.show();
	return a.exec();
}
