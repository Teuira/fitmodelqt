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
    // Sets background image
    QPixmap bkgnd(":/assets/WbestSelectionScreenBack.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
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
        msgBox.setIcon(QMessageBox::Warning);
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
