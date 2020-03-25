/****************************************************************************
** Meta object code from reading C++ file 'polartohsvchnbl.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../polartohsvchnbl.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'polartohsvchnbl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CxPolarToHSVChnbl_t {
    QByteArrayData data[4];
    char stringdata0[55];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CxPolarToHSVChnbl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CxPolarToHSVChnbl_t qt_meta_stringdata_CxPolarToHSVChnbl = {
    {
QT_MOC_LITERAL(0, 0, 17), // "CxPolarToHSVChnbl"
QT_MOC_LITERAL(1, 18, 10), // "CustomName"
QT_MOC_LITERAL(2, 29, 24), // "Polarization: Map to HSV"
QT_MOC_LITERAL(3, 54, 0) // ""

    },
    "CxPolarToHSVChnbl\0CustomName\0"
    "Polarization: Map to HSV\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CxPolarToHSVChnbl[] = {

 // content:
       7,       // revision
       0,       // classname
       1,   14, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       1,   17, // constructors
       0,       // flags
       0,       // signalCount

 // classinfo: key, value
       1,    2,

 // constructors: parameters
    0x80000000 | 3,

 // constructors: name, argc, parameters, tag, flags
       0,    0,   16,    3, 0x0e /* Public */,

       0        // eod
};

void CxPolarToHSVChnbl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::CreateInstance) {
        switch (_id) {
        case 0: { CxPolarToHSVChnbl *_r = new CxPolarToHSVChnbl();
            if (_a[0]) *reinterpret_cast<QObject**>(_a[0]) = _r; } break;
        default: break;
        }
    }
    Q_UNUSED(_o);
}

const QMetaObject CxPolarToHSVChnbl::staticMetaObject = {
    { &CxAbstractPolarChnbl::staticMetaObject, qt_meta_stringdata_CxPolarToHSVChnbl.data,
      qt_meta_data_CxPolarToHSVChnbl,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CxPolarToHSVChnbl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CxPolarToHSVChnbl::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CxPolarToHSVChnbl.stringdata0))
        return static_cast<void*>(const_cast< CxPolarToHSVChnbl*>(this));
    return CxAbstractPolarChnbl::qt_metacast(_clname);
}

int CxPolarToHSVChnbl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CxAbstractPolarChnbl::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
