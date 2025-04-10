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
#include <QHash>
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
    void updateMemberCount();
    void addMember(const QString& memberName);
    void removeMember(const QString& memberName);


private slots:
    void handleMemberClick(QListWidgetItem *item);
    void sendPrivateMessage(const QString &recipient);
    void privateChatClosed(const QString &contactName);
    void sendGlobalMessage();
    void receiveGlobalMessage(const QString& sender, const QString& message);

private:
    QLabel* memberCountLabel;
    void setupGroupChatUI();
    QHash<QString, PrivateChatWindow*> privateChats; // Stores all active private chats
    QHash<QString, uint8_t> memberAddresses;
    uint8_t myAddress;
};

#endif // CHATROOMWINDOW_H
