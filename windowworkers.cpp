#include "unit.hpp"
#include "linetool.hpp"
#include "toolstable.hpp"
#include "windowworkers.hpp"
#include "ui_windowworkers.h"

#include <QMessageBox>
#include <QComboBox>

WindowWorkers::WindowWorkers(QSqlDatabase qdb) : ui(new Ui::WindowWorkers), qdb{qdb}
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
    ui->comboBox->addItems({"По имени", "По инструменту"});

    //База данных
    q = new QSqlQuery(qdb);
    ok = q->exec("CREATE TABLE IF NOT EXISTS unit_tools (id INTEGER, tool TEXT, time TEXT)");
    pruf(ok);

    db_ok = sqlite3_open("toolbox.db", &db);
    pruf_db(db_ok, error_db);
    db_ok = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS units"
                    " (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    " f TEXT, i TEXT, o TEXT)", nullptr, nullptr, &error_db);
    pruf_db(db_ok, error_db);
    db_ok = sqlite3_exec(db, "SELECT * FROM units", callback_db_units, this, &error_db);
    pruf_db(db_ok, error_db);

    //Окно добавления юнитов
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

WindowWorkers::~WindowWorkers() { delete ui; }

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
                for (const auto& i : FIO) {
                    sql += "'" + i.toStdString() + "', ";
                }
                sql.pop_back();
                sql.pop_back();
                sql += ")";
                db_ok = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &error_db);
                pruf_db(db_ok, error_db);

                std::shared_ptr<Unit> unit =
                    std::make_shared<Unit>(FIO[0], FIO[1], FIO[2], ++last_id);
                unit->button = std::make_shared<QPushButton>();
                units.push_back(unit);
                unit->iter = --units.end();

                QString _fio = FIO[0] + ' ';
                _fio += FIO[1].front();
                _fio += ". ";
                _fio += FIO[2].front();
                _fio += '.';
                unit->button->setText(_fio);
                unit->button->setStyleSheet(blue_button_style);

                connect(unit->button.get(), &QPushButton::clicked, this,
                        [=, this]{on_unit_clicked(unit.get()); this_unit = unit.get(); });
                units_layout->addWidget(unit->button.get());
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
    obj->ok = obj->q->exec(QString("SELECT * FROM unit_tools WHERE id =") + data[0]);
    obj->pruf(obj->ok);
    std::shared_ptr<Unit> unit =
            std::make_shared<Unit>(QString(data[1]), QString(data[2]), QString(data[3]), obj->last_id);
    unit->button = std::make_shared<QPushButton>();
    if (obj->q->next()) {
        unit->button->setStyleSheet(obj->red_button_style);
        obj->units_layout->insertWidget(0, unit->button.get());
    }
    else {
        unit->button->setStyleSheet(obj->blue_button_style);
        obj->units_layout->addWidget(unit->button.get());
    }
    obj->q->clear();
    unit->button->setText(str);

    obj->units.push_back(unit);
    unit->iter = --(obj->units.end());

    obj->connect(unit->button.get(), QPushButton::clicked, obj,
        [=]{obj->on_unit_clicked(unit.get()); obj->this_unit = unit.get(); } );
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
            QString date = currentDateTime.toString("dd-MM-yyyy HH:mm ss сек");
            q->prepare("INSERT INTO unit_tools (id, tool, time) VALUES (:id, :tool, :time)");
            q->bindValue(":id", this_unit->id);
            q->bindValue(":tool", text);
            q->bindValue(":time", date);
            ok = q->exec();
            pruf(ok);
            ui->lineEdit_2->setText("");
            std::unique_ptr<LineTool> t =
                    std::make_unique<LineTool>(text, date, q, QString::number(this_unit->id));
            connect(t.get(), &LineTool::signal_click, this,
                    [&]{if (lines_tool.size()==1) this_unit->button->setStyleSheet(blue_button_style);});
            t->my_list = &lines_tool;
            tools_layout->addWidget(t.get());
            lines_tool.push_back(std::move(t));
            lines_tool.back()->me = --lines_tool.end();
            this_unit->button->setStyleSheet(red_button_style);
        }
    }
}

void WindowWorkers::on_unit_clicked(Unit* unit)
{
    lines_tool.clear();
    this_unit = unit;
    ui->label_2->setText(unit->f + ' ' + unit->i + ' ' + unit->o);
    ui->label_3->setText("id: " + QString::number(unit->id));
    ok = q->exec("SELECT * FROM unit_tools WHERE id = " + QString::number(unit->id));
    pruf(ok);
    while (q->next()) {
        std::unique_ptr<LineTool> t = std::make_unique<LineTool>(q->value(1).toString(),
                            q->value(2).toString(), q, QString::number(this_unit->id));
        connect(t.get(), &LineTool::signal_click, this,
            [&]{if (lines_tool.size()==1) this_unit->button->setStyleSheet(blue_button_style);});
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
    add_unit_show = false;
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
    if (ui->comboBox->currentText() == "По имени") {
        for (auto i : units) {
            if (!(i->button->text().startsWith(str.trimmed()))) {
                i->button->hide();
            }
            else {
                i->button->show();
            }
        }
    }
    else {
        ok = q->exec("SELECT id FROM unit_tools WHERE tool='"+str+"'");
        pruf(ok);
        bool y = false;
        std::vector<std::shared_ptr<Unit>> v;

        while (q->next()) {//нач

        y = true;
        int _id = q->value(0).toInt();
        for (auto i : units) {
            if (i->id == _id) {
                v.push_back(i);
            }
        }
        }//кон

        if (!y) {
            for (auto i : units) {
                i->button->hide();
            }
            return;
        }

        for (auto i : units) {
            i->button->hide();
        }


        for (auto i : v) {
            i->button->show();
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
        ok = q->exec("DELETE FROM unit_tools WHERE id = "+l->text().trimmed());
        pruf(ok);
        for (const auto i : units) {
            if (i->id == l->text().trimmed().toInt()) {
                units.erase(i->iter);
                i->button->hide();
                break;
            }
        }
        l->setText("");
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


void WindowWorkers::on_comboBox_activated(int index)
{
    QString x = ui->lineEdit->text();
    on_lineEdit_textChanged(x);
}

