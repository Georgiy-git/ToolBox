#ifndef OPENDB_HPP
#define OPENDB_HPP

#include <QDialog>

namespace Ui {
class OpenDb;
}

class OpenDb : public QDialog
{
    Q_OBJECT

public:
    OpenDb();
    ~OpenDb();

private slots:
    void on_pushButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::OpenDb *ui;

signals:
    void signal_file_find(QString);
};

#endif // OPENDB_HPP
