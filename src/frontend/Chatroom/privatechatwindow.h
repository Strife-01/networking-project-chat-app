#ifndef PRIVATECHATWINDOW_H
#define PRIVATECHATWINDOW_H

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include "../../backend/transport/TransportManager.h"

class PrivateChatWindow : public QDialog
{
    Q_OBJECT

public:
    PrivateChatWindow(const QString &contactName, uint8_t contactAddress, uint8_t myAddress, QWidget *parent = nullptr);
    ~PrivateChatWindow();

    // UI Elements for private chat
    QTextEdit *privateChatDisplay;
    QLineEdit *privateMessageInput;
    QPushButton *privateSendButton;


    TransportManager * tm;

    QString contactName;
    void sendMessage(const QString &message);
    void loadHistory();
    //void saveMessage(uint8_t myAddress, const std::string& message);
    //void loadHistory(uint8_t myAddress);

private:
    void setupPrivateChatUI();
    uint8_t contactAddress;
    uint8_t myAddress;
};

#endif // PRIVATECHATWINDOW_H
