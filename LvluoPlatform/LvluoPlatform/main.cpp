#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <ctime>
#include <cstdlib>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	srand((unsigned int)time(nullptr));
	MainWindow w;
	w.show();
	return a.exec();
}
