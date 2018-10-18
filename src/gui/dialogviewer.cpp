#include "dialogviewer.h"
#include "ui_dialogviewer.h"

#include <iostream>
#include <QPushButton>
#include <QFileInfo>

#include "phylotreewidget.h"

#define PRINT_DEBUG

DialogViewer::DialogViewer(QString filePath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogViewer)
{
    ui->setupUi(this);
#ifdef PRINT_DEBUG
    std::cout << "Working on file: " << filePath.toStdString() << std::endl;
#endif
    this->path = filePath;
    //this->trees = Tree::LoadForest(filePath.toStdString());
    this->trees = FitModelTreeWrapper::LoadForest(filePath.toStdString());
#ifdef PRINT_DEBUG
    std::cout << "The file contains: " << this->trees->size() << " trees." << std::endl;
#endif
    QFileInfo fi(filePath);
    QString fileName = fi.fileName();
    ui->phyloWidget->SetName(fileName);
    ui->phyloWidget->SetForest(this->trees);
}

DialogViewer::~DialogViewer()
{
    delete ui;
    for (size_t i = 0; i < this->trees->size(); i++) {
        delete this->trees->at(i);
    }
}
