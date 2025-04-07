#include <QApplication>
#include "chatwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ChatWidget chatWindow;
    chatWindow.show();

    return a.exec();
}
