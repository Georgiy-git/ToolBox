#ifndef VERIFICATION_HPP
#define VERIFICATION_HPP

#include <QWidget>
#include <QtSql>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class WindowVerification; }
QT_END_NAMESPACE

class WindowVerification : public QWidget
{
    Q_OBJECT

public:
    WindowVerification();
    ~WindowVerification();
    void pruf(bool);

private slots:
    void on_pushButton_clicked();

private:
    Ui::WindowVerification *ui;

    QSqlQuery* q;
    QSqlDatabase db;
    int ok;

};
#endif // VERIFICATION_HPP
