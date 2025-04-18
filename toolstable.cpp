#include "toolstable.hpp"
#include <iostream>
#include <QKeyEvent>
#include <QMessageBox>

ToolsTable::ToolsTable(sqlite3 *db) : db{db}
{
    setWindowTitle("Инструментальная база");
    setWindowIcon(QIcon(":/Images/user.svg"));
    setFixedWidth(670);
    setMinimumHeight(400);
    setColumnCount(4);
    setColumnWidth(0, 100);
    setColumnWidth(1, 220);
    setColumnWidth(2, 80);
    setColumnWidth(3, 250);
    setHorizontalHeaderLabels(QStringList() << "Идентификатор" << "Название" <<
                              "Количесвто" << "Изображение");

    ok = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS tools "
                    "(row INTEGER UNIQUE, id TEXT, name TEXT, "
                    "count TEXT, image TEXT)", nullptr, nullptr, &error_db);
    pruf_db(ok, error_db);
    ok = sqlite3_exec(db, "SELECT * FROM tools", callback_get_data, this, &error_db);
    pruf_db(ok, error_db);
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
        QTableWidgetItem* item;
        if (data[i] != NULL) {
            item = new QTableWidgetItem(QString(data[i]));
        }
        else {
            item = new QTableWidgetItem(" ");
        }
        t->setItem(t->last_row, i-1, item);
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
        setRowCount(rowCount()+1);
        for (size_t i = 0; i<columnCount(); ++i) {
            QTableWidgetItem* item = new QTableWidgetItem(" ");
            setItem(rowCount()-1, i, item);
        }
    }
    else if (event->key() == Qt::Key_Minus && rowCount() != last_row) {
        setRowCount(rowCount()-1);
    }
    else if (event->key() == Qt::Key_S && (event->modifiers() & Qt::ControlModifier)) {
        save_into_db();
    }
    else {
        QTableWidget::keyPressEvent(event);
    }
}
