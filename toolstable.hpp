#ifndef TOOLSTABLE_HPP
#define TOOLSTABLE_HPP

#include <QWidget>
#include <QTableWidget>
#include <vector>
#include <QTextEdit>
#include <QtSql>
#include <map>

#include "sqlite3.h"

class ToolsTable : public QTableWidget
{
    Q_OBJECT
public:
    ToolsTable(sqlite3* db, QSqlQuery *q);
    void save_into_db();

private:
    sqlite3* db;
    int ok = 0;
    char *error_db;
    std::vector<QTextEdit*> column_name;
    size_t last_row = 0;
    QSqlQuery* q;
    std::vector<std::shared_ptr<QTableWidgetItem>> all;

    void pruf_db(int ok, char* error_db);
    static int callback_get_data(void* x, int column, char** data, char** col_name);
    void keyPressEvent(QKeyEvent* event) override;
    void counts();
    void reload();

signals:

};

#endif // TOOLSTABLE_HPP
