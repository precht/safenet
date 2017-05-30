#include <QApplication>

#include "manager.h"
#include "httpwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    Manager *manager = new Manager();

    HttpWindow hw(manager);

    const QRect availableSize = QApplication::desktop()->availableGeometry(&hw);
    hw.resize(availableSize.width() / 5, availableSize.height() / 5);
    hw.move((availableSize.width() - hw.width()) / 2, (availableSize.height() - hw.height()) / 2);
    hw.show();

    return a.exec();

}
