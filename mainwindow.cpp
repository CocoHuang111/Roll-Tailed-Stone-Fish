#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->login_button->setStyleSheet(R"(
    QPushButton {
        background-color: rgb(93, 166, 255);
        color: white;
        border-color: rgb(255, 255, 255);
        border-radius: 20px;
        border: 3px solid white;
    }
    QPushButton:hover {
        background-color: rgb(93, 166, 255);
    }
    QPushButton:pressed {
        background-color: rgb(93, 166, 255);
    }
)");
    ui->search_button->setStyleSheet(R"(
    QPushButton {
        background-color: rgba(73, 150, 255, 158);
        color: white;
        border-color: rgb(255, 255, 255);
        border-radius: 20px;
        border: 3px solid white;
    }
    QPushButton:hover {
        background-color: rgb(93, 166, 255);
    }
    QPushButton:pressed {
        background-color: rgb(93, 166, 255);
    }
)");
    ui->chat_button->setStyleSheet(R"(
    QPushButton {
        background-color: rgba(73, 150, 255, 158);
        color: white;
        border-color: rgb(255, 255, 255);
        border-radius: 20px;
        border: 3px solid white;
    }
    QPushButton:hover {
        background-color: rgb(93, 166, 255);
    }
    QPushButton:pressed {
        background-color: rgb(93, 166, 255);
    }
)");
    ui->award_button->setStyleSheet(R"(
    QPushButton {
        background-color: rgba(73, 150, 255, 158);
        color: white;
        border-color: rgb(255, 255, 255);
        border-radius: 20px;
        border: 3px solid white;
    }
    QPushButton:hover {
        background-color: rgb(93, 166, 255);
    }
    QPushButton:pressed {
        background-color: rgb(93, 166, 255);
    }
)");
    ui->adddemand_button->setStyleSheet(R"(
    QPushButton {
        background-color: rgba(73, 150, 255, 158);
        color: white;
        border-color: rgb(255, 255, 255);
        border-radius: 20px;
        border: 3px solid white;
    }
    QPushButton:hover {
        background-color: rgb(93, 166, 255);
    }
    QPushButton:pressed {
        background-color: rgb(93, 166, 255);
    }
)");
    ui->addbook_button->setStyleSheet(R"(
    QPushButton {
        background-color: rgba(73, 150, 255, 158);
        color: white;
        border-color: rgb(255, 255, 255);
        border-radius: 20px;
        border: 3px solid white;
    }
    QPushButton:hover {
        background-color: rgb(93, 166, 255);
    }
    QPushButton:pressed {
        background-color: rgb(93, 166, 255);
    }
)");
    ui->mypage_button->setStyleSheet(R"(
    QPushButton {
        background-color: rgba(73, 150, 255, 158);
        color: white;
        border-color: rgb(255, 255, 255);
        border-radius: 20px;
        border: 3px solid white;
    }
    QPushButton:hover {
        background-color: rgb(93, 166, 255);
    }
    QPushButton:pressed {
        background-color: rgb(93, 166, 255);
    }
)");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *) {

    QPainter painter(this);
    QPixmap pix("/Users/huangkefan/Desktop/background.jpg"); // 使用资源文件
    painter.drawPixmap(this->rect(), pix);  // 拉伸绘制
}

void MainWindow::on_search_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_login_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_readytologin_button_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

