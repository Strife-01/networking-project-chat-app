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


    memberList->addItems(members);
    updateMemberCount();
    rightPanel->setFixedWidth(250);

    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);
    mainLayout->addWidget(splitter);

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
            dynamic_addressing::get_my_addr(),       // myAddress (should be set elsewhere in your ChatRoomWindow)
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
    QString shitty_format_msg = chatWindow->privateMessageInput->text();

    if (!shitty_format_msg.isEmpty()) {
        chatWindow->privateChatDisplay->append("You: " + shitty_format_msg);
        chatWindow->privateMessageInput->clear();

        // Here you would normally send the message over the network, WIP
        // networkInterface->sendPrivateMessage(recipient, message);


        vector<char> message;

        QByteArray ba = shitty_format_msg.toLocal8Bit().data();

        for(int i =0;i<ba.length();i++){
            message.push_back(ba[i]);
        }

        tm->sendMessage(message, static_cast<uint8_t>(memberAddresses[recipient]), packet_header::types::data);

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
    QString shitty_format_msg = messageInput->text();
    if (shitty_format_msg.isEmpty()) return;

    QByteArray ba = shitty_format_msg.toLocal8Bit();
    vector<char> message;
    for(int i = 0;i<ba.length();i++){
        message.push_back(ba[i]);
    }

    tm->sendMessage(message, static_cast<uint8_t>(0), packet_header::types::data);


    // Update UI immediately
    QString timestamp = QDateTime::currentDateTime().toString("[hh:mm]");
    chatDisplay->append(timestamp + " You: " + shitty_format_msg);
    messageInput->clear();
}

void ChatRoomWindow::receiveGlobalMessage(uint8_t sender, std::vector<char> msg)
{
    msg.push_back('\0');
    const QString sender_str = QString::number(sender);
    const QString message = QString::fromLatin1(msg);

    QString timestamp = QDateTime::currentDateTime().toString("[hh:mm]");
    chatDisplay->append(timestamp + " " + sender_str + ": " + message);
}


void ChatRoomWindow::receivePrivateMessage(uint8_t sender, std::vector<char> msg)
{
    msg.push_back('\0');

    const QString sender_str = "Node_"+QString::number(sender);
    const QString message = QString::fromLatin1(msg);

    PrivateChatWindow *chatWindow = privateChats[sender_str];
    chatWindow->privateChatDisplay->append(sender_str+" : " + message);
    chatWindow->privateMessageInput->clear();
}

ChatRoomWindow::~ChatRoomWindow()
{
    // Clean up all private chat windows
    qDeleteAll(privateChats);
    privateChats.clear();
}

void ChatRoomWindow::updateMemberList(){

    members.clear();
    for(char i=1;i<=MAX_NODE_NUMBER;i++){

        if(vector_routing_protocol::reachable_nodes[i] && i != dynamic_addressing::get_my_addr()){
            QString addr = "Node_" +QString::number(i);
            members.append(addr);
            memberAddresses[addr] = i;
        }
    }

    memberList->clear();
    memberList->addItems(members);
    updateMemberCount();
}
