#ifndef PHYLOTREEWIDGET_H
#define PHYLOTREEWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QStringListModel>

//#include <tree.h>
#include "fitmodeltreewrapper.h"

class PhyloTreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PhyloTreeWidget(QWidget *parent = nullptr);
    ~PhyloTreeWidget();
    //void SetForest(std::vector<Tree *> *forest);
    void SetName(QString name) {this->name = name;}
    void SetStatsPath(QString path) {this->statsPath = path;}
    void SetForest(std::vector<FitModelTreeWrapper *> *forest);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);

signals:

public slots:
    void handlePrevious();
    void handleNext();
    void handleSelWhole();
    void handleQuery();
    void handleSnippets();
    void handleSnapshot();
    void handleReport();
    void handleListPosClicked(QListWidgetItem *item);

private:
    QPushButton *prev;
    QPushButton *next;
    QPushButton *btnSelectWholeTree;
    QPushButton *btnQuery;
    QPushButton *btnSnippets;
    QPushButton *btnSnapshot;
    QLineEdit *lineQuery;
    QListWidget *listPos;
    QPushButton *btnReport;
    QString name;
    QString statsPath;
    std::vector<FitModelTreeWrapper *> *forest;
    FitModelTreeWrapper *currTree;
    int currIndex;
    bool mousePressed = false;
    QRectF rcSelection;
    std::vector<node *> selNodes;
    void Print_Tree_Qt(QPainter *pnt, edge *b_root, tdraw *w, arbre *tree);
    void Print_Tree_Pre_Qt(QPainter *pnt, node *a, node *d, tdraw *w, arbre *tree);
    void TraversalAdd(edge *b_root, arbre *tre);
    void TraversalAddPre(node *a, node *d, arbre *tree);
    std::vector<int> *positions;

};

#endif // PHYLOTREEWIDGET_H
