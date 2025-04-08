#include "privatechatwindow.h"

PrivateChatWindow::PrivateChatWindow(const QString &contactName, QWidget *parent)
    : QDialog(parent), contactName(contactName)
{
    setWindowTitle("Private Chat with " + contactName);
    resize(500, 400);
    setModal(false); // Allow multiple private chats

    setupPrivateChatUI();
}



void PrivateChatWindow::setupPrivateChatUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Chat display area
    privateChatDisplay = new QTextEdit(this);
    privateChatDisplay->setReadOnly(true);
    privateChatDisplay->setStyleSheet("QTextEdit { background-color: #f5f5f5; }");
    mainLayout->addWidget(privateChatDisplay);

    // Input area at bottom
    QWidget *inputWidget = new QWidget(this);
    QHBoxLayout *inputLayout = new QHBoxLayout(inputWidget);

    privateMessageInput = new QLineEdit(inputWidget);
    privateMessageInput->setPlaceholderText("Type private message to " + contactName + "...");
    privateSendButton = new QPushButton("Send", inputWidget);
    privateSendButton->setFixedWidth(80);

    inputLayout->addWidget(privateMessageInput);
    inputLayout->addWidget(privateSendButton);

    mainLayout->addWidget(inputWidget);
}

PrivateChatWindow::~PrivateChatWindow()
{
}
