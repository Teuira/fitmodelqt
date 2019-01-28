#include "phylotreewidget.h"
#include "queryparser.h"
#include "dialogsnippets.h"
#include "msgboxes.h"
#include "reporter.h"

#include <algorithm>
#include <iostream>

#include <QPainter>
#include <QPushButton>
#include <QApplication>
#include <QFileDialog>
#include <QDate>
#include <QDesktopServices>

/*
 * START OF GLOBAL CONSTANTS.
 */
#define BAR_HEIGHT 40
#define TREE_VISUAL_WIDTH 800
#define TREE_VISUAL_HEIGHT 650
#define LABEL_X_OFFSET 10
#define LST_QUERY_X TREE_VISUAL_WIDTH
#define WIDGET_W (this->width())
#define RIGHT_BAR_W (WIDGET_W - TREE_VISUAL_W)

/*
 * END OF GLOBAL CONSTANTS;
 */

//**************************************************************************
//                          PUBLIC FUNCTIONS START
//**************************************************************************

//!
//! \brief InSel Checks if a point is in a selection rectangle.
//! \param x The x coord of the point to check.
//! \param y The y coord of the point to check.
//! \param rcSel The selection rectangle.
//! \return true if the point (x, y) is in the selection rectangle.
//!
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
        return true;
    }
    return false;
}

//!
//! \brief PhyloTreeWidget::PhyloTreeWidget
//! \param parent The parent widget
//! \details This is the constructor for the widget.
//! All widget creation and other initialisations are here.
//!
PhyloTreeWidget::PhyloTreeWidget(QWidget *parent) : QWidget(parent)
{
    /*
     * CONTROL CREATION START.
     */
    this->currTree = nullptr;
    this->prev = new QPushButton("<", this);
    this->next = new QPushButton(">", this);
    this->btnSelectWholeTree = new QPushButton("Whole tree", this);
    this->btnQuery = new QPushButton("Query", this);
    this->btnSnippets = new QPushButton("Snippets", this);
    this->lineQuery = new QLineEdit(this);
    this->lblQueryResList = new QLabel("Positions matching the query:", this);
    this->lblQueryResList->setStyleSheet("QLabel { color : white; }");
    this->listPos = new QListWidget(this);
    this->lblSaveList = new QLabel("Positions to save: ", this);
    this->btnAddPos = new QPushButton("Add", this);
    this->btnRemovePos = new QPushButton("Remove", this);
    this->lblSaveList->setStyleSheet("QLabel { color : white; }");
    this->linePos = new QLineEdit(this);
    this->listPosSave = new QListWidget(this);
    this->btnSnapshot = new QPushButton("Snapshot", this);
    this->btnReport = new QPushButton("Save Report", this);
    /*
     * CONTROL CREATION END.
     */
    /*
     * EVENT CONNECTION START.
     */
    connect(prev, SIGNAL (released()), this, SLOT (handlePrevious()));
    connect(next, SIGNAL (released()), this, SLOT (handleNext()));
    connect(btnSelectWholeTree, SIGNAL (released()), this, SLOT (handleSelWhole()));
    connect(btnQuery, SIGNAL (released()), this, SLOT (handleQuery()));
    connect(btnSnippets, SIGNAL (released()), this, SLOT (handleSnippets()));
    connect(btnSnapshot, SIGNAL (released()), this, SLOT (handleSnapshot()));
    connect(btnAddPos, SIGNAL (released()), this, SLOT (handleAddPos()));
    connect(btnRemovePos, SIGNAL (released()), this, SLOT (handleRemovePos()));
    connect(btnReport, SIGNAL (released()), this, SLOT (handleReport()));
    connect(this->listPos, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(handleListPosClicked(QListWidgetItem*)));
    connect(this->listPosSave, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(handleListPosClicked(QListWidgetItem*)));
    /*
     * EVENT CONNECTION END.
     */
    /*
     * INIT TREE VARS.
     */
    this->prev->setGeometry(120, 0, 30, 30);
    this->next->setGeometry(150, 0, 30, 30);
    this->btnSelectWholeTree->setGeometry(200, 0, 100, 30);
    this->btnSnapshot->setGeometry(320, 0, 100, 30);
    this->positions = new std::vector<int>;
    /*
     * END INIT TREE VARS.
     */
}

