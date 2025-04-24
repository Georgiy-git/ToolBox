#ifndef SUBTOOLSTABLE_HPP
#define SUBTOOLSTABLE_HPP

#include <QTableWidget>
#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <vector>
#include <QTextEdit>
#include <QtSql>

class SubToolsTable : public QTableWidget
{
    Q_OBJECT
public:
    SubToolsTable(QSqlQuery *q, QString table_name);
    void save_into_db();

private:
    QString table_name;
    bool qok = true;
    std::vector<QTextEdit*> column_name;
    size_t last_row = 0;
    QSqlQuery* q;
    std::vector<std::shared_ptr<QTableWidgetItem>> all;

    void set_table();
    void pruf(bool ok);
    void keyPressEvent(QKeyEvent* event) override;
    void counts();
    void reload();
};

#endif // SUBTOOLSTABLE_HPP
