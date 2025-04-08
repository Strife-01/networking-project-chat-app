#include "chatroomwindow.h"
#include "privatechatwindow.h"

ChatRoomWindow::ChatRoomWindow(QWidget *parent)
    : QMainWindow(parent)
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

    // Input area at bottom
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

    // Connect signals
    connect(memberList, &QListWidget::itemClicked, this, &ChatRoomWindow::handleMemberClick);
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

    if (!privateChats.contains(memberName)) {
        // Create new private chat window
        PrivateChatWindow *privateChat = new PrivateChatWindow(memberName, this);
        privateChats[memberName] = privateChat;

        // Connect signals
        connect(privateChat->privateSendButton, &QPushButton::clicked,
                this, [this, memberName]() { sendPrivateMessage(memberName); });

        // Handle window closing
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

        // Here you would normally send the message over the network
        // networkInterface->sendPrivateMessage(recipient, message);
    }
}
void ChatRoomWindow::privateChatClosed(const QString &contactName)
{
    if (privateChats.contains(contactName)) {
        privateChats.remove(contactName);
    }
}
ChatRoomWindow::~ChatRoomWindow()
{
    // Clean up all private chat windows
    qDeleteAll(privateChats);
    privateChats.clear();
}
