
#include "msgboxes.h"

#include <QMessageBox>

void warningBox(QString msg)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.exec();
    return;
}

void infoBox(QString msg)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
    return;
}

void errorBox(QString msg)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
    return;
}
