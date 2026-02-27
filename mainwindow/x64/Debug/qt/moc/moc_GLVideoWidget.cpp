/****************************************************************************
** Meta object code from reading C++ file 'GLVideoWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../GLVideoWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GLVideoWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GLVideoWidget_t {
    QByteArrayData data[5];
    char stringdata0[43];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GLVideoWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GLVideoWidget_t qt_meta_stringdata_GLVideoWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "GLVideoWidget"
QT_MOC_LITERAL(1, 14, 11), // "roiSelected"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 4), // "rect"
QT_MOC_LITERAL(4, 32, 10) // "widgetSize"

    },
    "GLVideoWidget\0roiSelected\0\0rect\0"
    "widgetSize"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GLVideoWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QRect, QMetaType::QSize,    3,    4,

       0        // eod
};

void GLVideoWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GLVideoWidget *_t = static_cast<GLVideoWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->roiSelected((*reinterpret_cast< QRect(*)>(_a[1])),(*reinterpret_cast< QSize(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GLVideoWidget::*_t)(QRect , QSize );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLVideoWidget::roiSelected)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject GLVideoWidget::staticMetaObject = {
    { &QOpenGLWidget::staticMetaObject, qt_meta_stringdata_GLVideoWidget.data,
      qt_meta_data_GLVideoWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *GLVideoWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GLVideoWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GLVideoWidget.stringdata0))
        return static_cast<void*>(const_cast< GLVideoWidget*>(this));
    if (!strcmp(_clname, "QOpenGLFunctions"))
        return static_cast< QOpenGLFunctions*>(const_cast< GLVideoWidget*>(this));
    return QOpenGLWidget::qt_metacast(_clname);
}

int GLVideoWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void GLVideoWidget::roiSelected(QRect _t1, QSize _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
