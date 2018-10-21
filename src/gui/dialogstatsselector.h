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

private:
    Ui::DialogStatsSelector *ui;
};

#endif // DIALOGSTATSSELECTOR_H