//!
//! \brief PhyloTreeWidget::~PhyloTreeWidget
//! \details Destructor for the widget. All the controls created in the constructor are deleted.
//!
PhyloTreeWidget::~PhyloTreeWidget()
{
    delete this->prev;
    delete this->next;
    delete this->btnSelectWholeTree;
    delete this->btnQuery;
    delete this->btnSnippets;
    delete this->btnAddPos;
    delete this->btnRemovePos;
    delete this->lineQuery;
    delete this->lblQueryResList;
    delete this->listPos;
    delete this->lblSaveList;
    delete this->linePos;
    delete this->listPosSave;
    delete this->btnSnapshot;
    delete this->positions;
    delete this->btnReport;
}

//!
//! \brief PhyloTreeWidget::SetForest
//! \details public method used to set the list of trees for this session.
//! \param forest The list of trees taken from FitModel.
//!
void PhyloTreeWidget::SetForest(std::vector<FitModelTreeWrapper *> *forest) {
    this->forest = forest;
    this->currIndex = 0;
    this->currTree = forest->at(this->currIndex);
    this->repaint();
}

//**************************************************************************
//                          PUBLIC FUNCTIONS END
//**************************************************************************

//!
//! \brief PhyloTreeWidget::mousePressEvent
//! \param event
//!
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

//!
//! \brief PhyloTreeWidget::mouseMoveEvent
//! \details When mouse moves it updates the size of the selection rectangle.
//! \param event
//!
void PhyloTreeWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->type() == QEvent::MouseMove)
        if (event->pos().y() < this->height() - 20)
            this->rcSelection.setBottomRight(event->pos());

    std::cout << "ypos: " << event->y() << std::endl;
    // refresh
    update();
}

//!
//! \brief PhyloTreeWidget::mouseReleaseEvent
//! \details On mouse released refresh and prints selected nodes.
//!
void PhyloTreeWidget::mouseReleaseEvent(QMouseEvent *)
{
    std::cout << "released" << std::endl;
    this->mousePressed = false;
    // refresh
    update();
    repaint();
    std::cout << "Selected nodes: " << this->selNodes.size() << std::endl;
}

