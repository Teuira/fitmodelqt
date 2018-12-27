#include "phylotreewidget.h"
#include "queryparser.h"
#include "dialogsnippets.h"

#include <algorithm>
#include <fstream>
#include <iostream>

#include <QPainter>
#include <QPushButton>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QPdfWriter>
#include <QDate>
#include <QDesktopServices>

#define BAR_HEIGHT 40
#define TREE_VISUAL_WIDTH 800
#define TREE_VISUAL_HEIGHT 750
#define LABEL_X_OFFSET 10

static inline bool InSel(double x, double y, QRectF *rcSel) {
    qreal l, t, r, b;
    if (rcSel->width() < 0) {
        l = rcSel->right();
        r = rcSel->left();
    } else {
        l = rcSel->left();
        r = rcSel->right();
    }
    if (rcSel->height() < 0) {
        t = rcSel->bottom();
        b = rcSel->top();
    } else {
        t = rcSel->top();
        b = rcSel->bottom();
    }
    if (x > l && x < r && y > t && y < b) {
        //std::cout << "true" << std::endl;
        return true;
    }
    return false;
}

PhyloTreeWidget::PhyloTreeWidget(QWidget *parent) : QWidget(parent)
{
    this->currTree = nullptr;
    this->prev = new QPushButton("<", this);
    this->next = new QPushButton(">", this);
    this->btnSelectWholeTree = new QPushButton("Whole tree", this);
    this->btnQuery = new QPushButton("Query", this);
    this->btnSnippets = new QPushButton("Snippets", this);
    this->lineQuery = new QLineEdit(this);
    this->listPos = new QListWidget(this);
    this->btnSnapshot = new QPushButton("Snapshot", this);
    this->btnReport = new QPushButton("Save Report", this);
    connect(prev, SIGNAL (released()), this, SLOT (handlePrevious()));
    connect(next, SIGNAL (released()), this, SLOT (handleNext()));
    connect(btnSelectWholeTree, SIGNAL (released()), this, SLOT (handleSelWhole()));
    connect(btnQuery, SIGNAL (released()), this, SLOT (handleQuery()));
    connect(btnSnippets, SIGNAL (released()), this, SLOT (handleSnippets()));
    connect(btnSnapshot, SIGNAL (released()), this, SLOT (handleSnapshot()));
    connect(btnReport, SIGNAL (released()), this, SLOT (handleReport()));
    connect(this->listPos, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(handleListPosClicked(QListWidgetItem*)));
    this->prev->setGeometry(120, 0, 30, 30);
    this->next->setGeometry(150, 0, 30, 30);
    this->btnSelectWholeTree->setGeometry(200, 0, 100, 30);
    this->btnSnapshot->setGeometry(320, 0, 100, 30);
    this->positions = new std::vector<int>;
}

PhyloTreeWidget::~PhyloTreeWidget()
{
    delete this->prev;
    delete this->next;
    delete this->btnSelectWholeTree;
    delete this->btnQuery;
    delete this->btnSnippets;
    delete this->lineQuery;
    delete this->listPos;
    delete this->btnSnapshot;
    delete this->positions;
    delete this->btnReport;
}

void PhyloTreeWidget::SetForest(std::vector<FitModelTreeWrapper *> *forest) {
    this->forest = forest;
    this->currIndex = 0;
    this->currTree = forest->at(this->currIndex);
    this->repaint();
}

void PhyloTreeWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->pos().y() > this->height() - 20) {
        std::cout << "pressed for browse" << std::endl;
        // x:w = i=t
        this->currIndex = this->forest->size() * (event->pos().x() + 5) / this->width();
        this->currTree = this->forest->at(this->currIndex);
        this->repaint();
        return;
    }

    std::cout << "pressed for rect" << std::endl;
    this->mousePressed = true;

    this->selNodes.clear();
    this->rcSelection.setTopLeft(event->pos());
    this->rcSelection.setBottomRight(event->pos());
    std::cout << event->pos().x() << std::endl;
}

void PhyloTreeWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->type() == QEvent::MouseMove)
        this->rcSelection.setBottomRight(event->pos());

    // refresh
    update();
}

void PhyloTreeWidget::mouseReleaseEvent(QMouseEvent *event)
{
    std::cout << "released" << std::endl;
    this->mousePressed = false;
    // refresh
    update();
    repaint();
    std::cout << "Selected nodes: " << this->selNodes.size() << std::endl;
}

