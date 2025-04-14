#include "privatechatwindow.h"
#include "MessageQueue.h"
#include <QDateTime>
#include <unordered_set>
#include <algorithm>

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
    privateChatDisplay->setReadOnly(true);        // Here you would normally send the message over the network, WIP Andrei
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


    QString shitty_format_msg = privateMessageInput->text();

    if (!shitty_format_msg.isEmpty()) {
        privateChatDisplay->append("You: " + shitty_format_msg);
        privateMessageInput->clear();

        // Here you would normally send the message over the network, WIP
        // networkInterface->sendPrivateMessage(recipient, message);


        std::vector<char> message;

        QByteArray ba = shitty_format_msg.toLocal8Bit().data();

        for(int i =0;i<ba.length();i++){
            message.push_back(ba[i]);
        }

        privateMessageInput->clear();
        tm->sendMessage(message, contactAddress, packet_header::types::data);

    }

}

void PrivateChatWindow::loadHistory()
{
    // Temporary set to track seen messages by content+sender+timestamp
    std::unordered_set<std::string> seenMessages;

    // I DON'T WANT TO HEAR IT THIS IS VERY READABLE, FUCK YOU IF YOU THINK OTHERWISE, CONSIDER GOING BACK TO PYTHON
    auto getMessageSignature = [](const Message_Queue::MessageQueue::Message& msg) {
        return std::to_string(msg.sender_address) + "|" + msg.message + "|" +
               std::to_string(msg.private_message);
    };

    // Get all messages from both perspectives
    auto sentMessages = Message_Queue::msg_queue.get_messages(myAddress);
    auto receivedMessages = Message_Queue::msg_queue.get_messages(contactAddress);

    // Combine messages and sort by some logical order (like timestamp if available)
    std::vector<Message_Queue::MessageQueue::Message> allMessages;
    allMessages.insert(allMessages.end(), sentMessages.begin(), sentMessages.end());
    allMessages.insert(allMessages.end(), receivedMessages.begin(), receivedMessages.end());

    // Sort by some natural order (pseudo-timestamp using message content)
    std::sort(allMessages.begin(), allMessages.end(),
              [](const auto& a, const auto& b) {
                  return a.message < b.message; // Simple content-based ordering
                  // Need to figure out how to retrive the timestamp from the data transfer, WIP, atm is pseudo
              });

    // Display messages with deduplication by using the lambda generated signatures
    for (const auto& msg : allMessages) {
        std::string signature = getMessageSignature(msg);
        if (seenMessages.insert(signature).second) { // Only show if not seen before
            bool isFromMe = (msg.sender_address == myAddress);
            QString prefix = isFromMe ? "You: " : "Them: ";
            QString displayMsg = prefix + QString::fromStdString(msg.message);

            // Finally, show them when you display them in chat
            privateChatDisplay->append(displayMsg);
        }
    }
}
