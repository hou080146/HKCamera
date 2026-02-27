/****************************************************************************
** Meta object code from reading C++ file 'hectometerClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../hectometerClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hectometerClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_hectometerClient_t {
    QByteArrayData data[16];
    char stringdata0[206];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_hectometerClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_hectometerClient_t qt_meta_stringdata_hectometerClient = {
    {
QT_MOC_LITERAL(0, 0, 16), // "hectometerClient"
QT_MOC_LITERAL(1, 17, 9), // "connected"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 12), // "disconnected"
QT_MOC_LITERAL(4, 41, 13), // "errorOccurred"
QT_MOC_LITERAL(5, 55, 5), // "error"
QT_MOC_LITERAL(6, 61, 19), // "mileageInfoReceived"
QT_MOC_LITERAL(7, 81, 11), // "MileageInfo"
QT_MOC_LITERAL(8, 93, 4), // "info"
QT_MOC_LITERAL(9, 98, 11), // "onConnected"
QT_MOC_LITERAL(10, 110, 14), // "onDisconnected"
QT_MOC_LITERAL(11, 125, 11), // "onReadyRead"
QT_MOC_LITERAL(12, 137, 13), // "onSocketError"
QT_MOC_LITERAL(13, 151, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(14, 180, 11), // "socketError"
QT_MOC_LITERAL(15, 192, 13) // "sendHeartbeat"

    },
    "hectometerClient\0connected\0\0disconnected\0"
    "errorOccurred\0error\0mileageInfoReceived\0"
    "MileageInfo\0info\0onConnected\0"
    "onDisconnected\0onReadyRead\0onSocketError\0"
    "QAbstractSocket::SocketError\0socketError\0"
    "sendHeartbeat"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_hectometerClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    0,   60,    2, 0x06 /* Public */,
       4,    1,   61,    2, 0x06 /* Public */,
       6,    1,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   67,    2, 0x08 /* Private */,
      10,    0,   68,    2, 0x08 /* Private */,
      11,    0,   69,    2, 0x08 /* Private */,
      12,    1,   70,    2, 0x08 /* Private */,
      15,    0,   73,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, 0x80000000 | 7,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void,

       0        // eod
};

void hectometerClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        hectometerClient *_t = static_cast<hectometerClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->mileageInfoReceived((*reinterpret_cast< const MileageInfo(*)>(_a[1]))); break;
        case 4: _t->onConnected(); break;
        case 5: _t->onDisconnected(); break;
        case 6: _t->onReadyRead(); break;
        case 7: _t->onSocketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 8: _t->sendHeartbeat(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (hectometerClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&hectometerClient::connected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (hectometerClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&hectometerClient::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (hectometerClient::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&hectometerClient::errorOccurred)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (hectometerClient::*_t)(const MileageInfo & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&hectometerClient::mileageInfoReceived)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject hectometerClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_hectometerClient.data,
      qt_meta_data_hectometerClient,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *hectometerClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *hectometerClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_hectometerClient.stringdata0))
        return static_cast<void*>(const_cast< hectometerClient*>(this));
    return QObject::qt_metacast(_clname);
}

int hectometerClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void hectometerClient::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void hectometerClient::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void hectometerClient::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void hectometerClient::mileageInfoReceived(const MileageInfo & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
