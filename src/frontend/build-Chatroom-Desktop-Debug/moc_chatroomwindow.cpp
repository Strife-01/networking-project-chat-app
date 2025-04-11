/****************************************************************************
** Meta object code from reading C++ file 'chatroomwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../Chatroom/chatroomwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatroomwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ChatRoomWindow_t {
    QByteArrayData data[13];
    char stringdata0[169];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChatRoomWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChatRoomWindow_t qt_meta_stringdata_ChatRoomWindow = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ChatRoomWindow"
QT_MOC_LITERAL(1, 15, 17), // "handleMemberClick"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(4, 51, 4), // "item"
QT_MOC_LITERAL(5, 56, 18), // "sendPrivateMessage"
QT_MOC_LITERAL(6, 75, 9), // "recipient"
QT_MOC_LITERAL(7, 85, 17), // "privateChatClosed"
QT_MOC_LITERAL(8, 103, 11), // "contactName"
QT_MOC_LITERAL(9, 115, 17), // "sendGlobalMessage"
QT_MOC_LITERAL(10, 133, 20), // "receiveGlobalMessage"
QT_MOC_LITERAL(11, 154, 6), // "sender"
QT_MOC_LITERAL(12, 161, 7) // "message"

    },
    "ChatRoomWindow\0handleMemberClick\0\0"
    "QListWidgetItem*\0item\0sendPrivateMessage\0"
    "recipient\0privateChatClosed\0contactName\0"
    "sendGlobalMessage\0receiveGlobalMessage\0"
    "sender\0message"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChatRoomWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x08 /* Private */,
       5,    1,   42,    2, 0x08 /* Private */,
       7,    1,   45,    2, 0x08 /* Private */,
       9,    0,   48,    2, 0x08 /* Private */,
      10,    2,   49,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   11,   12,

       0        // eod
};

void ChatRoomWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ChatRoomWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->handleMemberClick((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->sendPrivateMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->privateChatClosed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->sendGlobalMessage(); break;
        case 4: _t->receiveGlobalMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ChatRoomWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ChatRoomWindow.data,
    qt_meta_data_ChatRoomWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ChatRoomWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatRoomWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ChatRoomWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int ChatRoomWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
