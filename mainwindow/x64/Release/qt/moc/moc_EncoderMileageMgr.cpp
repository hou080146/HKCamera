/****************************************************************************
** Meta object code from reading C++ file 'EncoderMileageMgr.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../EncoderMileageMgr.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EncoderMileageMgr.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_EncoderMileageMgr_t {
    QByteArrayData data[18];
    char stringdata0[208];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EncoderMileageMgr_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EncoderMileageMgr_t qt_meta_stringdata_EncoderMileageMgr = {
    {
QT_MOC_LITERAL(0, 0, 17), // "EncoderMileageMgr"
QT_MOC_LITERAL(1, 18, 14), // "mileageUpdated"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 13), // "MileageStatus"
QT_MOC_LITERAL(4, 48, 6), // "status"
QT_MOC_LITERAL(5, 55, 10), // "calibrated"
QT_MOC_LITERAL(6, 66, 10), // "tcpMileage"
QT_MOC_LITERAL(7, 77, 12), // "localMileage"
QT_MOC_LITERAL(8, 90, 6), // "offset"
QT_MOC_LITERAL(9, 97, 13), // "errorOccurred"
QT_MOC_LITERAL(10, 111, 5), // "error"
QT_MOC_LITERAL(11, 117, 21), // "onEncoderDataReceived"
QT_MOC_LITERAL(12, 139, 11), // "EncoderData"
QT_MOC_LITERAL(13, 151, 4), // "data"
QT_MOC_LITERAL(14, 156, 20), // "onTcpMileageReceived"
QT_MOC_LITERAL(15, 177, 11), // "MileageInfo"
QT_MOC_LITERAL(16, 189, 4), // "info"
QT_MOC_LITERAL(17, 194, 13) // "onUpdateTimer"

    },
    "EncoderMileageMgr\0mileageUpdated\0\0"
    "MileageStatus\0status\0calibrated\0"
    "tcpMileage\0localMileage\0offset\0"
    "errorOccurred\0error\0onEncoderDataReceived\0"
    "EncoderData\0data\0onTcpMileageReceived\0"
    "MileageInfo\0info\0onUpdateTimer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EncoderMileageMgr[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       5,    3,   47,    2, 0x06 /* Public */,
       9,    1,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    1,   57,    2, 0x08 /* Private */,
      14,    1,   60,    2, 0x08 /* Private */,
      17,    0,   63,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double,    6,    7,    8,
    QMetaType::Void, QMetaType::QString,   10,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void,

       0        // eod
};

void EncoderMileageMgr::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EncoderMileageMgr *_t = static_cast<EncoderMileageMgr *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->mileageUpdated((*reinterpret_cast< const MileageStatus(*)>(_a[1]))); break;
        case 1: _t->calibrated((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 2: _t->errorOccurred((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->onEncoderDataReceived((*reinterpret_cast< const EncoderData(*)>(_a[1]))); break;
        case 4: _t->onTcpMileageReceived((*reinterpret_cast< const MileageInfo(*)>(_a[1]))); break;
        case 5: _t->onUpdateTimer(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (EncoderMileageMgr::*_t)(const MileageStatus & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EncoderMileageMgr::mileageUpdated)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (EncoderMileageMgr::*_t)(double , double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EncoderMileageMgr::calibrated)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (EncoderMileageMgr::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EncoderMileageMgr::errorOccurred)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject EncoderMileageMgr::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_EncoderMileageMgr.data,
      qt_meta_data_EncoderMileageMgr,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *EncoderMileageMgr::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EncoderMileageMgr::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EncoderMileageMgr.stringdata0))
        return static_cast<void*>(const_cast< EncoderMileageMgr*>(this));
    return QObject::qt_metacast(_clname);
}

int EncoderMileageMgr::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void EncoderMileageMgr::mileageUpdated(const MileageStatus & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void EncoderMileageMgr::calibrated(double _t1, double _t2, double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void EncoderMileageMgr::errorOccurred(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
