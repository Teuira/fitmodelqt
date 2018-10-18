#include "dialoglauncher.h"
#include "ui_dialoglauncher.h"

#include <QFileDialog>

DialogLauncher::DialogLauncher(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLauncher)
{
    ui->setupUi(this);
}

DialogLauncher::~DialogLauncher()
{
    delete ui;
}

void DialogLauncher::on_btnBrowseSeq_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Sequence file"), "", tr("Sequence Files (*.nxs)"));
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
        tr("Tree file"), "", tr("Tree Files (*.tree)"));
    if (fileName == "") {
#ifdef PRINT_DEBUG
        std::cout << "No file selected!" << std::endl;
#endif
        return;
    }
    ui->leditTree->setText(fileName);
}
