#ifndef VERIFICATION_HPP
#define VERIFICATION_HPP

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class WindowVerification; }
QT_END_NAMESPACE

class WindowVerification : public QWidget
{
    Q_OBJECT

public:
    WindowVerification();
    ~WindowVerification();

private slots:
    void on_pushButton_clicked();

private:
    Ui::WindowVerification *ui;
};
#endif // VERIFICATION_HPP
