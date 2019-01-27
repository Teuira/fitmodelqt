#include "reporter.h"

#include <iostream>
#include <fstream>

#include <QDate>
#include <QPainter>
#include <QPdfWriter>

#define BAR_HEIGHT 40
#define LABEL_X_OFFSET 10
#define TREE_VISUAL_WIDTH 800
#define TREE_VISUAL_HEIGHT 650

void Print_Tree_Qt(QPainter *pnt, edge *b_root, tdraw *w, arbre *tree);
void Print_Tree_Pre_Qt(QPainter *pnt, node *a, node *d, tdraw *w, arbre *tree);

Reporter::Reporter(QString fileName,
         QString statsPath,
         std::vector<FitModelTreeWrapper *> *forest,
         QListWidget *positions)
{
    this->fileName = fileName;
    this->statsPath = statsPath;
    this->forest = forest;
    this->positions = positions;
}

void Reporter::save(QString fileOut)
{
    QPdfWriter pdfwriter(fileOut);
    pdfwriter.setPageSize(QPageSize(QPageSize::A4));
    QPainter painter(&pdfwriter);
    QString line = "File name: ";
    line += this->fileName;
    painter.drawText(20, 10, line);
    line = "Date: ";
    QDate dat = QDate::currentDate();
    line += dat.toString();
    painter.drawText(20, 200, line);
    line = "Query: ";
    painter.scale(2.0, 2.0);
    line = "Total positions: ";
    line.append(QString::fromStdString(std::to_string(this->forest->size())));
    painter.drawText(20, 400, line);
    line = "Selected positions: ";
    line.append(QString::fromStdString(std::to_string(positions->count())));
    painter.drawText(20, 600, line);
    // Appends report if present.
    if (this->statsPath == "") {
        std::cout << "No report";
    } else {
        painter.scale(0.1, 0.1);
        int statsYPos = 10500;
        painter.drawText(20, statsYPos, "Statistics:");
        statsYPos += 50;
        std::ifstream infile(this->statsPath.toStdString());
        std::string line;
        int i = 1;
        while (std::getline(infile, line))
        {
            statsYPos += 100;
            painter.drawText(20, statsYPos, QString::fromStdString(line));
            if (i > 25) {
                break;
            }
            i += 1;
        }
    }
    // Renders images.
    painter.scale(5, 5);
    for (int i = 0; i < this->positions->count(); i++) {
        pdfwriter.newPage();
        QListWidgetItem* currItem = this->positions->item(i);
        int pos = currItem->text().toInt();
        line = "";
        line.append(QString::fromStdString(std::to_string(pos)));
        painter.drawText(20, 100, line);
        std::cout << pos << std::endl;
        QPixmap *pix = new QPixmap(TREE_VISUAL_WIDTH, TREE_VISUAL_HEIGHT);
        QPainter *paint = new QPainter(pix);
        FitModelTreeWrapper *currTree = forest->at(pos - 1);
        Print_Tree_Qt(paint, currTree->GetRoot(), currTree->GetDrawStruct(), currTree->GetTree());
        painter.drawImage(50, 300, pix->toImage());
        delete paint;
        delete pix;
        // DO NOT DELETE TREES.
    }
}

void Print_Tree_Qt(QPainter *pnt, edge *b_root, tdraw *w, arbre *tree)
{
    int i;
    double max,min,step;

    min = 0.0;
    max = 1.0;

    step = (max-min)/13.;

    if(b_root->prob_sel_regime <= min+1.*step)
        pnt->setPen(QColor(26, 26, 255));
    else if(b_root->prob_sel_regime > min+1.*step && b_root->prob_sel_regime <= min+2.*step)
        pnt->setPen(QColor(26, 77, 255));
    else if(b_root->prob_sel_regime > min+2.*step && b_root->prob_sel_regime <= min+3.*step)
        pnt->setPen(QColor(26, 128, 204));
    else if(b_root->prob_sel_regime > min+3.*step && b_root->prob_sel_regime <= min+4.*step)
        pnt->setPen(QColor(26, 178, 153));
    else if(b_root->prob_sel_regime > min+4.*step && b_root->prob_sel_regime <= min+5.*step)
        pnt->setPen(QColor(26, 230, 102));
    else if(b_root->prob_sel_regime > min+5.*step && b_root->prob_sel_regime <= min+6.*step)
        pnt->setPen(QColor(26, 230, 26));
    else if(b_root->prob_sel_regime > min+6.*step && b_root->prob_sel_regime <= min+7.*step)
        pnt->setPen(QColor(77, 204, 26));
    else if(b_root->prob_sel_regime > min+7.*step && b_root->prob_sel_regime <= min+8.*step)
        pnt->setPen(QColor(128, 204, 51));
    else if(b_root->prob_sel_regime > min+8.*step && b_root->prob_sel_regime <= min+9.*step)
        pnt->setPen(QColor(178, 178, 51));
    else if(b_root->prob_sel_regime > min+9.*step && b_root->prob_sel_regime <= min+10.*step)
        pnt->setPen(QColor(204, 153, 51));
    else if(b_root->prob_sel_regime > min+10.*step && b_root->prob_sel_regime <= min+11.*step)
        pnt->setPen(QColor(255, 128, 77));
    else if(b_root->prob_sel_regime > min+11.*step && b_root->prob_sel_regime <= 0.9)
        pnt->setPen(QColor(255, 64, 77));
    else if(b_root->prob_sel_regime > 0.9 && b_root->prob_sel_regime <= 1.0)
        pnt->setPen(QColor(255, 0, 0));


    QPainterPath path;
    path.moveTo(w->xcoord[b_root->left->num], w->ycoord[b_root->left->num] + BAR_HEIGHT);
    path.lineTo(0, w->ycoord[b_root->left->num] + BAR_HEIGHT);
    path.lineTo(0, w->ycoord[b_root->rght->num] + BAR_HEIGHT);
    path.lineTo(w->xcoord[b_root->rght->num], w->ycoord[b_root->rght->num] + BAR_HEIGHT);
    pnt->drawPath(path);

    if(b_root->left->tax) {
        qreal rcHeight = (double)w->page_height / tree->n_otu;
        QRectF rcLabel(w->xcoord[b_root->left->num] + LABEL_X_OFFSET, w->ycoord[b_root->left->num] + BAR_HEIGHT - rcHeight / 2.0, 100, rcHeight);
        pnt->drawText(rcLabel, Qt::AlignCenter, QString::fromStdString(b_root->left->name));
    }

    if(b_root->rght->tax) {
        qreal rcHeight = (double)w->page_height / tree->n_otu;
        QRectF rcLabel(w->xcoord[b_root->rght->num] + LABEL_X_OFFSET, w->ycoord[b_root->rght->num] + BAR_HEIGHT - rcHeight / 2.0, 100, rcHeight);
        pnt->drawText(rcLabel, Qt::AlignCenter, QString::fromStdString(b_root->rght->name));
    }

    if(!b_root->left->tax) {
        For(i,3)
                if((b_root->left->v[i]) && (b_root->left->v[i] != b_root->rght))
                Print_Tree_Pre_Qt(pnt, b_root->left, b_root->left->v[i], w, tree);
    }

    if(!b_root->rght->tax) {
        For(i,3)
                if((b_root->rght->v[i]) && (b_root->rght->v[i] != b_root->left))
                Print_Tree_Pre_Qt(pnt, b_root->rght, b_root->rght->v[i], w, tree);
    }

}


