#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <iostream>

#include <vector>
#include "fitmodeltreewrapper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/assets/splash.png"));
    splash->show();

    MainWindow w;
    QTimer::singleShot(2500, splash, SLOT(close()));
    QTimer::singleShot(3000, &w, SLOT(show()));
    // w.show();

    return a.exec();
}
