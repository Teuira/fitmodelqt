#include "dialogtreeselector.h"
#include "ui_dialogtreeselector.h"
#include "dialogstatsselector.h"
#include "msgboxes.h"

#include <fstream>
#include <iostream>

#include <QFileDialog>
#include <QMessageBox>

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
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Wrong format", "Incorrect file format!\nMaybe this file has been created with an older version of FitModel.\nDo You want to try to convert it?",
                                        QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            // Creates a converted copy of the file.
            std::string converted = "";
            bool isSecond = false;
            for (size_t i = 0; i < content.length(); i++) {
                converted += content[i];
                if (content[i] == ':') {
                    if (isSecond) {
                        converted += ":";
                        isSecond = false;
                    }
                    else {
                        isSecond = true;
                    }
                }
            }
            // Saves file.
            std::string convertedOut = fileName.toStdString() + "_converted_wbest";
            std::ofstream out(convertedOut);
            out << converted;
            out.close();
            infoBox(QString::fromStdString("New file saved to: " + convertedOut + "\n\nPress \"Next\" to continue."));
            // Sets new path.
            ui->leditTreePath->setText(QString::fromStdString(convertedOut));
        }
        return;
    }
    // Sets tree.
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
