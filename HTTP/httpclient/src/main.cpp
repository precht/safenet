#include <QApplication>

#include "manager.h"
#include "httpwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Manager *manager = new Manager();

    HttpWindow hw(manager);
    hw.show();

    return a.exec();

}
