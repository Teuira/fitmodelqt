#include "dialoginfo.h"
#include "ui_dialoginfo.h"

DialogInfo::DialogInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInfo)
{
    ui->setupUi(this);
    // Sets background to white.
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->show();
}

DialogInfo::~DialogInfo()
{
    delete ui;
}
