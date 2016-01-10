#include "gamearea.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFontDatabase::addApplicationFont(":/fonts/FontAwesome.otf");
    GameArea w;
    w.show();

    return a.exec();
}
