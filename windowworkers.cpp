#include "linetool.hpp"
#include "toolstable.hpp"
#include "windowworkers.hpp"
#include "ui_windowworkers.h"

#include <QMessageBox>
#include <QtSql>

WindowWorkers::WindowWorkers() : ui(new Ui::WindowWorkers)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
    setWindowTitle("ToolBox");
    setWindowIcon(QIcon(":/Images/mine.svg"));
    ui->lineEdit->setPlaceholderText("Поиск");
    setCardVisible(false);
    units_layout = new QVBoxLayout;
    units_layout->setAlignment(Qt::AlignTop);
    ui->scrollArea->widget()->setLayout(units_layout);
    tools_layout = new QVBoxLayout;
    tools_layout->setAlignment(Qt::AlignTop);
    ui->scrollArea_2->widget()->setLayout(tools_layout);

    qdb = QSqlDatabase::addDatabase("QSQLITE");
    qdb.setDatabaseName("toolbox.db");
    qdb.open();
    q = new QSqlQuery(qdb);
    ok = q->exec("CREATE TABLE IF NOT EXISTS unit_tools (id INTEGER, tool TEXT, time TEXT)");
    pruf(ok);

    //База данных
    db_ok = sqlite3_open("toolbox.db", &db);
    pruf_db(db_ok, error_db);
    db_ok = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS units"
                    " (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    " f TEXT, i TEXT, o TEXT)", nullptr, nullptr, &error_db);
    pruf_db(db_ok, error_db);
    db_ok = sqlite3_exec(db, "SELECT * FROM units", callback_db_units, this, &error_db);
    pruf_db(db_ok, error_db);

    //Добавление юнитов
    add_unit = new QWidget(nullptr, Qt::WindowCloseButtonHint);
    add_unit->setFixedWidth(400);
    add_unit->setMaximumHeight(300);
    add_unit->setMaximumHeight(600);
    add_unit->setWindowTitle(" ");
    add_unit->setWindowIcon(QIcon(":/Images/mine.svg"));
    QVBoxLayout *add_unit_layout = new QVBoxLayout(add_unit);
    add_unit_text = new QTextEdit(add_unit);
    add_unit_text->setPlaceholderText("Введите ФИО юнита(-ов)");
    QPushButton* add_unit_button = new QPushButton(add_unit);
    add_unit_button->setText("Добавить");
    add_unit_button->setStyleSheet(blue_button_style);
    add_unit_button->setFixedSize(100, 35);
    connect(add_unit_button, &QPushButton::clicked, this, add_unit_get);
    add_unit_layout->addWidget(add_unit_text);
    add_unit_layout->addWidget(add_unit_button, 0, Qt::AlignHCenter);
    add_unit->setLayout(add_unit_layout);
}

WindowWorkers::~WindowWorkers()
{
    delete ui;
}

void WindowWorkers::setCardVisible(bool x)
{
    if (!x) {
        ui->label_2->setVisible(0);
        ui->label_3->setVisible(0);
        ui->scrollArea_2->setVisible(0);
        ui->lineEdit_2->setVisible(0);
        ui->pushButton->setVisible(0);
        ui->pushButton_2->setVisible(0);
    } else {
        ui->label_2->setVisible(true);
        ui->label_3->setVisible(true);
        ui->scrollArea_2->setVisible(true);
        ui->lineEdit_2->setVisible(true);
        ui->pushButton->setVisible(true);
        ui->pushButton_2->setVisible(true);
    }
}

void WindowWorkers::add_unit_get()
{
    QString text = add_unit_text->toPlainText() + '\n';
    std::vector<QString> FIO;
    QString str;
    for (auto i : text) {
        if (i == '\n') {
            FIO.push_back(str);
            str.clear();
            if (FIO.size() == 3) {
                std::string sql = "INSERT INTO units (f, i, o) VALUES (";
                for (auto& i : FIO) {
                    sql += "'" + i.toStdString() + "', ";
                }
                sql.pop_back();
                sql.pop_back();
                sql += ")";
                db_ok = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &error_db);
                pruf_db(db_ok, error_db);

                std::shared_ptr<QPushButton> b = std::make_shared<QPushButton>();
                QString _fio = FIO[0] + ' ';
                _fio += FIO[1].front();
                _fio += ". ";
                _fio += FIO[2].front();
                _fio += '.';
                b->setText(_fio);
                b->setStyleSheet(blue_button_style);
                std::vector<QString> strs;
                strs.push_back(QString::number(++last_id));
                strs.push_back(FIO[0]);
                strs.push_back(FIO[1]);
                strs.push_back(FIO[2]);
                idfio.insert(strs);
                connect(b.get(), &QPushButton::clicked, this,
                        [=, this]{on_unit_clicked(strs); this_button = b.get(); });
                unit_buttons.push_back(b);
                units_layout->addWidget(b.get());
            }
            FIO.clear();
        }
        else if (i == ' ') {
            if (!str.isEmpty()) {
                FIO.push_back(str);
            }
            str.clear();
        }
        else {
            str += i;
        }
    }
    add_unit_text->clear();
    add_unit->hide();
    add_unit_show = false;
}

void WindowWorkers::pruf_db(int db_ok, char* error_db)
{
    if (db_ok) {
        QMessageBox::information(nullptr,
            QString("Ошибка в базе данных"), QString(error_db));
    }
}

void WindowWorkers::pruf(bool ok)
{
    if (!ok) {
        QMessageBox::critical(nullptr, QString("Ошибка в базе данных"), q->lastError().text());
    }
}

