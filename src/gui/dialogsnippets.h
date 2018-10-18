#ifndef DIALOGSNIPPETS_H
#define DIALOGSNIPPETS_H

#include <QDialog>

namespace Ui {
class DialogSnippets;
}

class DialogSnippets : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSnippets(QWidget *parent = nullptr);
    ~DialogSnippets();
    std::string snippet;

private slots:
    void on_btnEq_clicked();
    void on_btnDiff_clicked();
    void on_btnSim_clicked();

private:
    Ui::DialogSnippets *ui;
};

#endif // DIALOGSNIPPETS_H
