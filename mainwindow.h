#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "golbal.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_selectFile_clicked();

    void on_pushButton_clicked();

    void on_clearcache_clicked();

    void on_readfile_clicked();

    void on_outDir_clicked();

    void on_openout_clicked();

    void on_analize_clicked();

    void on_clearout_clicked();

    void on_predo_clicked();

    void on_reset_clicked();

    void on_H1_clicked();

    void on_H2_clicked();

    void on_H3_clicked();

    void on_H4_clicked();

    void on_H5_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    values golbal;
};
#endif // MAINWINDOW_H
