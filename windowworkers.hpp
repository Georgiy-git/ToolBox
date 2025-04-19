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

class WindowWorkers : public QMainWindow
{
    Q_OBJECT

public:
    explicit WindowWorkers();
    ~WindowWorkers();

    QWidget* add_unit = nullptr;
    QTextEdit* add_unit_text = nullptr;
    bool add_unit_show = false;

private slots:
    void on_action_3_triggered(); //Новый юнит

    void on_pushButton_clicked(); // Записать инструмент

    void on_pushButton_2_clicked();

    void on_action_triggered();

    void on_lineEdit_textChanged(const QString &str);

    void on_action_4_triggered();

private:
    Ui::WindowWorkers *ui;
    bool card_visible = false;
    sqlite3* db;
    QSqlDatabase qdb;
    QSqlQuery* q;
    int db_ok = 0;
    char* error_db;
    std::vector<std::shared_ptr<QPushButton>> unit_buttons;
    unsigned int last_id = 0;
    const QString blue_button_style = "QPushButton { background-color: blue; border: none; "
                        "color: white; margin: 1px 1px; border-radius: 5px; } "
                        "QPushButton:hover { background-color: lightblue; } "
                        "QPushButton:pressed { background-color: #21618C; }";
    const QString red_button_style = "QPushButton { background-color: red; border: none; "
                        "color: white; margin: 1px 1px; border-radius: 5px; } "
                        "QPushButton:hover { background-color: #FF7F7F; } "
                        "QPushButton:pressed { background-color: #8B0000; }";
    QVBoxLayout* units_layout;
    QVBoxLayout* tools_layout;
    ToolsTable* toolstable = nullptr;
    std::set<std::vector<QString>> idfio;
    std::vector<QString> this_idfio;
    bool ok = true;
    std::list<std::unique_ptr<LineTool>> lines_tool;
    QPushButton* this_button;
    bool del_unit = false;
    QWidget *del_unit_window;

    void setCardVisible(bool);
    void add_unit_get();
    static void pruf_db(int db_ok, char *error_db);
    void pruf(bool ok);
    static int callback_db_units(void* x, int column, char** data, char** col_name);
    void on_unit_clicked(std::vector<QString> strs); //Нажатие на кнопку юнита
    void closeEvent(QCloseEvent* event) override;
};

#endif // WINDOWWORKERS_HPP
