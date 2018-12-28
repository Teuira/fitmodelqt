#include "dialogstatsselector.h"
#include "ui_dialogstatsselector.h"
#include "dialogviewer.h"

#include <iostream>

#include <QFileDialog>

DialogStatsSelector::DialogStatsSelector(QString treePath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogStatsSelector)
{
    ui->setupUi(this);
    this->strTreePath = treePath;
    ui->lblStatsHeader->setStyleSheet("QLabel { color : white; }");
    // Sets background image
    QPixmap bkgnd(":/assets/StatsSelectionScreenBack.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    this->show();
}

DialogStatsSelector::~DialogStatsSelector()
{
    delete ui;
}

void DialogStatsSelector::on_btnBrowseStats_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open statistics file"), ""
                                                    , tr("Statistics files (*.*)"));
    if (fileName == "") {
#ifdef PRINT_DEBUG
        std::cout << "No file selected!" << std::endl;
#endif
        return;
    }
    ui->leditStatsPath->setText(fileName);
}

void DialogStatsSelector::on_btnNext_clicked()
{
    if (ui->leditStatsPath->text() != "") {
        std::cout << "Stats path ok." << std::endl;
    } else {
        std::cout << "No stats file." << std::endl;
    }
    this->setVisible(false);
    DialogViewer dlgViewer(this->strTreePath, ui->leditStatsPath->text());
    dlgViewer.setModal(true);
    dlgViewer.exec();
    this->close();
}