//!
//! \brief PhyloTreeWidget::paintEvent
//! \details All of the painting events and widget resising happens here.
//!
void PhyloTreeWidget::paintEvent(QPaintEvent *)
{
    /*
     * SETS GEOMETRY FOR THE WIDGETS.
     */
    this->btnQuery->setGeometry(this->width() - 100, 0,
                                100, 30);
    this->lineQuery->setGeometry(440, 0,
                                 WIDGET_W - 680, 30);
    this->btnSnippets->setGeometry(this->lineQuery->width() + 460, 0,
                                   100, 30);
    this->lblQueryResList->setGeometry(LST_QUERY_X, BAR_HEIGHT + 10,
                                       150, 30);
    this->listPos->setGeometry(LST_QUERY_X, BAR_HEIGHT + 50, this->width() - TREE_VISUAL_WIDTH,
                               this->height() - BAR_HEIGHT - 500);
    this->lblSaveList->setGeometry(LST_QUERY_X, BAR_HEIGHT + 60 + this->listPos->height(),
                                   150, 30);
    this->listPosSave->setGeometry(LST_QUERY_X, this->lblSaveList->geometry().y() + 100,
                                   this->width() - TREE_VISUAL_WIDTH, this->height() - lblSaveList->geometry().y()-200);
    this->btnReport->setGeometry(LST_QUERY_X, this->height() - 80,
                                 this->listPos->geometry().width(), 30);
    int posEditY = this->lblSaveList->geometry().y() + 40;
    this->linePos->setGeometry(LST_QUERY_X + 2, posEditY,
                               WIDGET_W - TREE_VISUAL_WIDTH - 140, 30);
    this->btnAddPos->setGeometry(LST_QUERY_X + this->linePos->geometry().width() + 10, posEditY,
                                 60, 30);
    this->btnRemovePos->setGeometry(this->btnAddPos->geometry().x() + 70, posEditY,
                                 60, 30);
    /*
     * END SETTING GEOMETRY.
     */
    QPainter painter(this);
    /*
    * START DRAWING BACKGROUND.
    */
    painter.fillRect(QRectF(0.0, 0.0, this->width(), this->height()), QColor(0,0,0));
    painter.fillRect(QRectF(TREE_VISUAL_WIDTH, 0, this->width() - TREE_VISUAL_WIDTH, this->height()), QColor(0, 32, 96));
    painter.fillRect(QRectF(0.0, 0.0, this->width(), 31), QColor(79, 129, 189));
    painter.fillRect(QRectF(0.0, this->height() - 20, this->width(), 20), QColor(79, 129, 189));
    /*
     * END DRAWING BACKGROUND.
     */
    if (this->mousePressed) {
        painter.setPen(Qt::white);
        painter.drawRect(this->rcSelection);
    }
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
    /*
     * START DRAWING BOTTOM CURSOR.
     */
    painter.drawLine(0, this->height() - 10, this->width(), this->height() - 10);
    painter.setPen(Qt::white);
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
    /*
     * END DRAWING BOTTOM CURSOR.
     */
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
    infoBox("Whole tree selected.");
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
        infoBox("Select some nodes first.");
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

//!
//! \brief PhyloTreeWidget::handleSnippets
//! \details Opens the snippets window.
//!
void PhyloTreeWidget::handleSnippets()
{
    std::cout << "Snippets requested" << std::endl;
    DialogSnippets dlgSnippets;
    dlgSnippets.setModal(true);
    if (dlgSnippets.exec() == QDialog::DialogCode::Accepted) {
        this->lineQuery->setText(QString::fromStdString(dlgSnippets.snippet));
        if (dlgSnippets.snippet == "") {
            handleQuery();
        }
    }
    std::cout << "comeback to viewer" << std::endl;
}

//!
//! \brief PhyloTreeWidget::handleAddPos
//! \details Adds a position to the save list only if the position is not empty
//! and if the position isn't already in the list.
//!
void PhyloTreeWidget::handleAddPos()
{
    QString itm = linePos->text();
    if (itm.isEmpty()) {
        warningBox("Enter a position first!");
        return;
    }
    bool ok;
    int pos = itm.toInt(&ok, 10);
    if (!ok) {
        warningBox("Enter a valid position!");
        return;
    }
    if (pos < 1 || pos > this->forest->size()) {
        warningBox("Enter a valid position!");
        this->linePos->clear();
        return;
    }
    for (int i = 0; i < this->listPosSave->count(); ++i)
    {
        QListWidgetItem* currItem = this->listPosSave->item(i);
        if (currItem->text() == itm) {
            infoBox("This position is already in the save list!");
            return;
        }
    }
    this->listPosSave->addItem(itm);
    this->linePos->clear();
}

//!
//! \brief PhyloTreeWidget::handleRemovePos
//! \details Removes a position from the save list.
//!
void PhyloTreeWidget::handleRemovePos()
{
    QString itm = linePos->text();
    if (itm.isEmpty()) {
        warningBox("Enter a position first!");
        return;
    }
    for (int i = 0; i < this->listPosSave->count(); ++i)
    {
        QListWidgetItem* currItem = this->listPosSave->item(i);
        if (currItem->text() == itm) {
            delete currItem;
            this->linePos->clear();
            return;
        }
    }
    warningBox("This position is not in the save list!");
}

//!
//! \brief PhyloTreeWidget::handleSnapshot
//! \details Takes a snapshot of the current window and asks the user where to save it.
//!
void PhyloTreeWidget::handleSnapshot()
{
    std::cout << "Snapshot requested" << std::endl;
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save snapshot"), "", tr("PNG file (*.png)"));
    if (!fileName.endsWith(".png"))
        fileName += ".png";
    if (fileName != ".png") {
        this->grab().save(fileName);
        infoBox("Image saved!");
    }
}

//!
//! \brief PhyloTreeWidget::handleReport
//!
void PhyloTreeWidget::handleReport()
{
    std::cout << "Report requested" << std::endl;
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save report"), "", tr("PDF file (*.pdf)"));
    if (!fileName.endsWith(".pdf"))
        fileName += ".pdf";
    if (fileName != "") {
        Reporter rep(this->name, this->statsPath, this->forest, this->listPosSave);
        rep.save(fileName);
        // End of report.
        infoBox("Report saved!");
        // Opens file.
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
    }
}

//!
//! \brief PhyloTreeWidget::handleListPosClicked On position clicked.
//! \details On click goes to the selected position.
//! \param item The selected item.
//!
void PhyloTreeWidget::handleListPosClicked(QListWidgetItem *item)
{
    QString sel = item->text();
    std::cout << sel.toStdString() << std::endl;
    int pos = sel.toInt() - 1;
    this->currIndex = pos;
    this->currTree = this->forest->at((size_t)this->currIndex);
    this->linePos->setText(sel);

    this->update();
}

//!
//! \brief PhyloTreeWidget::Print_Tree_Qt
//! \param pnt
//! \param b_root
//! \param w
//! \param tree
//!
void PhyloTreeWidget::Print_Tree_Qt(QPainter *pnt, edge *b_root, tdraw *w, arbre *tree)
{
    int i;
    double max,min,step;

    min = 0.0;
    max = 1.0;

    step = (max-min)/13.;

    if(b_root->prob_sel_regime <= min+1.*step)
        pnt->setPen(QColor(26, /*26*/152, 255));
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
            pnt->setPen(QColor(26, /*26*/152, 255));
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


