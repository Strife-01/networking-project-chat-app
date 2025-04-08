#include "chatroomwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatRoomWindow w;
    w.show();
    return a.exec();
}
