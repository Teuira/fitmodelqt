#include "dialogsnippets.h"
#include "ui_dialogsnippets.h"

#include <iostream>

DialogSnippets::DialogSnippets(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSnippets)
{
    ui->setupUi(this);
    this->snippet = "";
    ui->btnEq->setStyleSheet("background-image:url(:/assets/Eq.png)");
    ui->btnEq->setToolTip("Equality test");
    ui->btnDiff->setStyleSheet("background-image:url(:/assets/Diff.png)");
    ui->btnDiff->setToolTip("Non-Equality test");
    ui->btnSim->setStyleSheet("background-image:url(:/assets/Sim.png)");
    ui->btnSim->setToolTip("Similarity test");
}

DialogSnippets::~DialogSnippets()
{
    delete ui;
}

void DialogSnippets::on_btnEq_clicked()
{
    this->snippet = "eq";
    this->accept();
}

void DialogSnippets::on_btnDiff_clicked()
{
    this->snippet = "diff";
    this->accept();
}

void DialogSnippets::on_btnSim_clicked()
{
    this->snippet = "sim <max_diff>";
    this->accept();
}
