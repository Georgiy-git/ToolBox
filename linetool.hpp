#ifndef LINETOOL_HPP
#define LINETOOL_HPP

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QtSql>
#include <memory>
#include <list>

class LineTool : public QWidget
{
    Q_OBJECT
public:
    LineTool(QString _tool_name, QString _date, QSqlQuery* q, QString unit_id);

    std::shared_ptr<QHBoxLayout> layout;
    std::shared_ptr<QLabel> tool_name;
    std::shared_ptr<QLabel> date;
    std::shared_ptr<QPushButton> gave;

    QString _tool_name;
    QString _date;
    QSqlQuery *q;
    QString unit_id;
    std::list<std::unique_ptr<LineTool>>* my_list = nullptr;
    std::list<std::unique_ptr<LineTool>>::iterator me;

    void refund();

signals:
    void signal_click();
};

#endif // LINETOOL_HPP
