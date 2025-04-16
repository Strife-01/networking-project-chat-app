/****************************************************************************
** Meta object code from reading C++ file 'chatroomwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "chatroomwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatroomwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_ChatRoomWindow_t {
    uint offsetsAndSizes[20];
    char stringdata0[15];
    char stringdata1[18];
    char stringdata2[1];
    char stringdata3[17];
    char stringdata4[5];
    char stringdata5[19];
    char stringdata6[10];
    char stringdata7[18];
    char stringdata8[12];
    char stringdata9[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_ChatRoomWindow_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_ChatRoomWindow_t qt_meta_stringdata_ChatRoomWindow = {
    {
        QT_MOC_LITERAL(0, 14),  // "ChatRoomWindow"
        QT_MOC_LITERAL(15, 17),  // "handleMemberClick"
        QT_MOC_LITERAL(33, 0),  // ""
        QT_MOC_LITERAL(34, 16),  // "QListWidgetItem*"
        QT_MOC_LITERAL(51, 4),  // "item"
        QT_MOC_LITERAL(56, 18),  // "sendPrivateMessage"
        QT_MOC_LITERAL(75, 9),  // "recipient"
        QT_MOC_LITERAL(85, 17),  // "privateChatClosed"
        QT_MOC_LITERAL(103, 11),  // "contactName"
        QT_MOC_LITERAL(115, 17)   // "sendGlobalMessage"
    },
    "ChatRoomWindow",
    "handleMemberClick",
    "",
    "QListWidgetItem*",
    "item",
    "sendPrivateMessage",
    "recipient",
    "privateChatClosed",
    "contactName",
    "sendGlobalMessage"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_ChatRoomWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x08,    1 /* Private */,
       5,    1,   41,    2, 0x08,    3 /* Private */,
       7,    1,   44,    2, 0x08,    5 /* Private */,
       9,    0,   47,    2, 0x08,    7 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject ChatRoomWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ChatRoomWindow.offsetsAndSizes,
    qt_meta_data_ChatRoomWindow,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_ChatRoomWindow_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ChatRoomWindow, std::true_type>,
        // method 'handleMemberClick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QListWidgetItem *, std::false_type>,
        // method 'sendPrivateMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'privateChatClosed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'sendGlobalMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void ChatRoomWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ChatRoomWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->handleMemberClick((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 1: _t->sendPrivateMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->privateChatClosed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->sendGlobalMessage(); break;
        default: ;
        }
    }
}

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
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
