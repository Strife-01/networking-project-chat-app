#include "chatwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QTime>
#include <QDebug>
#include <QTimer>
#include <QListWidgetItem>

ChatWidget::ChatWidget(QWidget *parent) //need to convert this part to the actual general code

    : QWidget(parent)

{
    setupUI();
    setupConnections();

    // 模拟初始用户
    //currentUser = QString('a' + userCounter % 26);
    //users[currentUser] = true;
    //userCounter++;
    updateUserList();
    appendMessage("Welcome to the chat rooom!", "System");
}

void ChatWidget::setupUI()
{
    // main setup
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // left side message area
    QVBoxLayout *messageLayout = new QVBoxLayout();

    messageDisplay = new QTextEdit();
    messageDisplay->setReadOnly(true);
    messageLayout->addWidget(messageDisplay);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    messageInput = new QLineEdit();
    sendButton = new QPushButton("Send");
    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);
    messageLayout->addLayout(inputLayout);

    // right side user list
    userList = new QListWidget();
    userList->setFixedWidth(100);

    mainLayout->addLayout(messageLayout, 1);
    mainLayout->addWidget(userList);

    setWindowTitle("Chatroom");
    resize(600, 400);
}

void ChatWidget::setupConnections() //need to convert this part to the actual general code
{
    connect(sendButton, &QPushButton::clicked, this, &ChatWidget::sendMessage);
    connect(messageInput, &QLineEdit::returnPressed, this, &ChatWidget::sendMessage);

    // user join/leave
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this](){
        static bool addUser = true;
        if(addUser && userCounter < 4) {
            userJoined();
        } else {
            userLeft();
        }
        addUser = !addUser || userCounter >= 4;
    });
    timer->start(5000);  // change every 5s
}

void ChatWidget::sendMessage()
{
    QString message = messageInput->text().trimmed();
    if(!message.isEmpty()) {
        appendMessage(message, currentUser);
        messageInput->clear();
    }
}

void ChatWidget::userJoined() //need to convert this part to the actual general code
{
    QString newUser = QString('a' + userCounter % 4);
    users[newUser] = true;
    userCounter++;
    appendMessage(QString("User %1 join the room").arg(newUser), "System");
    updateUserList();
}

void ChatWidget::userLeft() //need to convert this part to the actual general code
{
    if(userCounter > 1) {  // at least keep the current user
        int leaveIndex = QRandomGenerator::global()->bounded(userCounter-1);
        QString leaveUser = QString('a' + leaveIndex % 4);
        if(users.contains(leaveUser) && leaveUser != currentUser) {
            users.remove(leaveUser);
            appendMessage(QString("User %1 left the room").arg(leaveUser), "System");
            updateUserList();
        }
    }
}

void ChatWidget::updateUserList() //need to convert this part to the actual general code
{
    userList->clear();
    for(auto it = users.begin(); it != users.end(); ++it) {
        QListWidgetItem *item = new QListWidgetItem(it.key(), userList);
        item->setForeground(it.key() == currentUser ? Qt::blue : Qt::black);
    }
}

void ChatWidget::appendMessage(const QString &message, const QString &sender)
{
    QString time = QTime::currentTime().toString("hh:mm:ss");
    QString formattedMessage = QString("[%1] %2: %3")
                                   .arg(time)
                                   .arg(sender)
                                   .arg(message);
    messageDisplay->append(formattedMessage);
}
