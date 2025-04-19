#include "linetool.hpp"

#include <QMessageBox>

LineTool::LineTool(QString _tool_name, QString _date, QSqlQuery *q, QString unit_id)
    : _tool_name{_tool_name}, _date{_date}, q{q}, unit_id{unit_id}
{
    layout = std::make_shared<QHBoxLayout>();
    setLayout(layout.get());
    gave = std::make_shared<QPushButton>();
    gave->setText("Вернуть");
    gave->setFixedSize(100, 25);
    gave->setStyleSheet("QPushButton { background-color: green; border: none; "
                        "color: white; margin: 1px 1px; border-radius: 5px; } "
                        "QPushButton:hover { background-color: lightgreen; } ");
    connect(gave.get(), &QPushButton::clicked, this, refund);
    tool_name = std::make_shared<QLabel>(_tool_name);
    tool_name->setStyleSheet("font-weight: bold; font-size: 16px;");
    date = std::make_shared<QLabel>(_date);
    layout->addWidget(tool_name.get());
    layout->addWidget(date.get());
    layout->addWidget(gave.get());
}

void LineTool::refund()
{
    if (!q->exec("DELETE FROM unit_tools WHERE id = "+unit_id+" AND time = '"+_date+"'")) {
       QMessageBox::critical(nullptr, QString("Ошибка базы данных"), q->lastError().text());
    } else {
        emit signal_click();
        my_list->erase(me);
    }
}
