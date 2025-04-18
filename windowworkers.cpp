#include "windowworkers.hpp"
#include "ui_windowworkers.h"

#include <QVBoxLayout>
#include <iostream>

WindowWorkers::WindowWorkers() : ui(new Ui::WindowWorkers)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
    setWindowTitle("ToolBox");
    setWindowIcon(QIcon(":/Images/mine.svg"));
    ui->lineEdit->setPlaceholderText("Поиск");
    setCardVisible(false);
    db_ok = sqlite3_open("toolbox.db", &db);
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS units (INTAGER PRIMARY KEY AUTOINKREMENT id,"
                     " name TEXT)", nullptr, nullptr, &error_db);

    //Добавление юнитов
    add_unit = new QWidget(nullptr, Qt::WindowCloseButtonHint);
    add_unit->setFixedWidth(300);
    add_unit->setMaximumHeight(300);
    add_unit->setMaximumHeight(600);
    add_unit->setWindowTitle(" ");
    add_unit->setWindowIcon(QIcon(":/Images/mine.svg"));
    QVBoxLayout *add_unit_layout = new QVBoxLayout(add_unit);
    add_unit_text = new QTextEdit(add_unit);
    add_unit_text->setPlaceholderText("Введите ФИО юнита(-ов)");
    QPushButton* add_unit_button = new QPushButton(add_unit);
    add_unit_button->setText("Добавить");
    add_unit_button->setStyleSheet("QPushButton { background-color: blue; border: none; "
                                   "color: white; margin: 1px 1px; border-radius: 5px; } "
                                   "QPushButton:hover { background-color: lightblue; } "
                                   "QPushButton:pressed { background-color: #21618C; }");
    add_unit_button->setFixedSize(100, 35);
    connect(add_unit_button, &QPushButton::clicked, this, add_unit_get);
    add_unit_layout->addWidget(add_unit_text);
    add_unit_layout->addWidget(add_unit_button, 0, Qt::AlignHCenter);
    add_unit->setLayout(add_unit_layout);
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

void WindowWorkers::add_unit_get()
{
    QString text = add_unit_text->toPlainText();

}

void WindowWorkers::pruf_db(int db_ok, char* error_db)
{
    if (db_ok) {
        std::cerr << "Ошибка sqlite: " << error_db;
    }
}

void WindowWorkers::on_action_3_triggered()
{
    if (!add_unit_show) {
        add_unit->show();
        add_unit_show = true;
    }
    else {
        add_unit->hide();
        add_unit->show();
    }
}


void WindowWorkers::on_pushButton_clicked()
{
    if (card_visible) {

    }
}

