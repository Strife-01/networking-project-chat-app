QT += widgets

INCLUDEPATH += $$PWD/../../backend/addressing \
               $$PWD/../../backend/channel_state \
               $$PWD/../../backend/fragmentation \
               $$PWD/../../backend/mac \
               $$PWD/../../backend/MessageQueue \
               $$PWD/../../backend/multihop-relay \
               $$PWD/../../backend/network \
               $$PWD/../../backend/routing \
               $$PWD/../../backend/transport \
               $$PWD/../../utils

DEPENDPATH += $$PWD/../../backend/addressing \
              $$PWD/../../backend/channel_state \
              $$PWD/../../backend/fragmentation \
              $$PWD/../../backend/mac \
              $$PWD/../../backend/MessageQueue \
              $$PWD/../../backend/multihop-relay \
              $$PWD/../../backend/network \
              $$PWD/../../backend/routing \
              $$PWD/../../backend/transport \
              $$PWD/../../utils

SOURCES += \
    main.cpp \
    chatroomwindow.cpp \
    privatechatwindow.cpp \
    $$PWD/../../backend/addressing/Addressing.cpp \
    $$PWD/../../backend/channel_state/ChannelState.cpp \
    $$PWD/../../backend/fragmentation/Fragmenter.cpp \
    $$PWD/../../backend/fragmentation/Reassembler.cpp \
    $$PWD/../../backend/mac/MediumAccessControl.cpp \
    $$PWD/../../backend/MessageQueue/MessageQueue.cpp \
    $$PWD/../../backend/multihop-relay/MultihopRelay.cpp \
    $$PWD/../../backend/network/Client.cpp \
    $$PWD/../../backend/network/DataReceiver.cpp \
    $$PWD/../../backend/routing/Route.cpp \
    $$PWD/../../backend/routing/VectorRoutingProtocol.cpp \
    $$PWD/../../backend/transport/StopAndWaitReceiver.cpp \
    $$PWD/../../backend/transport/StopAndWaitSender.cpp \
    $$PWD/../../backend/transport/TransportManager.cpp \
    $$PWD/../../utils/packet_header.cpp

HEADERS += \
    chatroomwindow.h \
    privatechatwindow.h \
    $$PWD/../../backend/addressing/Addressing.h \
    $$PWD/../../backend/channel_state/ChannelState.h \
    $$PWD/../../backend/fragmentation/Fragmenter.h \
    $$PWD/../../backend/fragmentation/Reassembler.h \
    $$PWD/../../backend/mac/MediumAccessControl.h \
    $$PWD/../../backend/MessageQueue/MessageQueue.h \
    $$PWD/../../backend/multihop-relay/MultihopRelay.h \
    $$PWD/../../backend/network/Client.h \
    $$PWD/../../backend/network/DataReceiver.h \
    $$PWD/../../backend/routing/Route.h \
    $$PWD/../../backend/routing/VectorRoutingProtocol.h \
    $$PWD/../../backend/transport/StopAndWaitReceiver.h \
    $$PWD/../../backend/transport/StopAndWaitSender.h \
    $$PWD/../../backend/transport/TransportManager.h \
    $$PWD/../../utils/BlockingQueue.h \
    $$PWD/../../utils/Message.h \
    $$PWD/../../utils/MessageType.h \
    $$PWD/../../utils/packet_header.h \
    $$PWD/../../utils/Random.h

