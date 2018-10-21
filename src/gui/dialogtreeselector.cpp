#include "dialogtreeselector.h"
#include "ui_dialogtreeselector.h"
#include "dialogstatsselector.h"

#include <iostream>

#include <QFileDialog>
#include <QMessageBox>

DialogTreeSelector::DialogTreeSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTreeSelector)
{
    ui->setupUi(this);
    // Sets background to white.
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->show();
}

DialogTreeSelector::~DialogTreeSelector()
{
    delete ui;
}

void DialogTreeSelector::on_btnBrowseTree_clicked()
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
    ui->leditTreePath->setText(fileName);
}

void DialogTreeSelector::on_btnNext_clicked()
{
    if (ui->leditTreePath->text() == "") {
        QMessageBox msgBox;
        msgBox.setText("Select a file first.");
        msgBox.exec();
        return;
    }
    std::cout << "Tree path ok." << std::endl;
    this->setVisible(false);
    DialogStatsSelector dlgStatsSelector(ui->leditTreePath->text());
    dlgStatsSelector.setModal(true);
    dlgStatsSelector.exec();
    this->close();
}
