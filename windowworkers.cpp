#include "windowworkers.hpp"
#include "ui_windowworkers.h"

WindowWorkers::WindowWorkers() : ui(new Ui::WindowWorkers)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
    setWindowTitle("ToolBox");
    setWindowIcon(QIcon(":/Images/mine.svg"));
    ui->lineEdit->setPlaceholderText("Поиск");

    setCardVisible(false);
}

WindowWorkers::~WindowWorkers()
{
    delete ui;
}

void WindowWorkers::setCardVisible(bool x)
{
    if (!x) {
        ui->label_2->setVisible(0);
        ui->label_3->setVisible(0);
        ui->scrollArea_2->setVisible(0);
        ui->lineEdit_2->setVisible(0);
        ui->pushButton->setVisible(0);
    } else {
        ui->label_2->setVisible(true);
        ui->label_3->setVisible(true);
        ui->scrollArea_2->setVisible(true);
        ui->lineEdit_2->setVisible(true);
        ui->pushButton->setVisible(true);
    }
}

void WindowWorkers::on_action_3_triggered()
{
    setCardVisible(true);
}


void WindowWorkers::on_pushButton_clicked()
{
    if (card_visible) {

    }
}

