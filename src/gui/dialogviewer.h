#ifndef DIALOGVIEWER_H
#define DIALOGVIEWER_H

#include <QDialog>

//#include "tree.h"
#include "fitmodeltreewrapper.h"

namespace Ui {
class DialogViewer;
}

class DialogViewer : public QDialog
{
    Q_OBJECT

public:
    explicit DialogViewer(QString filePath, QString statsPath, QWidget *parent = nullptr);
    ~DialogViewer();

private:
    Ui::DialogViewer *ui;
    QString path;
    //std::vector<Tree*> *trees;
    std::vector<FitModelTreeWrapper *> *trees;

protected:
    void paintEvent(QPaintEvent *);
};

#endif // DIALOGVIEWER_H
