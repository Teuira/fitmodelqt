#ifndef REPORTER_H
#define REPORTER_H

#include "fitmodeltreewrapper.h"
#include <QListWidget>
#include <QString>

class Reporter
{
public:
    Reporter(QString fileName,
             QString statsPath,
             std::vector<FitModelTreeWrapper *> *forest,
             QListWidget *positions);
    void save(QString fileOut);

private:
    QString fileName;
    QString statsPath;
    std::vector<FitModelTreeWrapper *> *forest;
    QListWidget *positions;
};

#endif // REPORTER_H
