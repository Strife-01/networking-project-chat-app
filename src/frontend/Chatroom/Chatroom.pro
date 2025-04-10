QT += widgets

INCLUDEPATH += $$PWD/../../backend/MessageQueue
DEPENDPATH += $$PWD/../../backend/MessageQueue

SOURCES += \
    main.cpp \
    chatroomwindow.cpp \
    privatechatwindow.cpp \
    $$PWD/../../backend/MessageQueue/MessageQueue.cpp

HEADERS += \
    chatroomwindow.h \
    privatechatwindow.h \
    $$PWD/../../backend/MessageQueue/MessageQueue.h
