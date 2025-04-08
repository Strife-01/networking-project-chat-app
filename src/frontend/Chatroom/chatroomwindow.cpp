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

    rightLayout->addWidget(new QLabel("Online Members (4 max):", rightPanel));

    memberList = new QListWidget(rightPanel);
    memberList->setStyleSheet(
        "QListWidget { border: 1px solid #ddd; }"
        "QListWidget::item { padding: 5px; }"
        "QListWidget::item:hover { background-color: #e6f3ff; }"
        );
    rightLayout->addWidget(memberList);

    // Add some example members
    QStringList members = {"Alice", "Bob", "Charlie", "You"};
    memberList->addItems(members);

    // Set member list width
    rightPanel->setFixedWidth(200);

    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);

    mainLayout->addWidget(splitter);
}

ChatRoomWindow::~ChatRoomWindow()
{
    // In ChatRoomWindow constructor, after setting up UI:
    connect(memberList, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {
        QString memberName = item->text();
        if (memberName != "You") { // Don't open chat with yourself
            privateChat->contactName = memberName;
            privateChat->show();
            //privatechatDisplay->clear();
        }else{
            privateChat->hide();

        }
    });
}
