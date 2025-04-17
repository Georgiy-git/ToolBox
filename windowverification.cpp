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
}

WindowVerification::~WindowVerification()
{
    delete ui;
}

void WindowVerification::on_pushButton_clicked()
{
    WindowWorkers *w = new WindowWorkers;
    w->show();
    delete this;
}

