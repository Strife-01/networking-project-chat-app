#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <QMap>
#include <QString>

class QTextEdit;
class QLineEdit;
class QPushButton;
class QListWidget;

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(QWidget *parent = nullptr);

private slots:
    void sendMessage();
    void userJoined();
    void userLeft();
    void updateUserList();

private:
    void setupUI();
    void setupConnections();
    void appendMessage(const QString &message, const QString &sender = "System");

    QTextEdit *messageDisplay;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    QListWidget *userList;

    QMap<QString, bool> users;  // 存储用户状态
    QString currentUser;        // 当前用户标识
    int userCounter = 0;        // 用户计数器
};

#endif // CHATWIDGET_H
