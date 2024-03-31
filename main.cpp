#include "piano.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    piano window;
    window.show();
    return a.exec();
}
