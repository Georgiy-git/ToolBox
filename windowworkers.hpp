#ifndef WINDOWWORKERS_HPP
#define WINDOWWORKERS_HPP

#include <QMainWindow>
#include <QTextEdit>

#include "sqlite3.h"

namespace Ui {
class WindowWorkers;
}

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
    void on_action_3_triggered();

    void on_pushButton_clicked();

private:
    Ui::WindowWorkers *ui;
    bool card_visible = false;
    sqlite3* db;
    int db_ok = 0;
    char* error_db;

    void setCardVisible(bool);
    void add_unit_get();
    void pruf_db(int db_ok, char *error_db);
};

#endif // WINDOWWORKERS_HPP
