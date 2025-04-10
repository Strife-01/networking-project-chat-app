#include "privatechatwindow.h"
#include "MessageQueue.h"
#include <QDateTime>

PrivateChatWindow::PrivateChatWindow(const QString &contactName, uint8_t contactAddress, uint8_t myAddress, QWidget *parent)
    : QDialog(parent), contactName(contactName), contactAddress(contactAddress), myAddress(myAddress)
{
    setWindowTitle("Private Chat with " + contactName);
    resize(500, 400);
    setModal(false);

    setupPrivateChatUI();
    loadHistory();
}

void PrivateChatWindow::setupPrivateChatUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Chat display area
    privateChatDisplay = new QTextEdit(this);
    privateChatDisplay->setReadOnly(true);        // Here you would normally send the message over the network
        // networkInterface->sendPrivateMessage(recipient, message);
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
    // Connect and make the send button functional (hopefully?)
    connect(privateSendButton, &QPushButton::clicked, this, [this]() {
        sendMessage(privateMessageInput->text());
    });
}

PrivateChatWindow::~PrivateChatWindow()
{ 
}

void PrivateChatWindow::sendMessage(const QString &message)
{
    if (message.isEmpty()) return;

    // Add to message queue
    auto newMsg = Message_Queue::msg_queue.create_message(
        contactAddress,
        message.toStdString(),
        true,  // private_message
        false  // seen_message
        );
    Message_Queue::msg_queue.push_message(newMsg, myAddress);       // As sender
    Message_Queue::msg_queue.push_message(newMsg, contactAddress);  // As recipient

    // Update UI
    QString timestamp = QDateTime::currentDateTime().toString("[hh:mm]");
    privateChatDisplay->append(timestamp + " You: " + message);
    privateMessageInput->clear();
}

void PrivateChatWindow::loadHistory()
{
    auto messages = Message_Queue::msg_queue.get_messages(contactAddress);

    for (const auto& msg : messages) {
        QString prefix = (msg.sender_address == myAddress) ? "You: " : "Them: ";
        QString timestamp = QDateTime::currentDateTime().toString("[hh:mm] ");
        privateChatDisplay->append(timestamp + " " + prefix + QString::fromStdString(msg.message));
    }
}
