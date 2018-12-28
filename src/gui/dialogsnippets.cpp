#include "dialogsnippets.h"
#include "ui_dialogsnippets.h"

#include <iostream>

DialogSnippets::DialogSnippets(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSnippets)
{
    ui->setupUi(this);
    this->snippet = "";
    this->setStyleSheet("background-color:black;");
    // First column of buttons.
    ui->btnEq->setStyleSheet("background-image:url(:/assets/Eq.png)");
    ui->btnEq->setToolTip("Equality test");
    ui->btnDiff->setStyleSheet("background-image:url(:/assets/Diff.png)");
    ui->btnDiff->setToolTip("Non-Equality test");
    ui->btnSim->setStyleSheet("background-image:url(:/assets/Sim.png)");
    ui->btnSim->setToolTip("Similarity test");
    // Second column of buttons (whole tree)
    ui->btnHasRed->setStyleSheet("background-image:url(:/assets/HasRed.png)");
    ui->btnHasRed->setToolTip("Checks if tree has w3");
    ui->btnHasGreen->setStyleSheet("background-image:url(:/assets/HasGreen.png)");
    ui->btnHasGreen->setToolTip("Checks if tree has w2");
    ui->btnHasBlue->setStyleSheet("background-image:url(:/assets/HasBlue.png)");
    ui->btnHasBlue->setToolTip("Checks if tree has w1");
    // Third column of buttons.
    ui->btnGreenToBlue->setStyleSheet("background-image:url(:/assets/GreenToBlue.png)");
    ui->btnGreenToBlue->setToolTip("w2 to w1");
    ui->btnRedToGreen->setStyleSheet("background-image:url(:/assets/RedToGreen.png)");
    ui->btnRedToGreen->setToolTip("w3 to w2");
    ui->btnClear->setStyleSheet("background-image:url(:/assets/Clear.png)");
    ui->btnClear->setToolTip("Clear");
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
    this->snippet = "sim max_diff";
    this->accept();
}

void DialogSnippets::on_btnHasRed_clicked()
{
    this->snippet = "has w3";
    this->accept();
}

void DialogSnippets::on_btnHasGreen_clicked()
{
    this->snippet = "has w2";
    this->accept();
}

void DialogSnippets::on_btnHasBlue_clicked()
{
    this->snippet = "has w1";
    this->accept();
}

void DialogSnippets::on_btnGreenToBlue_clicked()
{
    this->snippet = "from w2 to w1";
    this->accept();
}

void DialogSnippets::on_btnRedToGreen_clicked()
{
    this->snippet = "from w3 to w2";
    this->accept();
}

void DialogSnippets::on_btnClear_clicked()
{
    this->snippet = "";
    this->accept();
}
