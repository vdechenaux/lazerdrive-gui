#include "dialog.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFontDatabase::addApplicationFont(":/fonts/FontAwesome.otf");
    Dialog w;
    w.show();

    return a.exec();
}
