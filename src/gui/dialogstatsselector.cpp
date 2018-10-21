#include "dialogstatsselector.h"
#include "ui_dialogstatsselector.h"
#include "dialogviewer.h"

#include <iostream>

#include <QFileDialog>
#include <QMessageBox>

DialogStatsSelector::DialogStatsSelector(QString treePath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogStatsSelector)
{
    ui->setupUi(this);
    this->strTreePath = treePath;
    // Sets background to white.
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->show();
}

DialogStatsSelector::~DialogStatsSelector()
{
    delete ui;
}

void DialogStatsSelector::on_btnBrowseStats_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open tree file"), ""
                                                    , tr("Tree Files (*.*tree*)"));
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
    if (ui->leditStatsPath->text() == "") {
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
