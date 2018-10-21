#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <qfiledialog.h>
#include <qdesktopwidget.h>
#include <qstyle.h>

#include "dialoglauncher.h"
#include "dialogtreeselector.h"
extern "C" {
#include "fitmodel/fit_launcher.h"
}
#include "dialoginfo.h"

#define PRINT_DEBUG

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("FitModel Qt");
    // Sets background image
    QPixmap bkgnd(":/assets/tree.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    // Centers window on screen
    this->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            qApp->desktop()->availableGeometry()
        )
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    std::cout << "Launcher requested" << std::endl;
    this->setVisible(false);
    DialogLauncher dlgLauncher;
    dlgLauncher.setModal(true);
    dlgLauncher.exec();
    this->setVisible(true);
    std::cout << "comeback" << std::endl;
}

void MainWindow::on_pushButton_2_clicked()
{
    std::cout << "Viewer requested" << std::endl;
    this->setVisible(false);
    DialogTreeSelector dlgTreeSelector;
    dlgTreeSelector.setModal(true);
    dlgTreeSelector.exec();
    this->setVisible(true);
    std::cout << "comeback" << std::endl;
}

void MainWindow::on_btnInfo_clicked()
{
    std::cout << "Infos requested" << std::endl;
    this->setVisible(false);
    DialogInfo dlgInfo;
    dlgInfo.setModal(true);
    dlgInfo.exec();
    this->setVisible(true);
    std::cout << "comeback" << std::endl;
}
