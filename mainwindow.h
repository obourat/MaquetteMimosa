#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class AllListing;
class Dialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_quitButton_released();
    void on_completeListButton_released();

private:
    Ui::MainWindow *ui;
    MainWindow *newMain;
    AllListing *allListing;

};

#endif // MAINWINDOW_H
