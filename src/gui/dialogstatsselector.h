#ifndef DIALOGSTATSSELECTOR_H
#define DIALOGSTATSSELECTOR_H

#include <QDialog>

namespace Ui {
class DialogStatsSelector;
}

class DialogStatsSelector : public QDialog
{
    Q_OBJECT

public:
    explicit DialogStatsSelector(QString treePath, QWidget *parent = nullptr);
    ~DialogStatsSelector();

private slots:
    void on_btnNext_clicked();

    void on_btnBrowseStats_clicked();

private:
    Ui::DialogStatsSelector *ui;
    QString strTreePath;

};

#endif // DIALOGSTATSSELECTOR_H
