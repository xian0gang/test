/****************************************************************************
** Meta object code from reading C++ file 'mylable.h'
**
** Created: Mon Mar 7 16:37:09 2016
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QtOpenCvTongue/mylable.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mylable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MyLable[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,    9,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,    8,    8,    8, 0x0a,
      65,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MyLable[] = {
    "MyLable\0\0red,green,blue\0"
    "send_rgb_num(int,int,int)\0set_filename()\0"
    "set_scaled_button()\0"
};

const QMetaObject MyLable::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_MyLable,
      qt_meta_data_MyLable, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MyLable::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MyLable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MyLable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MyLable))
        return static_cast<void*>(const_cast< MyLable*>(this));
    return QLabel::qt_metacast(_clname);
}

int MyLable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: send_rgb_num((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: set_filename(); break;
        case 2: set_scaled_button(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void MyLable::send_rgb_num(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
