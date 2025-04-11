#include "chatroomwindow.h"
#include "privatechatwindow.h"
#include "../../backend/MessageQueue/MessageQueue.h"
#include "../../backend/addressing/Addressing.h"

#include <QDateTime>

using  namespace std;

ChatRoomWindow::ChatRoomWindow(QWidget *parent)
    : QMainWindow(parent), myAddress(dynamic_addressing::get_my_addr()) // Ok so since I have no addressing system implemented yet I will just use this
{
    setWindowTitle("Chat Room");
    resize(800, 600);

    // Main central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Splitter for resizable panels
    QSplitter *splitter = new QSplitter(Qt::Horizontal, centralWidget);

    // Left side - Chat display
    QWidget *leftPanel = new QWidget(splitter);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);

    chatDisplay = new QTextEdit(leftPanel);
    chatDisplay->setReadOnly(true);
    chatDisplay->setStyleSheet("QTextEdit { background-color: #f9f9f9; }");
    leftLayout->addWidget(new QLabel("Group Chat:", leftPanel));
    leftLayout->addWidget(chatDisplay);

    chatDisplay->setStyleSheet(
        "QTextEdit {"
        "   background-color: #f9f9f9;"
        "   font-family: 'Courier New';"
        "}"
        "QTextEdit[messageType='global'] {"
        "   color: #333;"
        "}"
        "QTextEdit[messageType='private'] {"
        "   color: #0066cc;"
        "}"
        );
    // Input area at the bottom
    QWidget *inputWidget = new QWidget(leftPanel);
    QHBoxLayout *inputLayout = new QHBoxLayout(inputWidget);

    messageInput = new QLineEdit(inputWidget);
    messageInput->setPlaceholderText("Type your message here...");
    sendButton = new QPushButton("Send", inputWidget);
    sendButton->setFixedWidth(80);

    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);
    leftLayout->addWidget(inputWidget);

    // Right side - Member list
    QWidget *rightPanel = new QWidget(splitter);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);

    memberCountLabel = new QLabel("Online Members (4 max): 0", rightPanel);
    memberCountLabel->setStyleSheet("QLabel { color: #555; font-weight: bold; }");
    rightLayout->addWidget(memberCountLabel);

    memberList = new QListWidget(rightPanel);
    memberList->setStyleSheet(
        "QListWidget { border: 1px solid #ddd; }"
        "QListWidget::item { padding: 5px; }"
        "QListWidget::item:hover { background-color: #e6f3ff; }"
        );
    rightLayout->addWidget(memberList);

    // Add some example members
    QStringList members = {"Alice", "Bob", "Charlie"};
    memberList->addItems(members);
    updateMemberCount();
    rightPanel->setFixedWidth(250);

    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);
    mainLayout->addWidget(splitter);

    // Manual addressing, very smart I know
    memberAddresses["Alice"] = 2;
    memberAddresses["Bob"] = 3;
    memberAddresses["Charlie"] = 4;
    // Connect signals
    connect(memberList, &QListWidget::itemClicked, this, &ChatRoomWindow::handleMemberClick);
    connect(sendButton, &QPushButton::clicked, this, &ChatRoomWindow::sendGlobalMessage);
    connect(messageInput, &QLineEdit::returnPressed, this, &ChatRoomWindow::sendGlobalMessage);
}
void ChatRoomWindow::updateMemberCount()
{
    int count = memberList->count();
    memberCountLabel->setText(QString("Online Members (4 max): %1").arg(count));
}

void ChatRoomWindow::addMember(const QString& memberName)
{
    memberList->addItem(memberName);
    updateMemberCount();
}

void ChatRoomWindow::removeMember(const QString& memberName)
{
    QList<QListWidgetItem*> items = memberList->findItems(memberName, Qt::MatchExactly);
    foreach(QListWidgetItem* item, items) {
        delete memberList->takeItem(memberList->row(item));
    }
    updateMemberCount();
}
void ChatRoomWindow::handleMemberClick(QListWidgetItem *item)
{
    QString memberName = item->text();
    if (memberName == "You") return;

    if (!privateChats.contains(memberName)) {
        // Get the member's address from your mapping
        uint8_t memberAddress = memberAddresses.value(memberName, 0); // Default to 0 if not found

        // Create new private chat window with all required parameters
        PrivateChatWindow *privateChat = new PrivateChatWindow(
            memberName,      // contactName
            memberAddress,   // contactAddress
            myAddress,       // myAddress (should be set elsewhere in your ChatRoomWindow)
            this            // parent
            );

        privateChats[memberName] = privateChat;

        connect(privateChat, &PrivateChatWindow::finished,
                this, [this, memberName](int result) {
                    Q_UNUSED(result);
                    privateChats.remove(memberName);
                });
    }

    // Show or bring to front
    privateChats[memberName]->show();
    privateChats[memberName]->raise();
    privateChats[memberName]->activateWindow();
}

void ChatRoomWindow::sendPrivateMessage(const QString &recipient)
{
    if (!privateChats.contains(recipient)) return;

    PrivateChatWindow *chatWindow = privateChats[recipient];
    QString message = chatWindow->privateMessageInput->text();

    if (!message.isEmpty()) {
        chatWindow->privateChatDisplay->append("You: " + message);
        chatWindow->privateMessageInput->clear();

        // Here you would normally send the message over the network, WIP
        // networkInterface->sendPrivateMessage(recipient, message);


        vector<char> message(message.begin(), message.end());
        //tm->sendMessage(message, static_cast<uint8_t>(dest), packet_header::types::data);


    }
}
void ChatRoomWindow::privateChatClosed(const QString &contactName)
{
    if (privateChats.contains(contactName)) {
        privateChats.remove(contactName);
    }
}
void ChatRoomWindow::sendGlobalMessage()
{
    QString message = messageInput->text();
    if (message.isEmpty()) return;

    // Create and store global message (address 0 is for broadcast)
    auto globalMsg = Message_Queue::msg_queue.create_message(
        myAddress,
        message.toStdString(),
        false,  // Not private
        false   // Not seen
        );
    Message_Queue::msg_queue.push_message(globalMsg, 0); // 0 = broadcast address, very shitty btw, will obv not work whith multiple windows on

    // Update UI immediately
    QString timestamp = QDateTime::currentDateTime().toString("[hh:mm]");
    chatDisplay->append(timestamp + " You: " + message);
    messageInput->clear();
}

void ChatRoomWindow::receiveGlobalMessage(const QString& sender, const QString& message)
{
    QString timestamp = QDateTime::currentDateTime().toString("[hh:mm]");
    chatDisplay->append(timestamp + " " + sender + ": " + message);
}
ChatRoomWindow::~ChatRoomWindow()
{
    // Clean up all private chat windows
    qDeleteAll(privateChats);
    privateChats.clear();
}
