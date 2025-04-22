#include "opendb.hpp"
#include "tableimage.hpp"


TableImage::TableImage(QTableWidget* table_path, QString file_name, int row)
    : table_path{table_path}, file_name{file_name}, row{row}
{
    if (!file_name.trimmed().isEmpty()) {
        l_image = std::make_shared<QLabel>();
        layout = std::make_shared<QVBoxLayout>();
        l_image->setPixmap(QPixmap(file_name));
        layout->addWidget(l_image.get());
        setLayout(layout.get());
    }

    find_file = std::make_shared<OpenDb>();
    menu = std::make_shared<QMenu>();
    action_input = std::make_shared<QAction>("Вставить");
    connect(action_input.get(), &QAction::triggered, this, [&]{find_file->show();});
    connect(find_file.get(), &OpenDb::signal_file_find, this, set_new_image);
    action_del = std::make_shared<QAction>("Удалить");
    menu->addActions({action_input.get(), action_del.get()});
}

void TableImage::set_new_image(QString _file_name)
{
    l_image.reset();
    layout.reset();

    l_image = std::make_shared<QLabel>();
    layout = std::make_shared<QVBoxLayout>();
    l_image->setPixmap(QPixmap(_file_name));
    layout->addWidget(l_image.get());
    setLayout(layout.get());
    file_name = _file_name;
    emit signal_set_size(row);
}

void TableImage::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        menu->popup(event->globalPosition().toPoint());
    }
}
