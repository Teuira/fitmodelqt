#include "dialogstatsselector.h"
#include "ui_dialogstatsselector.h"

DialogStatsSelector::DialogStatsSelector(QString treePath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogStatsSelector)
{
    ui->setupUi(this);
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
