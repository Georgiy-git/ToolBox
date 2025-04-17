#include "verification.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WindowVerification *w = new WindowVerification;
    w->show();
    return a.exec();
}
