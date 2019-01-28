#include "dialogtreeselector.h"
#include "ui_dialogtreeselector.h"
#include "dialogstatsselector.h"
#include "msgboxes.h"

#include <fstream>
#include <iostream>

#include <QFileDialog>

DialogTreeSelector::DialogTreeSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTreeSelector)
{
    ui->setupUi(this);
    ui->lblTreeLoadHeader->setStyleSheet("QLabel { color : white; }");
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
    // Checks for file version.
    std::ifstream ifs(fileName.toStdString());
    std::string content((std::istreambuf_iterator<char>(ifs)),
                           (std::istreambuf_iterator<char>()));
    if (content.find("::") == std::string::npos) {
        warningBox("Incorrect file format!\nMaybe this file has been created with an older version of FitModel.");
        return;
    }
    ui->leditTreePath->setText(fileName);
}

void DialogTreeSelector::on_btnNext_clicked()
{
    if (ui->leditTreePath->text() == "") {
        warningBox("Select a file first.");
        return;
    }
    std::cout << "Tree path ok." << std::endl;
    this->setVisible(false);
    DialogStatsSelector dlgStatsSelector(ui->leditTreePath->text());
    dlgStatsSelector.setModal(true);
    dlgStatsSelector.exec();
    this->close();
}
