#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    QPixmap pixmap(":/images/img1.png");
    QIcon ButtonIcon(pixmap);
    ui->button1->setIcon(ButtonIcon);
    ui->button1->setIconSize(pixmap.rect().size());

    QPixmap pixmap2(":images/img2.png");
    QIcon ButtonIcon2(pixmap2);
    ui->button2->setIcon(ButtonIcon2);
    ui->button2->setIconSize(pixmap2.rect().size());

    QPixmap pixmap3(":images/img3.png");
    QIcon ButtonIcon3(pixmap3);
    ui->button3->setIcon(ButtonIcon3);
    ui->button3->setIconSize(pixmap3.rect().size());

    QPixmap pixmap4(":images/img4.png");
    QIcon ButtonIcon4(pixmap4);
    ui->button4->setIcon(ButtonIcon4);
    ui->button4->setIconSize(pixmap4.rect().size());
}

MainWindow::~MainWindow(){
    delete ui;
}