void PhyloTreeWidget::paintEvent(QPaintEvent *)
{
    this->btnQuery->setGeometry(this->width() - 100, 0, 100, 30);
    this->lineQuery->setGeometry(440, 0, this->width() - 680, 30);
    this->btnSnippets->setGeometry(this->lineQuery->width() + 460, 0, 100, 30);
    this->listPos->setGeometry(TREE_VISUAL_WIDTH, BAR_HEIGHT + 50.0, this->width() - TREE_VISUAL_WIDTH, this->height() - BAR_HEIGHT - 130);
    this->btnReport->setGeometry(this->listPos->geometry().x(), this->listPos->geometry().y() + this->listPos->geometry().height() + 10, this->listPos->geometry().width(), 30);
    QPainter painter(this);
    if (this->mousePressed) {
        painter.drawRect(this->rcSelection);
    }
    // Draws bars
    painter.fillRect(QRectF(TREE_VISUAL_WIDTH, 0, this->width() - TREE_VISUAL_WIDTH, this->height()), QColor(0, 32, 96));
    painter.fillRect(QRectF(0.0, 0.0, this->width(), 32), QColor(79, 129, 189));
    painter.fillRect(QRectF(0.0, this->height() - 20, this->width(), 20), QColor(79, 129, 189));
    // End Draw bars
    if(this->currTree != nullptr) {
        QFont font = painter.font();
        int fontSize = (int)((double)this->currTree->GetDrawStruct()->page_height / (double)this->currTree->GetTree()->n_otu);
        // Sets max font size.
        if (fontSize > 15) {
            fontSize = 15;
        }
        font.setPixelSize(fontSize);
        painter.setFont(font);
        Print_Tree_Qt(&painter, this->currTree->GetRoot(), this->currTree->GetDrawStruct(), this->currTree->GetTree());
    }
    painter.setPen(Qt::black);
    QRectF rectTreeName = QRectF(0, 0, 100, 30);
    QFont font = painter.font();
    font.setPixelSize(rectTreeName.height() / 2);
    font.setItalic(true);
    painter.setFont(font);
    if (!this->selNodes.empty() && !this->mousePressed) {
        painter.fillRect(rectTreeName, QColor(255, 255, 0));
        painter.drawRect(rectTreeName);
        painter.drawText(rectTreeName, Qt::AlignCenter, QString::fromStdString(this->currTree->GetName()));
    } else {
        painter.drawRect(rectTreeName);
        painter.drawText(rectTreeName, Qt::AlignCenter, QString::fromStdString(this->currTree->GetName()));
    }
    painter.drawLine(0, this->height() - 10, this->width(), this->height() - 10);
    painter.setPen(Qt::blue);
    // x:w=c:nt
    if (this->positions) {
        if (!this->positions->empty()){
            for (size_t i = 0; i < this->positions->size(); i++) {
                painter.setBrush(QBrush(Qt::red));
                painter.drawEllipse(this->width() * (qreal)this->positions->at(i) / (qreal)this->forest->size() - 5, this->height() - 10 - 5, 10, 10);
            }
        }
    }
    painter.setBrush(QBrush(Qt::transparent));
    painter.drawEllipse(this->width() * (qreal)this->currIndex / (qreal)this->forest->size() - 7, this->height() - 10 - 7, 14, 14);
}

void PhyloTreeWidget::handlePrevious()
{
    std::cout << "prev" << std::endl;
    if (this->currIndex > 0) {
        this->currIndex--;
        this->currTree = this->forest->at((size_t)this->currIndex);
        this->repaint();
    }
}

void PhyloTreeWidget::handleNext()
{
    std::cout << "next" << std::endl;
    if (this->currIndex < this->forest->size() - 1) {
        this->currIndex++;
        this->currTree = this->forest->at((size_t)this->currIndex);
        this->repaint();
    }
}

void PhyloTreeWidget::handleSelWhole()
{
    // Resets selection rectangle.
    this->rcSelection.setTopLeft(QPoint(-1, -1));
    this->rcSelection.setBottomRight(QPoint(-1, -1));

    std::cout << "Add whole tree" << std::endl;
    this->selNodes.clear();
    TraversalAdd(this->currTree->GetRoot(), this->currTree->GetTree());
    std::cout << "Selected nodes: " << this->selNodes.size() << std::endl;
    QMessageBox msgBox;
    msgBox.setText("Whole tree selected.");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
    update();
}

void PhyloTreeWidget::handleQuery()
{
    std::string query = this->lineQuery->text().toStdString();
    std::cout << "Query: " << query << std::endl;
    QueryParser *parser = nullptr;
    // "" is a special reset query.
    if (query == "") {
        this->positions->clear();
        this->listPos->clear();
        this->repaint();
        return;
    }
    if (this->selNodes.empty()) {
        /*
        std::vector<edge *> *rootsOfForest = new std::vector<edge *>;
        for (size_t i = 0; i < this->forest->size(); i++) {
            rootsOfForest->push_back(this->forest->at(i)->GetRoot());
        }
        parser = new QueryParser(query, rootsOfForest);
        parser->Parse();
        delete rootsOfForest;
        */
        QMessageBox msgBox;
        msgBox.setText("Select some nodes first.");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    } else {
        std::vector<std::vector<node *> *> *nodesOfForest = new std::vector<std::vector<node *> *>;
        for (size_t i = 0; i < this->forest->size(); i++) {
            std::vector<node *> *nodesOfTree = new std::vector<node *>;
            for (size_t y = 0; y < this->selNodes.size(); y++) {
                nodesOfTree->push_back(this->forest->at(i)->GetTree()->noeud[this->selNodes.at(y)->num]);
            }
            nodesOfForest->push_back(nodesOfTree);
        }
        parser = new QueryParser(query, nodesOfForest, (size_t)this->currIndex);
        parser->Parse(this->positions);
        for (size_t i = 0; i < nodesOfForest->size(); i++) {
            delete nodesOfForest->at(i);
        }
        delete nodesOfForest;
        // Refreshes list.
        this->listPos->clear();
        for (size_t i = 0; i < this->positions->size(); i++) {
            this->listPos->addItem(QString(std::to_string(this->positions->at(i) + 1).c_str()));
        }
    }
    if (parser)
        delete parser;
    this->repaint();
}

