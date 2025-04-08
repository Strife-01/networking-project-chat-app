#ifndef PRIVATECHATWINDOW_H
#define PRIVATECHATWINDOW_H

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class PrivateChatWindow : public QDialog
{
    Q_OBJECT

public:
    PrivateChatWindow(const QString &contactName, QWidget *parent = nullptr);
    ~PrivateChatWindow();

    // UI Elements for private chat
    QTextEdit *privateChatDisplay;
    QLineEdit *privateMessageInput;
    QPushButton *privateSendButton;

    QString contactName;


private:
    void setupPrivateChatUI();
};

#endif // PRIVATECHATWINDOW_H
