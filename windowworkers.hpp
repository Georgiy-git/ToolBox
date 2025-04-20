#ifndef WINDOWWORKERS_HPP
#define WINDOWWORKERS_HPP

#include <QMainWindow>
#include <QTextEdit>
#include <chrono>
#include <vector>
#include <string>
#include <QPushButton>
#include <memory>
#include <QVBoxLayout>
#include <iostream>
#include <QtSql>
#include <set>
#include <list>
#include "sqlite3.h"

namespace Ui {
class WindowWorkers;
}

class ToolsTable;
class LineTool;
class Unit;

class WindowWorkers : public QMainWindow
{
    Q_OBJECT

public:
    WindowWorkers();
    ~WindowWorkers();

private slots:
    void on_action_3_triggered(); //Новый юнит

    void on_pushButton_clicked(); // Записать инструмент

    void on_pushButton_2_clicked(); //Закрыть окно юнита

    void on_action_triggered(); //Инструментальная база

    void on_lineEdit_textChanged(const QString &str); //Поиск юнита

    void on_action_4_triggered(); //Удаление юнита

private:
    //Разное
    Ui::WindowWorkers *ui;
    QVBoxLayout* units_layout;
    QVBoxLayout* tools_layout;
    ToolsTable* toolstable = nullptr;
    std::list<std::unique_ptr<LineTool>> lines_tool;
    void closeEvent(QCloseEvent* event) override;

    //Юнит
    int last_id = 0;
    Unit* this_unit;
    std::vector<std::shared_ptr<Unit>> units;
    void add_unit_get();
    void on_unit_clicked(Unit *unit); //Нажатие на кнопку юнита

    //Окно добавления юнита
    QWidget* add_unit = nullptr;
    QTextEdit* add_unit_text = nullptr;
    bool add_unit_show = false;

    //БД
    sqlite3* db;
    QSqlDatabase qdb;
    QSqlQuery* q;
    int db_ok = 0;
    char* error_db;
    bool ok = true;
    static void pruf_db(int db_ok, char *error_db);
    void pruf(bool ok);
    static int callback_db_units(void* x, int column, char** data, char** col_name);

    //Стили кнопки
    const QString blue_button_style = "QPushButton { background-color: blue; border: none; "
                        "color: white; margin: 1px 1px; border-radius: 5px; } "
                        "QPushButton:hover { background-color: lightblue; } "
                        "QPushButton:pressed { background-color: #21618C; }";
    const QString red_button_style = "QPushButton { background-color: red; border: none; "
                        "color: white; margin: 1px 1px; border-radius: 5px; } "
                        "QPushButton:hover { background-color: #FF7F7F; } "
                        "QPushButton:pressed { background-color: #8B0000; }";


    //Окно удаления юнита
    bool del_unit = false;
    QWidget *del_unit_window;

    //Показ окна юнита
    bool card_visible = false;
    void setCardVisible(bool);
};

#endif // WINDOWWORKERS_HPP
