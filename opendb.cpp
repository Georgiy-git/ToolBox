#include "opendb.hpp"
#include "ui_opendb.h"

#include <QFileDialog>
#include <QMessageBox>

OpenDb::OpenDb() : ui(new Ui::OpenDb)
{
    ui->setupUi(this);
    setFixedSize(400, 150);
    ui->lineEdit->setPlaceholderText("Введите путь к изображению ");
    disconnect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    setWindowIcon(QIcon(":/Images/mine.svg"));
    setWindowTitle(" ");
}

OpenDb::~OpenDb()
{
    delete ui;
}

void OpenDb::on_pushButton_clicked()
{
    QString s = QFileDialog::getOpenFileName(
        this,
        "Открыть файл",
        QDir::homePath(),
        "Изображения (*.svg *.png);;Все файлы (*)"
    );
   ui->lineEdit->setText(s);
}


void OpenDb::on_buttonBox_accepted()
{
    QString file_name = ui->lineEdit->text();

    if (file_name.trimmed().isEmpty()) {
        QMessageBox::information(this, "Ошибка", "Поле не может быть пустым ");
        return;
    } else if (!QFile::exists(file_name)) {
        QMessageBox::information(this, "Ошибка", "Файл не найден ");
        return;
    }

    ui->lineEdit->setText("");
    emit signal_file_find(file_name.trimmed());
    hide();
}

