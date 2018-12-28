#include "dialoglauncher.h"
#include "ui_dialoglauncher.h"
#include "msgboxes.h"

#include <fstream>
#include <iostream>

#include <QFileDialog>
#include <QProcess>

#define PRINT_DEBUG

DialogLauncher::DialogLauncher(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLauncher)
{
    ui->setupUi(this);
    // Sets background to white.
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->show();
}

DialogLauncher::~DialogLauncher()
{
    delete ui;
}

void DialogLauncher::on_btnBrowseSeq_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Sequence file"), "", tr("Sequence Files (*.*)"));
    if (fileName == "") {
#ifdef PRINT_DEBUG
        std::cout << "No file selected!" << std::endl;
#endif
        return;
    }
    ui->leditSeq->setText(fileName);
}

void DialogLauncher::on_btnBrowseTree_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Tree file"), "", tr("Tree Files (*.*)"));
    if (fileName == "") {
#ifdef PRINT_DEBUG
        std::cout << "No file selected!" << std::endl;
#endif
        return;
    }
    ui->leditTree->setText(fileName);
}

void DialogLauncher::on_bntRun_clicked()
{
#ifdef WIN32
    QString fitmodelPath = QDir::cleanPath(QCoreApplication::applicationDirPath() + QDir::separator() + "fitmodel.exe");
#else
    QString fitmodelPath = QDir::cleanPath(QCoreApplication::applicationDirPath() + QDir::separator() + "fitmodel");
#endif
#ifdef PRINT_DEBUG
    std::cout << fitmodelPath.toStdString() << std::endl;
#endif
    if (ui->leditTree->text().isEmpty() || ui->leditSeq->text().isEmpty()) {
        infoBox("There are some missing fields!");
        return;
    }
    QFileInfo pathInfo(fitmodelPath);
    if (pathInfo.exists() && pathInfo.isFile()) {
        // TODO: run fitmodel.
        // Generates input txt for fitmodel.
        QString txtPath = QDir::cleanPath(QCoreApplication::applicationDirPath() + QDir::separator() + "f.txt");
        std::ofstream txtFile(txtPath.toStdString());
        txtFile << ui->leditTree->text().toStdString() << std::endl;
        txtFile << ui->leditSeq->text().toStdString() << std::endl;
        txtFile << "Y" << std::endl;
        txtFile << "Y" << std::endl;
        txtFile.close();
        QProcess *proc = new QProcess;
        //proc->setReadChannelMode(QProcess::SeparateChannels);
        //proc->setStandardInputFile(txtPath);
        //proc->start(fitmodelPath);
        proc->startDetached("cmd.exe");
    } else {
        // fitmodel not found.
        errorBox("fitmodel not found.");
    }
}
