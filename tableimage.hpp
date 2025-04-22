#ifndef TABLEIMAGE_HPP
#define TABLEIMAGE_HPP

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QMenu>
#include <QTableWidget>
#include <memory>

class OpenDb;

class TableImage : public QWidget
{
    Q_OBJECT
public:
    TableImage(QTableWidget* table_path, QString file_name, int row);

    void save();

    void set_new_image(QString _file_name);

    QTableWidget* table_path;
    QString file_name;
    int row;

private:

    std::shared_ptr<QLabel> l_image;
    std::shared_ptr<QVBoxLayout> layout;
    std::shared_ptr<QPixmap> pixmap;
    std::shared_ptr<QMenu> menu;
    std::shared_ptr<QAction> action_input;
    std::shared_ptr<QAction> action_del;
    std::shared_ptr<OpenDb> find_file;

    void mousePressEvent(QMouseEvent *event) override;

signals:
    void signal_set_size(int);
};

#endif // TABLEIMAGE_HPP