void Print_Tree_Pre_Qt(QPainter *pnt, node *a, node *d, tdraw *w, arbre *tree)
{
    int i;
    double min,max;
    double step;

    min = 0.0;
    max = 1.0;

    step = (max-min)/13.;

    For(i,3)
            if(a->v[i] == d) {
        if(a->b[i]->prob_sel_regime <= min+1.*step)
            pnt->setPen(QColor(26, 26, 255));
        else if(a->b[i]->prob_sel_regime > min+1.*step && a->b[i]->prob_sel_regime <= min+2.*step)
            pnt->setPen(QColor(26, 77, 255));
        else if(a->b[i]->prob_sel_regime > min+2.*step && a->b[i]->prob_sel_regime <= min+3.*step)
            pnt->setPen(QColor(26, 128, 204));
        else if(a->b[i]->prob_sel_regime > min+3.*step && a->b[i]->prob_sel_regime <= min+4.*step)
            pnt->setPen(QColor(26, 230, 102));
        else if(a->b[i]->prob_sel_regime > min+4.*step && a->b[i]->prob_sel_regime <= min+5.*step)
            pnt->setPen(QColor(26, 230, 26));
        else if(a->b[i]->prob_sel_regime > min+5.*step && a->b[i]->prob_sel_regime <= min+6.*step)
            pnt->setPen(QColor(26, 230, 26));
        else if(a->b[i]->prob_sel_regime > min+6.*step && a->b[i]->prob_sel_regime <= min+7.*step)
            pnt->setPen(QColor(77, 204, 26));
        else if(a->b[i]->prob_sel_regime > min+7.*step && a->b[i]->prob_sel_regime <= min+8.*step)
            pnt->setPen(QColor(77, 204, 26));
        else if(a->b[i]->prob_sel_regime > min+8.*step && a->b[i]->prob_sel_regime <= min+9.*step)
            pnt->setPen(QColor(128, 204, 51));
        else if(a->b[i]->prob_sel_regime > min+9.*step && a->b[i]->prob_sel_regime <= min+10.*step)
            pnt->setPen(QColor(204, 153, 51));
        else if(a->b[i]->prob_sel_regime > min+10.*step && a->b[i]->prob_sel_regime <= min+11.*step)
            pnt->setPen(QColor(255, 128, 77));
        else if(a->b[i]->prob_sel_regime > min+11.*step && a->b[i]->prob_sel_regime <= 0.9)
            pnt->setPen(QColor(255, 64, 77));
        else if(a->b[i]->prob_sel_regime > 0.9 && a->b[i]->prob_sel_regime <= 1.0)
            pnt->setPen(QColor(255, 0, 0));

        break;
    }

    QPainterPath path;
    path.moveTo(w->xcoord[a->num], w->ycoord[a->num] + BAR_HEIGHT);
    path.lineTo(w->xcoord[a->num], w->ycoord[d->num] + BAR_HEIGHT);
    path.lineTo(w->xcoord[d->num], w->ycoord[d->num] + BAR_HEIGHT);
    pnt->drawPath(path);

    if(d->tax) {
        qreal rcHeight = (double)w->page_height / tree->n_otu;
        QRectF rcLabel(w->xcoord[d->num] + LABEL_X_OFFSET, w->ycoord[d->num] + BAR_HEIGHT - rcHeight / 2.0, 100, rcHeight);
        pnt->drawText(rcLabel, Qt::AlignCenter, QString::fromStdString(d->name));
        return;
    }
    else {
        For(i,3)
                if(d->v[i] != a)
                Print_Tree_Pre_Qt(pnt, d, d->v[i], w, tree);
    }

    return;
}
