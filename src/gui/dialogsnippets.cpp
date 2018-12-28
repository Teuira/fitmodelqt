#include "dialogsnippets.h"
#include "ui_dialogsnippets.h"

#include <iostream>

/*
 * W3 = RED
 * W2 = GREEN
 * W1 = BLUE
 */

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
    ui->btnWxToWy->setStyleSheet("background-image:url(:/assets/GreenToBlue.png)");
    ui->btnWxToWy->setToolTip("Wx to Wy");
    ui->btnWxyToWz->setStyleSheet("background-image:url(:/assets/RedToGreen.png)");
    ui->btnWxyToWz->setToolTip("Wx or Wy to Wz");
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

void DialogSnippets::on_btnWxToWy_clicked()
{
    this->snippet = "from w2 to w1";
    this->accept();
}

void DialogSnippets::on_btnWxyToWz_clicked()
{
    this->snippet = "from w1 or w2 to w3";
    this->accept();
}

void DialogSnippets::on_btnClear_clicked()
{
    this->snippet = "";
    this->accept();
}
