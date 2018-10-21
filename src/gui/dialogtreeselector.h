#ifndef DIALOGTREESELECTOR_H
#define DIALOGTREESELECTOR_H

#include <QDialog>

namespace Ui {
class DialogTreeSelector;
}

class DialogTreeSelector : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTreeSelector(QWidget *parent = nullptr);
    ~DialogTreeSelector();

private slots:
    void on_btnBrowseTree_clicked();
    void on_btnNext_clicked();

private:
    Ui::DialogTreeSelector *ui;

};

#endif // DIALOGTREESELECTOR_H
