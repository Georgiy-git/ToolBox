#include "subtoolstable.hpp"

#include <QMessageBox>
#include <QKeyEvent>

#define COLUMN_COUNT 4

SubToolsTable::SubToolsTable(QSqlQuery *q, QString table_name)
    : q{q}, table_name{table_name}
{
    setWindowTitle(table_name);
    setWindowIcon(QIcon(":/Images/DataBase.svg"));
    setFixedWidth(500);
    setMinimumHeight(400);
    setColumnCount(COLUMN_COUNT);
    setColumnWidth(0, 100);
    setColumnWidth(1, 220);
    setColumnWidth(2, 80);
    setColumnWidth(3, 80);
    setHorizontalHeaderLabels(QStringList() << "Идентификатор" << "Название" <<
                              "Количесвто" << "На складе");
    qok = q->exec("CREATE TABLE IF NOT EXISTS "+table_name+
                  " (row INTEGER UNIQUE, id TEXT)");
    pruf(qok);

    set_table();
}

void SubToolsTable::save_into_db()
{
    for (size_t i = 0; i<rowCount(); ++i) {

        QString sql = "REPLACE INTO tools (row, id) VALUES (";
        sql += QString::number(i) + ", ";
        if (item(i, 0)->text().trimmed().isEmpty()) {
            sql += "NULL, ";
        }
        else {
            sql += "'" + item(i, 0)->text() + "', ";
        }
        sql.erase(sql.end()-1);
        sql.erase(sql.end()-1);
        sql+= ")";
        qok = q->exec(sql);
        pruf(qok);
    }
}

void SubToolsTable::set_table()
{
    qok = q->exec("SELECT * FROM "+table_name);
    pruf(qok);
    int row = 0;
    while(q->next()) {
        std::shared_ptr<QTableWidgetItem> item;
        if (!q->value(1).toString().isEmpty()) {
            item = std::make_shared<QTableWidgetItem>(q->value(1).toString());
        }
        else {
            item = std::make_shared<QTableWidgetItem>(" ");
        }
        setItem(row, 0, item.get());
        row++;
    }
}

void SubToolsTable::pruf(bool ok)
{
    if (!ok) {
        QMessageBox::information(nullptr,
            QString("Ошибка в базе данных"), q->lastError().text());
    }
}

void SubToolsTable::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Equal) {

        setRowCount(rowCount()+1);
        for (size_t i = 0; i<COLUMN_COUNT; ++i) {
            std::shared_ptr<QTableWidgetItem> item = std::make_shared<QTableWidgetItem>(" ");
            setItem(rowCount()-1, i, item.get());
            all.push_back(item);
        }
    }
    else if (event->key() == Qt::Key_Minus && rowCount() != last_row) {
        for (int i = 0; i < COLUMN_COUNT-1; ++i) { all.pop_back(); }
        setRowCount(rowCount()-1);
    }
    else if (event->key() == Qt::Key_S && (event->modifiers() & Qt::ControlModifier)) {
        save_into_db();
    }
    else if (event->key() == Qt::Key_R || event->key() == Qt::Key_1) {
        reload();
    }
    else {
        QTableWidget::keyPressEvent(event);
    }
}

void SubToolsTable::counts()
{
    q->exec("SELECT name, count FROM tools");
    std::vector<std::pair<QString, int>> m;
    size_t row = 0;
    while (q->next()) {
        m.push_back({q->value(0).toString(), q->value(1).toInt()});
    }
    size_t num = 3;

    for (auto& i : m) {
        if (i.second != 0) {
        size_t s = 0;
        q->exec("SELECT tool FROM unit_tools WHERE tool = '"+i.first+"'");
        while (q->next()) { ++s; }
        all[num] = std::make_shared<QTableWidgetItem>(QString::number(i.second-s)+" шт");
        if (s != 0) { all[num]->setBackground(QBrush(QColor(255, 0, 0)));
                                all[num]->setForeground(QBrush(QColor(255, 255, 255))); }
        setItem(row, 3, all[num].get());
        ++row;
        num += COLUMN_COUNT;
        }
    }
}

void SubToolsTable::reload()
{
    save_into_db();
    all.clear();
    set_table();
}
