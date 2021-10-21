#ifndef GOLBAL_H
#define GOLBAL_H

#include <QMainWindow>
#include <QTextBrowser>

class values{
public:
    QString SelectFile;
    QString SaveFile;
    bool isFileSelected=false;
    bool isUnzip=false;
    bool isOutdirSelected=false;
    QList<QString> fileName;
    int count=1;

};
#endif // GOLBAL_H
