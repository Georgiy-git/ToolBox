#ifndef WINDOWWORKERS_HPP
#define WINDOWWORKERS_HPP

#include <QMainWindow>

namespace Ui {
class WindowWorkers;
}

class WindowWorkers : public QMainWindow
{
    Q_OBJECT

public:
    explicit WindowWorkers();
    ~WindowWorkers();

private slots:
    void on_action_3_triggered();

    void on_pushButton_clicked();

private:
    Ui::WindowWorkers *ui;
    bool card_visible = false;

    void setCardVisible(bool);
};

#endif // WINDOWWORKERS_HPP
