#include "toolstable.hpp"
#include <iostream>
#include <QKeyEvent>
#include <QMessageBox>

ToolsTable::ToolsTable(sqlite3 *db, QSqlQuery* q) : db{db}, q{q}
{
    setWindowTitle("Инструментальная база");
    setWindowIcon(QIcon(":/Images/user.svg"));
    setFixedWidth(750);
    setMinimumHeight(400);
    setColumnCount(5);
    setColumnWidth(0, 100);
    setColumnWidth(1, 220);
    setColumnWidth(2, 80);
    setColumnWidth(3, 80);
    setColumnWidth(4, 250);
    setHorizontalHeaderLabels(QStringList() << "Идентификатор" << "Название" <<
                              "Количесвто" << "На складе" << "Изображение");

    ok = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS tools "
                    "(row INTEGER UNIQUE, id TEXT, name TEXT, "
                    "count TEXT, image TEXT)", nullptr, nullptr, &error_db);
    pruf_db(ok, error_db);
    ok = sqlite3_exec(db, "SELECT * FROM tools", callback_get_data, this, &error_db);
    pruf_db(ok, error_db);
    counts();
}

void ToolsTable::pruf_db(int ok, char *error_db)
{
    if (ok) {
        QMessageBox::information(nullptr,
            QString("Ошибка в базе данных"), QString(error_db));
    }
}

int ToolsTable::callback_get_data(void *x, int column, char **data, char **col_name)
{
    ToolsTable* t = static_cast<ToolsTable*>(x);
    t->setRowCount(t->rowCount()+1);
    for (size_t i = 1; i<5; ++i) {
        std::shared_ptr<QTableWidgetItem> item;
        if (data[i] != NULL) {
            item = std::make_shared<QTableWidgetItem>(QString(data[i]));
        }
        else {
            item = std::make_shared<QTableWidgetItem>(" ");
        }
        if (i != 4) {
            t->setItem(t->last_row, i-1, item.get());
            t->all.push_back(item);
        }
        else {
            t->all.push_back(std::make_shared<QTableWidgetItem>());
            t->setItem(t->last_row, i, item.get());
            t->all.push_back(item);
        }
    }
    t->last_row++;
    return 0;
}

void ToolsTable::save_into_db()
{
    for (size_t i = 0; i<rowCount(); ++i) {
        QString sql = "REPLACE INTO tools (row, id, name, count, image) VALUES (";
        sql += QString::number(i) + ", ";
        for (size_t j = 0; j<columnCount(); ++j) {
            if (j == 3) {++j;}
            if (item(i, j)->text().trimmed().isEmpty()) {
                sql += "NULL, ";
            }
            else {
                sql += "'" + item(i, j)->text() + "', ";
            }
        }
        sql.erase(sql.end()-1);
        sql.erase(sql.end()-1);
        sql+= ")";
        ok = sqlite3_exec(db, sql.toStdString().c_str(), nullptr, nullptr, &error_db);
        pruf_db(ok, error_db);
    }
}

void ToolsTable::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Equal) {
        int id = item(rowCount()-1, 0)->text().toInt()+1;
        setRowCount(rowCount()+1);
        for (size_t i = 1; i<columnCount(); ++i) {
            std::shared_ptr<QTableWidgetItem> item = std::make_shared<QTableWidgetItem>(" ");
            setItem(rowCount()-1, i, item.get());
            all.push_back(item);
        }
        std::shared_ptr<QTableWidgetItem> item = std::make_shared<QTableWidgetItem>(QString::number(id));
        setItem(rowCount()-1, 0, item.get());
        all.push_back(item);
    }
    else if (event->key() == Qt::Key_Minus && rowCount() != last_row) {
        setRowCount(rowCount()-1);
        for (int i = 0; i < columnCount(); ++i) { all.pop_back(); }
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

void ToolsTable::counts()
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
        num += 5;
        }
    }
}

void ToolsTable::reload()
{
    last_row = 0;
    all.clear();
    setRowCount(0);
    ok = sqlite3_exec(db, "SELECT * FROM tools", callback_get_data, this, &error_db);
    pruf_db(ok, error_db);
    counts();
}
