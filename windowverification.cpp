#include "windowworkers.hpp"
#include "verification.hpp"
#include "./ui_windowverification.h"

WindowVerification::WindowVerification() : ui(new Ui::WindowVerification)
{
    ui->setupUi(this);
    setFixedSize(400, 300);
    ui->horizontalLayout_2->setAlignment(Qt::AlignLeft);
    setWindowTitle("ToolBox");
    setWindowIcon(QIcon(":/Images/mine.svg"));

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("toolbox.db");
    db.open();
    q = new QSqlQuery(db);
    ok = q->exec("CREATE TABLE IF NOT EXISTS verification (login TEXT, password TEXT)");
    pruf(ok);
    ok = q->exec("CREATE TABLE IF NOT EXISTS verif_history (login TEXT, password TEXT)");
    pruf(ok);
    ok = q->exec("SELECT * FROM verif_history");
    pruf(ok);
    if (q->next()) {
        ui->lineEdit->setText(q->value(0).toString());
        ui->lineEdit_2->setText(q->value(1).toString());
        ui->checkBox->setChecked(true);
    }
}

WindowVerification::~WindowVerification()
{
    delete ui;
}

void WindowVerification::on_pushButton_clicked()
{
    QString l = ui->lineEdit->text();
    QString p = ui->lineEdit_2->text();

    ok = q->exec("SELECT * FROM verification WHERE login='"+l+"' AND password='"+p+"'");
    pruf(ok);
    if (!ok) { return; }

    if (q->next()) {
        WindowWorkers *w = new WindowWorkers(db);
        w->show();
        delete q;
        delete this;

        if (ui->checkBox->isChecked()) {
            ok = q->exec("DELETE FROM verif_history");
            ok = q->exec("INSERT INTO verif_history (login, password) VALUES ('"+l+"', '"+p+"')");
            pruf(ok);
        }
        else {
            ok = q->exec("DELETE FROM verification");
            pruf(ok);
        }
    }
    else {
        QMessageBox::information(nullptr, " ", "Неправильный логин или пароль");
    }
}

void WindowVerification::pruf(bool ok)
{
    if (!ok) {
        QMessageBox::critical(nullptr, QString("Ошибка в базе данных"), q->lastError().text());
    }
}
