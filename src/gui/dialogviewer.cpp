#include "dialogviewer.h"
#include "ui_dialogviewer.h"

#include <iostream>
#include <QPushButton>
#include <QFileInfo>
#include <QPainter>

#include "phylotreewidget.h"

#define PRINT_DEBUG

DialogViewer::DialogViewer(QString filePath, QString statsPath, QWidget *parent) :
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
    ui->phyloWidget->SetStatsPath(statsPath);
    ui->phyloWidget->SetForest(this->trees);
}

//!
//! \brief DialogViewer::paintEvent
//! \details All of the painting events and widget resising happens here.
//!
void DialogViewer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(QRectF(0.0, 0.0, this->width(), this->height()), QColor(0,0,0));
}

DialogViewer::~DialogViewer()
{
    delete ui;
    for (size_t i = 0; i < this->trees->size(); i++) {
        delete this->trees->at(i);
    }
}
