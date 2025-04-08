#ifndef CHATROOMWINDOW_H
#define CHATROOMWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QLabel>
#include "privatechatwindow.h"


class ChatRoomWindow : public QMainWindow
{
    Q_OBJECT

public:
    ChatRoomWindow(QWidget *parent = nullptr);
    ~ChatRoomWindow();

    // UI Elements for group chat
    QTextEdit *chatDisplay;
    QListWidget *memberList;
    QLineEdit *messageInput;
    QPushButton *sendButton;

private:
    void setupGroupChatUI();

    PrivateChatWindow *privateChat = new PrivateChatWindow("membername", this);


};

#endif // CHATROOMWINDOW_H
