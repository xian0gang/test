/****************************************************************************
** Meta object code from reading C++ file 'camera.h'
**
** Created: Sat Mar 5 17:07:52 2016
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Camera/camera.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'camera.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Camera[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x08,
      31,    7,    7,    7, 0x08,
      53,    7,    7,    7, 0x08,
      63,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Camera[] = {
    "Camera\0\0on_pbt_start_clicked()\0"
    "on_pbt_stop_clicked()\0up_date()\0"
    "on_pushButton_clicked()\0"
};

const QMetaObject Camera::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Camera,
      qt_meta_data_Camera, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Camera::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Camera::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Camera::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Camera))
        return static_cast<void*>(const_cast< Camera*>(this));
    return QWidget::qt_metacast(_clname);
}

int Camera::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_pbt_start_clicked(); break;
        case 1: on_pbt_stop_clicked(); break;
        case 2: up_date(); break;
        case 3: on_pushButton_clicked(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
