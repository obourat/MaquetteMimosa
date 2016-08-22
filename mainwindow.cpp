#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QDebug>

#include "search.h"
#include "alllisting.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this); //Affichage de la fenêtre
}

MainWindow::~MainWindow()
{
    delete ui;
}

//On clique sur Quitter
void MainWindow::on_quitButton_released()
{
    close();
}


void MainWindow::on_completeListButton_released()
{
    allListing = new AllListing(); //On crée un nouvel objet dialog de la classe Dialog
    allListing->exec(); //On execute le listing
}