int WindowWorkers::callback_db_units(void *x, int column, char **data, char **col_name)
{
    WindowWorkers *obj = static_cast<WindowWorkers*>(x);
    obj->last_id = atoi(data[0]);
    QString str;
    str += data[1];
    str += ' ';
    str += QString(data[2]).front();
    str += ". ";
    str += QString(data[3]).front();
    str += '.';
    std::shared_ptr<QPushButton> b = std::make_shared<QPushButton>();
    obj->q->exec(QString("SELECT * FROM unit_tools WHERE id =") + data[0]);
    obj->pruf(obj->q);
    if (obj->q->next()) {
        b->setStyleSheet(obj->red_button_style);
        obj->units_layout->insertWidget(0, b.get());
    }
    else {
        b->setStyleSheet(obj->blue_button_style);
        obj->units_layout->addWidget(b.get());
    }
    obj->q->clear();
    b->setText(str);
    std::vector<QString> strs;
    for (size_t i = 0; i<4; ++i) {
        strs.push_back(QString(data[i]));
    }
    obj->idfio.insert(strs);
    obj->connect(b.get(), QPushButton::clicked, obj,
                 [=]{obj->on_unit_clicked(strs); obj->this_button = b.get();});
    obj->unit_buttons.push_back(b);
    return 0;
}

void WindowWorkers::on_action_3_triggered()
{
    if (!add_unit_show) {
        add_unit->show();
        add_unit_show = true;
    }
    else {
        add_unit->hide();
        add_unit->show();
    }
}


void WindowWorkers::on_pushButton_clicked()
{
    if (!card_visible) {
        QString text = ui->lineEdit_2->text().trimmed();
        if (!text.isEmpty()) {
            QDateTime currentDateTime = QDateTime::currentDateTime();
            QString date = currentDateTime.toString("dd-MM-yyyy HH:mm");
            q->prepare("INSERT INTO unit_tools (id, tool, time) VALUES (:id, :tool, :time)");
            q->bindValue(":id", this_idfio.front());
            q->bindValue(":tool", text);
            q->bindValue(":time", date);
            ok = q->exec();
            pruf(ok);
            ui->lineEdit_2->setText("");
            std::unique_ptr<LineTool> t =
                    std::make_unique<LineTool>(text, date, q, this_idfio.front());
            connect(t.get(), &LineTool::signal_click, this,
                    [&]{if (lines_tool.size()==1) this_button->setStyleSheet(blue_button_style);});
            t->my_list = &lines_tool;
            tools_layout->addWidget(t.get());
            lines_tool.push_back(std::move(t));
            lines_tool.back()->me = --lines_tool.end();
            this_button->setStyleSheet(red_button_style);
        }
    }
}

void WindowWorkers::on_unit_clicked(std::vector<QString> strs)
{
    lines_tool.clear();
    this_idfio = strs;
    ui->label_2->setText(strs.at(1) + ' ' + strs.at(2) + ' ' + strs.at(3));
    ui->label_3->setText("id: " + strs.at(0));
    ok = q->exec("SELECT * FROM unit_tools WHERE id = " + strs.at(0));
    pruf(ok);
    while (q->next()) {
        std::unique_ptr<LineTool> t = std::make_unique<LineTool>(q->value(1).toString(),
                            q->value(2).toString(), q, this_idfio.front());
        connect(t.get(), &LineTool::signal_click, this,
            [&]{if (lines_tool.size()==1) this_button->setStyleSheet(blue_button_style);});
        t->my_list = &lines_tool;
        tools_layout->addWidget(t.get());
        lines_tool.push_back(std::move(t));
        lines_tool.back()->me = --lines_tool.end();
    }
    setCardVisible(true);
}

void WindowWorkers::closeEvent(QCloseEvent *event)
{
    if (toolstable != nullptr) {toolstable->save_into_db();}
    QApplication::quit();
}


void WindowWorkers::on_pushButton_2_clicked()
{
    lines_tool.clear();
    this_idfio.clear();
    add_unit_show = false;
    this_button = nullptr;
    setCardVisible(false);
}


void WindowWorkers::on_action_triggered()
{
    if (toolstable == nullptr) {
        toolstable = new ToolsTable(db, q);
        toolstable->show();
    }
    else {
        toolstable->hide();
        toolstable->show();
    }
}


void WindowWorkers::on_lineEdit_textChanged(const QString &str)
{
    for (auto i : unit_buttons) {
        if (!(i->text().startsWith(str.trimmed()))) {
            i->hide();
        }
        else {
            i->show();
        }
    }
}


void WindowWorkers::on_action_4_triggered()
{
    if (!del_unit) {
    del_unit = true;
    del_unit_window = new QWidget;
    del_unit_window->setFixedSize(300, 60);
    del_unit_window->setWindowTitle(" ");
    del_unit_window->setWindowIcon(QIcon(":/Images/user.svg"));
    QLineEdit* l = new QLineEdit(del_unit_window);
    l->setPlaceholderText("Введите id юнита");
    QHBoxLayout* la = new QHBoxLayout;
    QPushButton* b = new QPushButton;
    b->setText("Удалить");
    b->setStyleSheet(red_button_style);
    b->setFixedSize(60, 25);
    connect(b, &QPushButton::clicked, this, [&, l]{
        on_pushButton_2_clicked();
        ok = q->exec("DELETE FROM units WHERE id = "+l->text().trimmed());
        pruf(ok);
        // db_ok = sqlite3_exec(db, "SELECT * FROM units", callback_db_units, this, &error_db);
        // pruf_db(db_ok, error_db);
        l->setText(" ");
        del_unit_window->hide();
    });
    la->addWidget(l);
    la->addWidget(b);
    del_unit_window->setLayout(la);
    del_unit_window->show();
    }
    else {
        del_unit_window->hide();
        del_unit_window->show();
    }
}

