#ifndef UNIT_HPP
#define UNIT_HPP

#include <QPushButton>
#include <QString>

class Unit
{
public:
    Unit(QString f, QString i, QString o, int id);

    std::shared_ptr<QPushButton> button;
    QString f;
    QString i;
    QString o;
    int id;
    std::vector<std::shared_ptr<Unit>>::iterator iter;
};

#endif // UNIT_HPP