void PhyloTreeWidget::handleSnippets()
{
    std::cout << "Snippets requested" << std::endl;
    DialogSnippets dlgSnippets;
    dlgSnippets.setModal(true);
    dlgSnippets.exec();
    this->lineQuery->setText(QString::fromStdString(dlgSnippets.snippet));
    std::cout << "comeback to viewer" << std::endl;
}

void PhyloTreeWidget::handleSnapshot()
{
    std::cout << "Snapshot requested" << std::endl;
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save snapshot"), "", tr("PNG file (*.png)"));
    if (!fileName.endsWith(".png"))
        fileName += ".png";
    if (fileName != ".png") {
        this->grab().save(fileName);
        QMessageBox msgBox;
        msgBox.setText("Image saved!");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void PhyloTreeWidget::handleReport()
{
    std::cout << "Report requested" << std::endl;
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save report"), "", tr("PDF file (*.pdf)"));
    if (!fileName.endsWith(".pdf"))
        fileName += ".pdf";
    if (fileName != "") {
        QPdfWriter pdfwriter(fileName);
        pdfwriter.setPageSize(QPageSize(QPageSize::A4));
        QPainter painter(&pdfwriter);
        QString line = "File name: ";
        line += this->name;
        painter.drawText(20, 10, line);
        line = "Date: ";
        QDate dat = QDate::currentDate();
        line += dat.toString();
        painter.drawText(20, 200, line);
        line = "Query: ";
        painter.scale(2.0, 2.0);
        line += this->lineQuery->text() == "" ? "No query" : this->lineQuery->text();
        painter.drawText(20, 400, line);
        line = "Match count: ";
        line.append(QString::fromStdString(std::to_string(this->positions->size())));
        painter.drawText(20, 600, line);
        line = "Total positions: ";
        line.append(QString::fromStdString(std::to_string(this->forest->size())));
        painter.drawText(20, 800, line);
        // Draws the image.
        painter.scale(5, 5);
        QPixmap img = this->grab(QRect(0, BAR_HEIGHT, 800, 700));
        painter.drawImage(50, 300, img.toImage());
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
        // End of report.
        QMessageBox msgBox;
        msgBox.setText("Report saved!");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
        // Opens file.
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
    }
}

void PhyloTreeWidget::handleListPosClicked(QListWidgetItem *item)
{
    QString sel = item->text();
    std::cout << sel.toStdString() << std::endl;
    int pos = sel.toInt() - 1;
    this->currIndex = pos;
    this->currTree = this->forest->at((size_t)this->currIndex);

    this->update();
}

void PhyloTreeWidget::Print_Tree_Qt(QPainter *pnt, edge *b_root, tdraw *w, arbre *tree)
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


void PhyloTreeWidget::Print_Tree_Pre_Qt(QPainter *pnt, node *a, node *d, tdraw *w, arbre *tree)
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
    bool highlight = false;
    if (!highlight)
        if (InSel(w->xcoord[a->num], w->ycoord[d->num] + BAR_HEIGHT, &this->rcSelection)) {
            //std::cout << "node selected" << std::endl;
            highlight = true;
        }
    path.moveTo(w->xcoord[a->num], w->ycoord[a->num] + BAR_HEIGHT);
    path.lineTo(w->xcoord[a->num], w->ycoord[d->num] + BAR_HEIGHT);
    if (highlight) {
        pnt->setPen(QColor(255, 255, 0));
        if(std::find(this->selNodes.begin(), this->selNodes.end(), d) == this->selNodes.end()) {
            // selNodes does not contain d
            // to store edges push a
            if (!this->mousePressed)
                this->selNodes.push_back(d);
        }
    }
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

void PhyloTreeWidget::TraversalAddPre(node *a, node *d, arbre *tree)
{
    int i;

    this->selNodes.push_back(d);

    if(d->tax) {
        return;
    }
    else {
        For(i,3)
                if(d->v[i] != a)
                TraversalAddPre(d, d->v[i], tree);
    }

    return;
}

void PhyloTreeWidget::TraversalAdd(edge *b_root, arbre *tree)
{
    int i;

    this->selNodes.push_back(b_root->left);
    this->selNodes.push_back(b_root->rght);

    if(!b_root->left->tax) {
        For(i,3)
                if((b_root->left->v[i]) && (b_root->left->v[i] != b_root->rght))
                TraversalAddPre(b_root->left, b_root->left->v[i], tree);
    }

    if(!b_root->rght->tax) {
        For(i,3)
                if((b_root->rght->v[i]) && (b_root->rght->v[i] != b_root->left))
                TraversalAddPre(b_root->rght, b_root->rght->v[i], tree);
    }

}


