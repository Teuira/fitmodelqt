#ifndef DIALOGLAUNCHER_H
#define DIALOGLAUNCHER_H

#include <QDialog>

namespace Ui {
class DialogLauncher;
}

class DialogLauncher : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLauncher(QWidget *parent = nullptr);
    ~DialogLauncher();

private slots:
    void on_btnBrowseSeq_clicked();

    void on_btnBrowseTree_clicked();

    void on_bntRun_clicked();

private:
    Ui::DialogLauncher *ui;
};

#endif // DIALOGLAUNCHER_H
